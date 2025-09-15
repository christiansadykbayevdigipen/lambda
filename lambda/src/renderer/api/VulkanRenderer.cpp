#include "VulkanRenderer.h"

#include <vector>
#include <cstring>
#include <vk_video/vulkan_video_codec_h264std_encode.h>
#include <vulkan/vulkan_core.h>
#include <optional>
#include <set>

#define VK_CHECK(X)\
{\
VkResult __a = X;\
if(__a != VK_SUCCESS)\
{\
	LAMBDA_FATAL("The vulkan error ", __a, " has been thrown");\
}\
}

namespace lambda {
	// static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    // VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    // VkDebugUtilsMessageTypeFlagsEXT messageType,
    // const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    // void* pUserData) {
	// 	switch(messageSeverity) {
	// 	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
	// 		LAMBDA_ERROR("Vulkan validation layer: ", pCallbackData->pMessage);
	// 		break;
	// 	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
	// 		LAMBDA_WARN("Vulkan validation layer: ", pCallbackData->pMessage);
	// 		break;
	// 	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
	// 		LAMBDA_DEBUG("Vulkan validation layer: ", pCallbackData->pMessage);
	// 		break;
	// 	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
	// 		LAMBDA_INFO("Vulkan validation layer: ", pCallbackData->pMessage);
	// 		break;
	// 	default:
	// 		LAMBDA_INFO("(unknown message severity) Vulkan validation layer: ", pCallbackData->pMessage);
	// 		break;
	// 	}

	// 	return VK_FALSE;
	// }

	struct QueueFamilyIndices {
		std::optional<u32> GraphicsFamilyIndex;
		std::optional<u32> PresentFamilyIndex;

		bool IsComplete() {
			return GraphicsFamilyIndex.has_value() && PresentFamilyIndex.has_value();
		}
	};

	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	static std::vector<const char*> GetLayers() {
		LAMBDA_AUTOTRACE();
		std::vector<const char*> layers;

		u32 length = 0;
		vkEnumerateInstanceLayerProperties(&length, nullptr);
		std::vector<VkLayerProperties> properties(length);
		vkEnumerateInstanceLayerProperties(&length, properties.data());

		for(const auto& layerProperties : properties) {
			#ifdef DEBUG
			if(strcmp(layerProperties.layerName, "VK_LAYER_KHRONOS_validation") == 0) {
				layers.push_back("VK_LAYER_KHRONOS_validation");
			}
			#endif
		}

		#ifdef DEBUG
		if(layers.size() <= 0) {
			LAMBDA_WARN("The debug build of Lambda requests that Vulkan thus has debug layers, however, when enumerating Vulkan for debug layers, there appears to be none. You may need to install the Vulkan SDK");
		}
		#endif

		return layers;
	}

	static VkInstance InitializeInstance(std::shared_ptr<IWindow> window) {
		LAMBDA_AUTOTRACE();

		u32 apiVersion = 0;

		VK_CHECK(vkEnumerateInstanceVersion(&apiVersion));

		VkApplicationInfo applicationInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
		applicationInfo.pApplicationName = "Lambda Application";
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "Lambda";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.apiVersion = apiVersion;

		std::vector<const char*> layers = GetLayers();

		VkInstanceCreateInfo info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
		info.pApplicationInfo = &applicationInfo;
		info.enabledLayerCount = layers.size();
		info.ppEnabledLayerNames = layers.data();
		std::vector<const char*> extensions = window->GetVulkanRequiredInstanceExtensions();

		// Add debug report utils
		#ifdef DEBUG
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		#endif

		info.enabledExtensionCount = extensions.size();
		info.ppEnabledExtensionNames = extensions.data();
		
		VkInstance instance;

		VK_CHECK(vkCreateInstance(&info, nullptr, &instance));

		if(instance == nullptr)
			LAMBDA_FATAL("lambda::InitializeInstance has encountered an unrecoverable error. vkCreateInstance outputted a nullpointer for pInstance");
		return instance;
	}

	static QueueFamilyIndices LocateQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
		LAMBDA_AUTOTRACE();

		QueueFamilyIndices indices;

		u32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

		u32 i = 0;
		for(const auto& queueFamily : queueFamilyProperties) {
			if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.GraphicsFamilyIndex = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if(presentSupport) {
				indices.PresentFamilyIndex = i;
			}

			if(indices.IsComplete()) break;

			i++;
		}
		
