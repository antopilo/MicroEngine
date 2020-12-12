#include "Window.h"
#include "Rendering/Debug.h"
#include "Scene/Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw.h>
#include <imgui\imgui_impl_opengl3.h>
#include <string>
#include "Scene/World/ChunkManager.h"

namespace Engine {
	Window::Window(int width, int height, std::string title) {
		m_Width = width;
		m_Height = height;
		m_Title = title;

		m_Scene = new Scene();

		if (Initialize() == -1) {
			std::cout << "ERROR: Failed to create window." << std::endl;
			return;
		}
	}

	int Window::Initialize() {
		/* Initialize the library */
		if (!glfwInit())
			return -1;

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		/* Create a windowed mode window and its OpenGL context */
		instance = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
		if (!instance)
		{
			glfwTerminate();
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(instance);

		std::cout << glGetString(GL_VERSION) << std::endl;

		// Glew init. after creating a valid context.
		if (glewInit() != GLEW_OK) {
			std::cout << "GLEW initialization failed!";
		}

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(instance, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		
	}

	int Window::Close() {
		delete m_Scene;

		glfwTerminate();
		return 0;
	}

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	void Window::Update(Timestep ts) {
		m_Scene->Update(ts);
	}

	void Window::Draw(Timestep ts) {
		
		
		// Draw imgui.
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Debugging!");                          // Create a window called "Hello, world!" and append into it.

			// CAMERA
			// position
			ImGui::Text("Camera:");

			ImGui::Text("Position:");
			std::string x = std::to_string((m_Scene->GetCamera()->GetTranslation()).x) + " "
			 + std::to_string((m_Scene->GetCamera()->GetTranslation()).y) + " "
			 + std::to_string((m_Scene->GetCamera()->GetTranslation()).z);
			ImGui::Text( x.c_str());

			// Direction
			ImGui::Text("Direction:");
			std::string d = std::to_string((m_Scene->GetCamera()->GetDirection()).x) + " "
				+ std::to_string((m_Scene->GetCamera()->GetDirection()).y) + " "
				+ std::to_string((m_Scene->GetCamera()->GetDirection()).z);
			ImGui::Text(d.c_str());

			// TYPE
			ImGui::Text("Type:");
			const char* items[] = { "Orthographic", "Perspective"};
			static int item = 0;
			ImGui::Combo("Type", &item, items, IM_ARRAYSIZE(items));

			//if (item == 0)
			//	m_Cam->SetType(ORTHO);
			//else if (item == 1)
			//	m_Cam->SetType(PERSPECTIVE);
				
			// FOV
			ImGui::SliderFloat("FOV", &m_Scene->GetCamera()->Fov, 1.0f, 120.0f);

			ImGui::Text("Chunks info:");
			ImGui::Text(std::to_string(ChunkManager::m_LoadedChunkCount).c_str());

			std::string drawcall = "Draw calls: " + std::to_string(Renderer::DrawCalls);
			ImGui::Text(drawcall.c_str());
			// Edit 1 float using a slider from 0.0f to 1.0f

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		ImGui::Render();
		
		m_Scene->Draw();
		Renderer::EndScene();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		/* Swap front and back buffers */
		glfwSwapBuffers(instance);
		/* Poll for and process events */
		glfwPollEvents();
	}
	GLFWwindow* Window::GetNative()
	{
		return instance;
	}
}
