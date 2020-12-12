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
		static const uint32_t MaxQuads = 20000 * 4;
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
	uint32_t Renderer::m_Indices[MaxIndexCount];

	int Renderer::IndicesCount = 0;

	Shader* Renderer::TextureShader;

	VertexArray* m_VertexArray;
	VertexBuffer* m_VertexBuffer;

	IndexBuffer* Renderer::m_IndexBuffer;



	void Renderer::Init() {
		glEnable(GL_DEPTH_TEST);
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
		//TextureShader->SetUniform3f("lightPos", 1.2f, 1.0f, 2.0f);
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

	// this push a mesh to the renderer. Make sure this is for indexed mesh.
	// Should only have quads.
	void Renderer::PushMesh(std::vector<QuadVertex> mesh)
	{
		int indices = 0;
		for (auto i = 0; i < mesh.size(); i += 4) {
			m_Vertices.push_back(mesh[i]);
			m_Vertices.push_back(mesh[i + 1]);
			m_Vertices.push_back(mesh[i + 2]);
			m_Vertices.push_back(mesh[i + 3]);
			indices += 6;
		}
		IndicesCount += indices;
	}

	void Renderer::DrawCube(float x, float y, float z, float textureId) {
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
											  
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
											  
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
											   
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
											  
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f
		};

		for (auto i = 0; i < 24 * 8; i += 8) {

			glm::vec3 position = glm::vec3(vertices[i] + x, vertices[i + 1] + y, vertices[i + 2] + z);
			glm::vec2 texPos   = glm::vec2(vertices[i + 3], vertices[i + 4]);
			glm::vec3 normal   = glm::vec3(vertices[i + 5], vertices[i + 6], vertices[i + 7]);
			glm::vec4 color    = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			QuadVertex vert = QuadVertex(position, normal, color, texPos, textureId, 1.0f);
			m_Vertices.push_back(vert);
		}
		IndicesCount += 6 * 6;
	}

	void Renderer::CreateQuad(float x, float y, float textureID) {
		float size = 1.0f;

		m_Vertices.push_back(
			QuadVertex (
				glm::vec3(x, y, 0),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec2(0.0f, 0.0f),
				textureID,
				1.0f
			)
		);

		m_Vertices.push_back(
			QuadVertex (
				glm::vec3(x + size, y, 0),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec2(1.0f, 0.0f),
				textureID,
				1.0f
				)
		);

		m_Vertices.push_back(
			QuadVertex (
				glm::vec3(x + size, y + size, 0),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec2(1.0f, 1.0f),
				textureID,
				1.0f
			)
		);

		m_Vertices.push_back(
			QuadVertex (
				glm::vec3(x, y + size, 0),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
				glm::vec2(0.0f, 1.0f),
				textureID,
				1.0f
			)
		);
		IndicesCount += 6;
	}

	//std::vector<QuadVertex> Renderer::m_Vertices;

	int lastIndicesCount = 0;
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

