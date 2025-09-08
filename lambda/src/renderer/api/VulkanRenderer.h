#pragma once

#include "core.h"
#include "renderer/renderer.h"

#include <vulkan/vulkan.h>

namespace lambda {
	class VulkanRenderer : public IRenderer {
	public:
		~VulkanRenderer();

		virtual void Init(std::shared_ptr<IWindow> window) override;
		virtual void PreRender() override;
		virtual void PostRender() override;
	private:
		std::shared_ptr<IWindow> m_Window;

		VkInstance m_Instance;
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_LogicalDevice;
		VkQueue m_GraphicsQueue;
		VkSurfaceKHR m_Surface;
	};
}