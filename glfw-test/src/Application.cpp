#include "Application.h"

namespace Engine {

	Application* Application::s_Instance = nullptr;

	void Application::Init() {
		m_Window = new Engine::Window(1280, 720, "Hello world");

		//m_ImGuiLayer = new ImGuiLayer();
	}

	void Application::Run() {
		float positions[] = {
			-0.5f, -0.5f, /* text corrds */ 0.0f, 0.0f,
			0.5f,  -0.5f, /* text corrds */ 1.0f, 0.0f,
			0.5f,   0.5f, /* text corrds */ 1.0f, 1.0f,
			-0.5f,  0.5f, /* text corrds */ 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);
		IndexBuffer ib(indices, 6);

		glm::mat4 proj = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);

		Shader shader = Shader("res/shaders/Basic.shader");
		shader.Bind();

		Texture texture = Texture("res/textures/test.png");
		texture.Bind();

		shader.SetUniform1i("u_Texture", 0);
		shader.SetUniformMat4f("u_MVP", proj);

		while (!m_Window->WindowShouldClose()) {
			m_Window->Update();

			// TODO: Remove params
			m_Window->Draw(va, ib, shader);
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
