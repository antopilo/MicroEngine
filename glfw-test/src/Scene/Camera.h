#pragma once
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_clip_space.hpp>

namespace Engine {
	class Camera 
	{
	private:
		glm::mat4 m_Perspective = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);
		glm::vec2 m_Position;

	public:
		Camera();
		Camera(glm::vec2 position);

		glm::mat4 GetPerspective();
		 
	};
}