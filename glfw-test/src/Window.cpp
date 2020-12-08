#include "Window.h"
#include "Rendering/Debug.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw.h>
#include <imgui\imgui_impl_opengl3.h>

namespace Engine {
	Window::Window(int width, int height, std::string title) {
		m_Width = width;
		m_Height = height;

		m_Title = title;

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
		glfwTerminate();
		return 0;
	}

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	void Window::Update() {

	}

	void Window::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) {
		Renderer::Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
		if (true)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}


		ImGui::Render();
		//ImGui::ShowDemoWindow(&true);

		
		Renderer::Draw(va, ib, shader);

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
