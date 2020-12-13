#include "Renderer.h"
#include <GLFW\glfw3.h>

#include <array>

#include "Debug.h"
#include "Texture.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"
#include "VertexArray.h"

#include "VertexBufferLayout.h"
namespace Engine {
	glm::mat4 Projection;
	static Engine::Renderer2DData* s_Data;

	std::vector<QuadVertex> Renderer::m_Vertices;
	uint32_t Renderer::m_Indices[MaxIndexCount];

	int Renderer::IndicesCount = 0;

	Shader* Renderer::TextureShader;
	VertexArray* Renderer::m_VertexArray;
	VertexBuffer* m_VertexBuffer;

	IndexBuffer* Renderer::m_IndexBuffer;

	void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar* msg, const void* data)
	{
		std::string _source;
		std::string _type;
		std::string _severity;

		switch (source) {
		case GL_DEBUG_SOURCE_API:
			_source = "API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			_source = "WINDOW SYSTEM";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			_source = "SHADER COMPILER";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			_source = "THIRD PARTY";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			_source = "APPLICATION";
			break;

		case GL_DEBUG_SOURCE_OTHER:
			_source = "UNKNOWN";
			break;

		default:
			_source = "UNKNOWN";
			break;
		}

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			_type = "ERROR";
			break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			_type = "DEPRECATED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			_type = "UDEFINED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
			_type = "PORTABILITY";
			break;

		case GL_DEBUG_TYPE_PERFORMANCE:
			_type = "PERFORMANCE";
			break;

		case GL_DEBUG_TYPE_OTHER:
			_type = "OTHER";
			break;

		case GL_DEBUG_TYPE_MARKER:
			_type = "MARKER";
			break;

		default:
			_type = "UNKNOWN";
			break;
		}

		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			_severity = "HIGH";
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			_severity = "MEDIUM";
			break;

		case GL_DEBUG_SEVERITY_LOW:
			_severity = "LOW";
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			_severity = "NOTIFICATION";
			break;

		default:
			_severity = "UNKNOWN";
			break;
		}

		printf("%d: %s of %s severity, raised from %s: %s\n",
			std::to_string(id).c_str(), _type.c_str(), _severity.c_str(), _source.c_str(), msg);
	}

	void Renderer::Init() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugMessageCallback, nullptr);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glDepthFunc(GL_LEQUAL);

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

		//m_VertexBuffer = new VertexBuffer(nullptr, sizeof(QuadVertex) * MaxVertexCount);

		//SetBufferLayout(m_VertexBuffer);
		CreateIndexBuffer();
		//m_IndexBuffer->Bind();
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

		Renderer::m_IndexBuffer->Bind();
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

