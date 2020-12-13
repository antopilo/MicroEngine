#include "Renderer.h"
#include <GLFW\glfw3.h>

#include <array>

#include "Debug.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"

namespace Engine {
	glm::mat4 Projection;
	static Engine::Renderer2DData* s_Data;

	std::vector<QuadVertex> Renderer::m_Vertices;
	uint32_t Renderer::m_Indices[MaxIndexCount];

	int Renderer::IndicesCount = 0;

	Shader* Renderer::TextureShader;

	VertexArray* m_VertexArray;
	VertexBuffer* m_VertexBuffer;

	IndexBuffer* Renderer::m_IndexBuffer;

	void Renderer::Init() {
		//glEnable(GL_DEPTH_TEST);
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// Texture shader
		Shader* shader = new Shader("res/shaders/Basic.shader");
		shader->Bind();

		TextureShader = shader;

		int samplers[2] = { 0, 1 };
		TextureShader->SetUniform1iv("u_Textures", 2, samplers);
		Texture* texture = new Texture("res/textures/test.png");
		texture->Bind(0);
		Texture* texture2 = new Texture("res/textures/test2.png");
		texture->Bind(1);

		// Vertex Array
		m_VertexArray = new VertexArray();

		m_VertexBuffer = new VertexBuffer(nullptr, sizeof(QuadVertex) * MaxVertexCount);

		SetBufferLayout(m_VertexBuffer);
		CreateIndexBuffer();
		m_IndexBuffer->Bind();
		Renderer::TextureShader->Bind();
	}

	void Renderer::SetBufferLayout(VertexBuffer* buffer) {
		VertexBufferLayout layout;
		layout.Push<float>(3); // pos
		layout.Push<float>(3); // normal
		layout.Push<float>(4); // color
		layout.Push<float>(2); // texture coords
		layout.Push<float>(1); // texture id
		layout.Push<float>(1); // texture tiling
	
		m_VertexArray->AddBuffer(*buffer, layout);
	}

	void Renderer::CreateIndexBuffer() {
		uint32_t offset = 0;
		for (auto i = 0; i < MaxIndexCount; i += 6) {
			m_Indices[i + 0] = 0 + offset;
			m_Indices[i + 1] = 1 + offset;
			m_Indices[i + 2] = 2 + offset;

			m_Indices[i + 3] = 2 + offset;
			m_Indices[i + 4] = 3 + offset;
			m_Indices[i + 5] = 0 + offset;

			offset += 4;
		}
		m_IndexBuffer = new IndexBuffer(m_Indices, MaxIndexCount);
	}

	void Renderer::BeginScene(glm::mat4 camera, glm::mat4 transform) {
		Clear();
		DrawCalls = 0;
		TextureShader->Bind();
		TextureShader->SetUniformMat4f("u_Projection", camera);
		TextureShader->SetUniformMat4f("u_View", transform);
	}


	void Renderer::EndScene() {
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//for (auto i = 0; i < m_Vertices.size(); i++)
		//	delete m_Vertices[i];
		//Flush();
		//m_Vertices.clear();
		//IndicesCount = 0;
		/* Swap front and back buffers */
	}

	int Renderer::DrawCalls = 0;

	void Renderer::Flush() {
		//TextureShader->Bind();
		

		//TextureShader->Bind();
		//m_VertexBuffer->Bind();
		//
		//if (lastIndicesCount != IndicesCount)
		//	m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(QuadVertex));
		//
		//m_IndexBuffer->Bind();
		//lastIndicesCount = IndicesCount;
		//GLCall(glDrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::Clear() {
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
}

