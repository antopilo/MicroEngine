#include "Application.h"

namespace Engine {

	Application* Application::s_Instance = nullptr;

	void Application::Init() {
		
		m_Window = new Engine::Window(1280, 720, "Hello world");
		Renderer::Init();
		//m_ImGuiLayer = new ImGuiLayer();
	}

	void Application::Run() {

		while (!m_Window->WindowShouldClose()) {
			m_Window->Update();


			// TODO: Remove params
			m_Window->Draw();
		}
	}

	int Application::Close() {
		return m_Window->Close();
	}

	Window* Application::GetWindow()
	{
		return m_Window;
	}
}