		return indices;
	}

	static bool CheckAvailableDeviceExtensions(VkPhysicalDevice device, std::vector<const char*> deviceExtensions) {
		LAMBDA_AUTOTRACE();
		u32 extension_count;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extension_count);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
		for(const auto& ext : availableExtensions) {
			requiredExtensions.erase(ext.extensionName);
		}

		return requiredExtensions.empty();
	}

	static SwapchainSupportDetails GetSwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
		LAMBDA_AUTOTRACE();
		SwapchainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);
		
		u32 formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if(formatCount <= 0) {
			LAMBDA_FATAL("lambda::GetSwapchainSupport has encountered an unrecoverable error. The physical device surface format count is less than or equal to zero.");
			return {};
		}

		details.Formats.resize(formatCount);
		
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.Formats.data());

		u32 presentModeCount = 0;

		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if(presentModeCount <= 0) {
			LAMBDA_FATAL("lambda::GetSwapchainSupport has encountered an unrecoverable error. The physical device surface present mode count is less than or equal to zero.");
			return {};
		}

		details.PresentModes.resize(presentModeCount);

		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.PresentModes.data());
		
		return details;
	}

	static bool IsPhysicalDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char*>& deviceExtensions) {
		LAMBDA_AUTOTRACE();

		bool isRequiredExtensionsAvailable = CheckAvailableDeviceExtensions(device, deviceExtensions);
		bool isSwapchainCompatible = false;

		if(isRequiredExtensionsAvailable) {
			SwapchainSupportDetails details = GetSwapchainSupport(device, surface);
			isSwapchainCompatible = !details.Formats.empty() && !details.PresentModes.empty();
		}

		return LocateQueueFamilies(device, surface).IsComplete() && isRequiredExtensionsAvailable && isSwapchainCompatible;
	}

	// This function may be problematic. I have no way of testing with a multi GPU setup to see if it properly picks the right physical device.
	// static VkPhysicalDevice CalculateHighestPerformingGPU(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& deviceExtensions) {
	// 	LAMBDA_AUTOTRACE();

	// 	u32 gpu_count = 0;
	// 	VK_CHECK(vkEnumeratePhysicalDevices(instance, &gpu_count, nullptr));
	// 	std::vector<VkPhysicalDevice> gpus(gpu_count);
	// 	VK_CHECK(vkEnumeratePhysicalDevices(instance, &gpu_count, gpus.data()));

	// 	if(gpu_count <= 0) {
	// 		LAMBDA_FATAL("lambda::CalculateHighestPerformingGPU has encountered and unrecoverable error. There appears to be less than one suitable physical device for Vulkan. Please double-check the graphics card in your system can utilize Vulkan. Otherwise, please restart your computer, and if that does not work, reinstall the Vulkan runtime and your graphics drivers.");
	// 		return VK_NULL_HANDLE;
	// 	}

	// 	u32 currentStrongestGPU = -1;
	// 	VkPhysicalDeviceProperties currentStrongestGPUProperties;
	// 	bool discreteFound = false;
	// 	u32 currentHighestMemoryHeapCount = 0;
	
	// 	u32 i = 0;
	// 	for(auto gpu : gpus) {
	// 		VkPhysicalDeviceProperties properties;
	// 		vkGetPhysicalDeviceProperties(gpu, &properties);

	// 		// First, check if the physical device even has proper support for this engine. If not, continue on with the next GPU.
	// 		if(IsPhysicalDeviceSuitable(gpu, surface, deviceExtensions)) {
	// 			if(currentStrongestGPU == -1)
	// 				currentStrongestGPU = i;
	// 		}
	// 		else {
	// 			continue;
	// 		}

	// 		if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
	// 			if(!discreteFound) {
	// 				discreteFound = true;
	// 				currentStrongestGPU = i;
	// 				currentStrongestGPUProperties = properties;
	// 				continue;
	// 			}

	// 			VkPhysicalDeviceMemoryProperties props;
	// 			vkGetPhysicalDeviceMemoryProperties(gpu, &props);
	// 			if(props.memoryHeapCount > currentHighestMemoryHeapCount) {
	// 				currentHighestMemoryHeapCount = props.memoryHeapCount;
	// 				currentStrongestGPU = i;
	// 				currentStrongestGPUProperties = properties;
	// 			}
	// 		}

	// 		i++;
	// 	}


	// 	if(currentStrongestGPUProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
	// 		LAMBDA_WARN("The strongest GPU in this system is an integrated GPU. Performance may struggle and there is a high likelihood of low frame-rates and stuttering");
	// 	}

	// 	LAMBDA_DEBUG("The picked GPU for Vulkan is a(n) ", currentStrongestGPUProperties.deviceName);

	// 	return gpus[currentStrongestGPU];
	// }

	// Please Refactor in a later date
	// TODO: Improve upon this function for an actual calculation instead of just returning the first GPU in the system.
	static VkPhysicalDevice CalculateHighestPerformingGPU(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& deviceExtensions) {
		LAMBDA_AUTOTRACE();

		u32 gpu_count = 0;
		VK_CHECK(vkEnumeratePhysicalDevices(instance, &gpu_count, nullptr));
		std::vector<VkPhysicalDevice> gpus(gpu_count);
		VK_CHECK(vkEnumeratePhysicalDevices(instance, &gpu_count, gpus.data()));

		if(gpu_count <= 0) {
			LAMBDA_FATAL("lambda::CalculateHighestPerformingGPU has encountered and unrecoverable error. There appears to be less than one suitable physical device for Vulkan. Please double-check the graphics card in your system can utilize Vulkan. Otherwise, please restart your computer, and if that does not work, reinstall the Vulkan runtime and your graphics drivers.");
			return VK_NULL_HANDLE;
		}

		if(IsPhysicalDeviceSuitable(gpus[0], surface, deviceExtensions))
			return gpus[0];
		else {
			LAMBDA_FATAL("lambda::CalculateHighestPerformingGPU has encountered an unrecoverable error. The first GPU is not suitable.");
			return VK_NULL_HANDLE;
		}
	}

	static VkDevice InitializeLogicalDevice(VkPhysicalDevice gpu, VkSurfaceKHR surface, const std::vector<const char*> deviceExtensions, VkQueue* outputGraphicsQueue, VkQueue* outputPresentQueue) {
		LAMBDA_AUTOTRACE();

		VkDevice logicalDevice = VK_NULL_HANDLE;

		QueueFamilyIndices indices = LocateQueueFamilies(gpu, surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

		std::set<u32> queues = {indices.PresentFamilyIndex.value(), indices.GraphicsFamilyIndex.value()};

		for(u32 queueFamily : queues) {
			VkDeviceQueueCreateInfo queueCreateInfo = {VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			f32 priority = 1.0f;
			queueCreateInfo.pQueuePriorities = &priority;

			queueCreateInfos.push_back(queueCreateInfo);
		}


		VkPhysicalDeviceFeatures physicalDeviceFeatures = {};

		VkDeviceCreateInfo deviceCreateInfo = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
		deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
		deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

		VK_CHECK(vkCreateDevice(gpu, &deviceCreateInfo, nullptr, &logicalDevice));

		if(logicalDevice == VK_NULL_HANDLE) {
			LAMBDA_FATAL("lambda::InitializeLogicalDevice has encountered an unrecoverable error. vkCreateDevice did not return a valid logical device handle.");
		}

		vkGetDeviceQueue(logicalDevice, indices.GraphicsFamilyIndex.value(), 0, outputGraphicsQueue);
		vkGetDeviceQueue(logicalDevice, indices.PresentFamilyIndex.value(), 0, outputPresentQueue);

		return logicalDevice;
	}

	void VulkanRenderer::Init(std::shared_ptr<IWindow> window) {
		LAMBDA_AUTOTRACE();
		m_Window = window;

		const std::vector<const char*> deviceExtensions = 
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};

		m_Instance = InitializeInstance(m_Window);
		m_Surface = reinterpret_cast<VkSurfaceKHR>(window->CreateVulkanSurface(reinterpret_cast<void*>(m_Instance)));
		m_PhysicalDevice = CalculateHighestPerformingGPU(m_Instance, m_Surface, deviceExtensions);
		m_LogicalDevice = InitializeLogicalDevice(m_PhysicalDevice, m_Surface, deviceExtensions, &m_GraphicsQueue, &m_PresentQueue);
		if(!m_GraphicsQueue) {
			LAMBDA_FATAL("lambda::VulkanRenderer::Init has encountered an unrecoverable error. The graphics queue associated with the logical device does not exist");
			return;
		}
	}

	void VulkanRenderer::PreRender() {

	}

	void VulkanRenderer::PostRender() {

	}

	VulkanRenderer::~VulkanRenderer() {
		LAMBDA_AUTOTRACE();

		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		vkDestroyDevice(m_LogicalDevice, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
	}
}