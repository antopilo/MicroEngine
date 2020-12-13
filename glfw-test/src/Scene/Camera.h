#pragma once
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_clip_space.hpp>
#include "../Core/Timestep.h"
#include "Frustrum.h"
namespace Engine {
	enum CAMERA_TYPE {
		ORTHO,
		PERSPECTIVE
	};
	class SubChunk;
	class Camera 
	{
	private:
		float mouseLastX = 0;
		float mouseLastY = 0;
		float Speed = 1.0f;
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		CAMERA_TYPE m_Type;

		bool controlled = true;

		glm::mat4 m_Perspective;
		bool firstMouse = true;

		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraDirection = glm::normalize(Translation - cameraTarget);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

		float Yaw = 0;
		float Pitch = 0;

	public:
		Frustum* Frustrum;
		float Fov = 90.0f;
		Camera(glm::vec2 position);
		
		Camera(CAMERA_TYPE type);

		void SetType(CAMERA_TYPE type);
		bool IsChunkVisible(SubChunk* chunk);
		void Update(Timestep ts);

		glm::vec3 GetTranslation();

		glm::vec3 GetDirection();

		glm::mat4 GetPerspective();
		glm::mat4 GetTransform();
	};
}