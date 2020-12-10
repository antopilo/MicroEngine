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

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		VertexArray* QuadVertexArray;
		VertexBuffer* QuadVertexBuffer;
		//Shader* TextureShader;
		Texture* WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array< Texture , MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 QuadVertexPositions[4];

		//Renderer2D::Statistics Stats;
	};

	glm::mat4 Projection;
	static Engine::Renderer2DData* s_Data;

	std::vector<QuadVertex> Renderer::m_Vertices;
	int Renderer::IndicesCount = 0;

	Shader* Renderer::TextureShader;

	VertexArray* m_VertexArray;
	VertexBuffer* m_VertexBuffer;
	IndexBuffer* m_IndexBuffer;

	const size_t MaxQuadCount = 1000;
	const size_t MaxVertexCount = 1000 * 4;
	const size_t MaxIndexCount = 1000 * 6;

	void Renderer::Init() {

		// Texture shader
		Shader* shader = new Shader("res/shaders/Basic.shader");
		shader->Bind();

		TextureShader = shader;

		int samplers[2] = { 0, 1 };
		TextureShader->SetUniform1iv("u_Textures", 2, samplers);

		Texture* texture = new Texture("res/textures/test.png");
		texture->Bind();
		Texture* texture2 = new Texture("res/textures/test2.png");
		texture->Bind(1);

		// Vertex Array
		m_VertexArray = new VertexArray();

		// Buffer and attributes
		m_VertexBuffer = new VertexBuffer(nullptr, sizeof(QuadVertex) * MaxVertexCount);

		VertexBufferLayout layout;
		layout.Push<QuadVertex>(1);
		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

		uint32_t indices[MaxIndexCount];
		uint32_t offset = 0;
		for (auto i = 0; i < MaxIndexCount; i+= 6) {
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		// Index buffer
		m_IndexBuffer = new IndexBuffer(indices, MaxIndexCount);
	}

	void Renderer::BeginScene(glm::mat4 camera, glm::mat4 transform) {
		Clear();

		TextureShader->Bind();
		TextureShader->SetUniformMat4f("u_Projection", camera);
		TextureShader->SetUniformMat4f("u_View", transform);
	}


	void Renderer::EndScene() {

		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//for (auto i = 0; i < m_Vertices.size(); i++)
		//	delete m_Vertices[i];
	
		m_Vertices.clear();

		/* Swap front and back buffers */
		
	}

	void Renderer::CreateQuad(float x, float y, float textureID) {
		float size = 1.0f;

		m_Vertices.push_back(
			QuadVertex (
				glm::vec2(x, y),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec2(0.0f, 0.0f),
				textureID,
				1.0f
			)
		);

		m_Vertices.push_back(
			QuadVertex (
				glm::vec2(x + size, y),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec2(1.0f, 0.0f),
				textureID,
				1.0f
				)
		);

		m_Vertices.push_back(
			QuadVertex (
				glm::vec2(x + size, y + size),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec2(1.0f, 1.0f),
				textureID,
				1.0f
			)
		);

		m_Vertices.push_back(
			QuadVertex (
				glm::vec2(x, y + size),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec2(0.0f, 1.0f),
				textureID,
				1.0f
				)
		);

		printf("Created quad");

		IndicesCount += 6;
	}

	//std::vector<QuadVertex> Renderer::m_Vertices;

	void Renderer::Flush() {
		m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(QuadVertex));
		TextureShader->Bind();
		m_VertexBuffer->Bind();
		
		m_IndexBuffer->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, IndicesCount, GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::Clear() {
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
}

