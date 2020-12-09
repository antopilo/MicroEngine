#pragma once
#include <GL/glew.h>
#include "Shader.h"

namespace Engine {
	class Texture;
	class Camera;

	struct Renderer2DData;
	
	class Renderer {
	public:
		static Shader* TextureShader;
		static void Init();

		static void Draw();

		static void BeginScene(glm::mat4 camera, glm::mat4 transform);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture);

		static void EndScene();

		static void Clear();

	private:
		static Renderer2DData s_Data;
	};
}








