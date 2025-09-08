#pragma once

#include "core.h"
#include "window/window.h"
#include "renderer/renderer.h"

namespace lambda {
	struct ApplicationSettings {
		u32 WindowWidth;
		u32 WindowHeight;
		std::string Title;
		RenderingAPI Api;
	};
	
	class Application {
	public:
		Application(const ApplicationSettings& settings);
		~Application();

		void Run();

		virtual void OnStart() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnDestroy() {}
	private:
		std::shared_ptr<IWindow> m_Window;
		std::shared_ptr<IRenderer> m_Renderer;
	};
}