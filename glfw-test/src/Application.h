#pragma once
#include "Window.h"
#include "ImGui/ImGuiLayer.h"
#include "Scene/Scene.h"

namespace Engine {

	class Application
	{
	private:
		bool m_Running = true;

		static Application* s_Instance;
		
		Engine::Window* m_Window;

		ImGuiLayer* m_ImGuiLayer;
		Scene* m_Scene;

	public:

		void Init();
		void Run();
		int Close();

		//Scene* GetScene();

		// Todo: serialization of scene.
		//void LoadScene(const std::string& path);

		static Application& Get() { return *s_Instance; }

		Window* GetWindow();
	};
}


