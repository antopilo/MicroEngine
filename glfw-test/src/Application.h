#pragma once
#include "Window.h"
#include "ImGui/ImGuiLayer.h"

namespace Engine {

	class Application
	{
	private:
		bool m_Running = true;
		ImGuiLayer* m_ImGuiLayer;
		static Application* s_Instance;

	public:
		Engine::Window* m_Window;

		void Init();
		void Run();
		int Close();

		static Application& Get() { return *s_Instance; }

		Window* GetWindow();
	};
}


