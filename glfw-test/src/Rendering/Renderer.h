#pragma once
#include <GL/glew.h>
#include "Shader.h"

namespace Engine {

	class Texture;
	class Camera;

	struct Renderer2DData;
	struct QuadVertex;
	class Renderer {
	public:
		static Shader* TextureShader;
		static void Init();
		static void Flush();

		static void BeginScene(glm::mat4 camera, glm::mat4 transform);
		static void CreateQuad(float x, float y, float textureID);
		static void EndScene();

		static void PushMesh(std::vector<QuadVertex> mesh);
		static void DrawCube(float x, float y, float z, float textureId);

		static void Clear();

	private:
		static Renderer2DData s_Data;
		static std::vector<QuadVertex> m_Vertices;
		static int IndicesCount;
	};
}








