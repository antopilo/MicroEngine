#include "Application.h"

namespace Engine {

	Application* Application::s_Instance = nullptr;

	void Application::Init() {
		

		m_Window = new Engine::Window(1280, 720, "Hello world");
		
		//m_ImGuiLayer = new ImGuiLayer();
	}

	void Application::Run() {

		while (!m_Window->WindowShouldClose()) {
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			m_Window->Update(timestep);
			// TODO: Remove params
			m_Window->Draw(timestep);
		}
	}

	int Application::Close() {
		return m_Window->Close();
	}

	Application::Application()
	{
		s_Instance = this;
	}

	Application::~Application() {
		delete m_Window;
	}
	Window* Application::GetWindow()
	{
		return m_Window;
	}
}
