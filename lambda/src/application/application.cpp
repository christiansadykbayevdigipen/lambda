#include "application.h"

#include <chrono>
#include <thread>

namespace lambda {
	Application::Application(const ApplicationSettings& settings) {
		LAMBDA_AUTOTRACE();

		m_Window = IWindow::CreateWindow(settings.WindowWidth, settings.WindowHeight, settings.Title, settings.Api);
		m_Renderer = IRenderer::CreateRenderer(m_Window, settings.Api);
	}

	Application::~Application() {
		LAMBDA_AUTOTRACE();

		OnDestroy();
	}

	void Application::Run() {
		LAMBDA_AUTOTRACE();

		OnStart();

		auto previous_time = std::chrono::high_resolution_clock::now();
		
		while(!m_Window->GetClosingState()) {
			
			/*Calculate Times*/
			auto current_time = std::chrono::high_resolution_clock::now();
			std::chrono::duration<f64> difference = current_time - previous_time;
			previous_time = current_time;

			/*Update things that need to be updated*/
			OnUpdate();
			m_Window->Update();

			/*Full render cycle*/
			m_Renderer->PreRender();
			OnRender();
			m_Renderer->PostRender();
		}
	}
}