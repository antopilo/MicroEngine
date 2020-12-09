#pragma once
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include "../Core/Timestep.h"

namespace Engine {
	class Camera 
	{
	private:
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };


		glm::mat4 m_Perspective = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);
		glm::vec2 m_Position = { 0.0f, 0.0f };

	public:
		Camera() {

		}
		Camera(glm::vec2 position);

		void Update(Timestep ts);

		glm::mat4 GetPerspective();
		glm::mat4 GetTransform();
	};
}