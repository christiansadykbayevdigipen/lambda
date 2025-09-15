#include "renderer.h"
#include "core.h"

#include "api/VulkanRenderer.h"
#include "api/OpenGLRenderer.h"

namespace lambda {
	std::shared_ptr<IRenderer> IRenderer::CreateRenderer(std::shared_ptr<IWindow> window, RenderingAPI api) {
		LAMBDA_AUTOTRACE();

		std::shared_ptr<IRenderer> renderer = nullptr;

		switch(api) {
		case RenderingAPI_OpenGL:
			renderer = std::make_shared<OpenGLRenderer>();
			break;
		case RenderingAPI_Vulkan:
			renderer = std::make_shared<VulkanRenderer>();
			break;
		default:
			LAMBDA_FATAL("lambda::IRenderer::CreateRenderer encountered an unrecoverable error. The API given does not exist or is not supported");
			return nullptr;
			break;
		}

		renderer->Init(window);
		
		return renderer;
	}
}