#include "Renderer.h"
#include "Debug.h"
#include <GLFW\glfw3.h>
#include "Texture.h"
#include <array>
#include "Vertex.h"

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

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
	static Renderer2DData* s_Data;





	Shader* Renderer::TextureShader;

	VertexArray* m_VertexArray;
	VertexBuffer* m_VertexBuffer;
	IndexBuffer* m_IndexBuffer;

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


		//float positions[] = {
		//	-1.5f, -0.5f,  /* text corrds */ 0.0f, 0.0f, /* color */	1.0f, 1.0f, 1.0f, 1.0f,   1.0f, /* textureID */ 1.0f,
		//	-0.5f,  -0.5f, /* text corrds */ 1.0f, 0.0f, /* color */	1.0f, 1.0f, 1.0f, 1.0f,   1.0f, /* textureID */ 1.0f,
		//	-0.5f,   0.5f, /* text corrds */ 1.0f, 1.0f, /* color */	1.0f, 0.0f, 1.0f, 1.0f,   1.0f, /* textureID */ 1.0f,
		//	-1.5f,   0.5f, /* text corrds */ 0.0f, 1.0f, /* color */	1.0f, 1.0f, 1.0f, 1.0f,   1.0f, /* textureID */ 1.0f,
		//
		//	0.5f,  -0.5f, /* text corrds */ 0.0f, 0.0f, /* color */	1.0f, 1.0f, 1.0f, 1.0f,   1.0f, /* textureID */ 0.0f,
		//	1.5f,  -0.5f, /* text corrds */ 1.0f, 0.0f, /* color */	1.0f, 1.0f, 1.0f, 1.0f,   1.0f, /* textureID */ 0.0f,
		//	1.5f,   0.5f, /* text corrds */ 1.0f, 1.0f, /* color */	1.0f, 1.0f, 1.0f, 1.0f,   1.0f, /* textureID */ 0.0f,
		//	0.5f,   0.5f, /* text corrds */ 0.0f, 1.0f, /* color */	1.0f, 1.0f, 1.0f, 1.0f,   1.0f, /* textureID */ 0.0f,
		//};

		// Vertex Array
		m_VertexArray = new VertexArray();

		// Buffer and attributes
		m_VertexBuffer = new VertexBuffer(nullptr, sizeof(QuadVertex) * 1000);

		VertexBufferLayout layout;
		layout.Push<QuadVertex>(1);
		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		// Index buffer
		m_IndexBuffer = new IndexBuffer(indices, 12);
	}

	void Renderer::BeginScene(glm::mat4 camera, glm::mat4 transform) {
		Clear();

		TextureShader->Bind();
		TextureShader->SetUniformMat4f("u_Projection", camera);
		TextureShader->SetUniformMat4f("u_View", transform);
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		//TextureShader->SetUniformMat4f("u_MVP", Projection);
	}

	void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture)
	{
		//TextureShader->SetUniformMat4f("u_MVP", Projection);
	}

	void Renderer::EndScene() {

		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		/* Swap front and back buffers */
		
	}

	std::array<QuadVertex, 4> CreateQuad(float x, float y, float textureID) {
		float size = 1.0f;

		QuadVertex v0;
		v0.Position = { x, y };
		v0.TexCoord = { 0.0f, 0.0f };
		v0.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		v0.TexIndex = { textureID };
		v0.TilingFactor = { 1.0f };

		QuadVertex v1;
		v1.Position = { x + size, y};
		v1.TexCoord = { 1.0f, 0.0f };
		v1.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		v1.TexIndex = { textureID };
		v1.TilingFactor = { 1.0f };

		QuadVertex v2;
		v2.Position = { x + size, y + size };
		v2.TexCoord = { 1.0f, 1.0f };
		v2.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		v2.TexIndex = { textureID };
		v2.TilingFactor = { 1.0f };

		QuadVertex v3;
		v3.Position = { x, y + size };
		v3.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		v3.TexCoord = { 0.0f, 1.0f };
		v3.TexIndex = { textureID };
		v3.TilingFactor = { 1.0f };
		 
		return { v0, v1, v2, v3 };
	}


	void Renderer::Draw() {

		
		

		auto q0 = CreateQuad(-1.5f, -0.5f, 1.0f);
		auto q1 = CreateQuad(0.5f,  -0.5f, 0.0f);

		QuadVertex vertices[8];
		memcpy(vertices, q0.data(), q0.size() * sizeof(QuadVertex));
		memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(QuadVertex));
		m_VertexBuffer->SetData(vertices, sizeof(vertices));

		TextureShader->Bind();
		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::Clear() {
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
}

