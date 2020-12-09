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

	float positions[] = {
		-0.5f, -0.5f, /* text corrds */ 0.0f, 0.0f, /* color */	1.0f, 1.0f, 1.0f, 1.0f,   1.0f, /* textureID */ 0.0f,
		0.5f,  -0.5f, /* text corrds */ 1.0f, 0.0f, /* color */	1.0f, 1.0f, 1.0f, 1.0f,   1.0f, /* textureID */ 0.0f,
		0.5f,   0.5f, /* text corrds */ 1.0f, 1.0f, /* color */	1.0f, 1.0f, 1.0f, 1.0f,   1.0f, /* textureID */ 0.0f,
		-0.5f,  0.5f, /* text corrds */ 0.0f, 1.0f, /* color */	1.0f, 1.0f, 1.0f, 1.0f,   1.0f, /* textureID */ 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	Shader* Renderer::TextureShader;

	VertexArray* m_VertexArray;
	VertexBuffer* m_VertexBuffer;
	IndexBuffer* m_IndexBuffer;

	void Renderer::Init() {
		m_VertexArray = new VertexArray();
		
		VertexBuffer* m_VertexBuffer = new VertexBuffer(positions, 4 * sizeof(QuadVertex));

		VertexBufferLayout layout;
		layout.Push<QuadVertex>(1);
		
		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer =  new IndexBuffer(indices, 6);

		Shader* shader = new Shader("res/shaders/Basic.shader");
		shader->Bind();

		Texture* texture = new Texture("res/textures/test.png");
		texture->Bind();

		TextureShader = shader;
		//TextureShader->SetUniformMat4f("u_MVP", Projection);

		int samplers[2] = { 0, 1 };
		TextureShader->SetUniform1iv("u_Textures", 2, samplers);
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

	void Renderer::Draw() {

		int count = (sizeof(positions) / sizeof(*positions));

		m_VertexBuffer = new VertexBuffer(positions, 4 * 10 * sizeof(float));
		m_IndexBuffer = new IndexBuffer(indices, 6);

		TextureShader->Bind();
		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::Clear() {
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
}

