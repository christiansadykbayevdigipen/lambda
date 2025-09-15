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

		virtual Mesh CreateMesh(std::vector<f32> vertexCoordinates) override{return-1;}
		virtual ShaderUnit CreateShader(const std::string& vertexShader, const std::string& pixelShader) override{return-1;}

		virtual void DestroyMesh(Mesh mesh) override{}
		virtual void DestroyShader(ShaderUnit shader) override{}
		virtual void Render(Mesh mesh, ShaderUnit shader) override{}
	private:
		std::shared_ptr<IWindow> m_Window;

		VkInstance m_Instance;
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_LogicalDevice;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		VkSurfaceKHR m_Surface;
	};
}