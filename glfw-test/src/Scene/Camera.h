#pragma once
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include "../Core/Timestep.h"

namespace Engine {
	enum CAMERA_TYPE {
		ORTHO,
		PERSPECTIVE
	};

	class Camera 
	{
	private:
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		CAMERA_TYPE m_Type;

		glm::mat4 m_Perspective;

		glm::vec2 m_Position = { 0.0f, 0.0f };

	public:

		float Fov = 90.0f;
		Camera(glm::vec2 position);

		Camera(CAMERA_TYPE type);

		void SetType(CAMERA_TYPE type);

		void Update(Timestep ts);

		glm::vec3 GetTranslation();

		glm::mat4 GetPerspective();
		glm::mat4 GetTransform();
	};
}