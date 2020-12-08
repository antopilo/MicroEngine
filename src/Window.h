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

namespace Engine {

	class Window 
	{
	private:
		int m_Width;
		int m_Height;
		std::string m_Title;

		GLFWwindow* instance;

	public:
		Window(int width, int height, std::string title);

		int Initialize();
		void Update();
		int Close();

		// TODO: Remove param until scene are dones
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);

		bool WindowShouldClose() {
			return glfwWindowShouldClose(instance);
		}
	};
}

