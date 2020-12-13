#pragma once
#include <GL/glew.h>
#include "Shader.h"

namespace Engine {

	class Texture;
	class Camera;
	class VertexBuffer;
	const size_t MaxQuadCount = 80000;
	const size_t MaxVertexCount = MaxQuadCount * 4;
	const size_t MaxIndexCount = MaxQuadCount * 6;
	struct Renderer2DData;
	struct QuadVertex;
	class IndexBuffer;
	class VertexArray;
	class Renderer {
	public:
		static Shader* TextureShader;
		static IndexBuffer* m_IndexBuffer;
		static VertexArray* m_VertexArray;

		static void Init();
		static void SetBufferLayout(VertexBuffer* buffer);
		static void CreateIndexBuffer();
		static void Flush();

		static void BeginScene(glm::mat4 camera, glm::mat4 transform);
		static void CreateQuad(float x, float y, float textureID);
		static void EndScene();

		static void PushMesh(std::vector<QuadVertex> mesh);
		static void DrawCube(float x, float y, float z, float textureId);

		static void Clear();
		static int DrawCalls;
		//static unsigned int m_VertexArray;
	private:
		static Renderer2DData s_Data;
		static std::vector<QuadVertex> m_Vertices;
		static uint32_t m_Indices[MaxIndexCount];
		static int IndicesCount;
	};
}








