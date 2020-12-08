#pragma once
#include "Window.h"
#include "ImGui/ImGuiLayer.h"

namespace Engine {

	static class Application
	{
	private:
		bool m_Running = true;
		ImGuiLayer* m_ImGuiLayer;

	public:
		Engine::Window* window;

		void Init();
		void Run();
		int Close();
	};
}


