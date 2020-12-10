#pragma once
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <iostream>

#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexBufferLayout.h"
#include "Rendering/VertexArray.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/Renderer.h"
#include "Rendering/Texture.h"
#include "Core/Timestep.h"
namespace Engine {

	class Window 
	{
	private:
		int m_Width;
		int m_Height;
		std::string m_Title;
		Camera* m_Cam;
		GLFWwindow* instance;

	public:

		Window(int width, int height, std::string title);

		int Initialize();
		void Update();
		int Close();

		void Draw(Timestep ts);

		bool WindowShouldClose() {
			return glfwWindowShouldClose(instance);
		}

		GLFWwindow* GetNative();
	};
}

