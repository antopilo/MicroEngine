#include "Camera.h"
#include <glm\gtx\quaternion.hpp>
#include "../Core/Input.h"
#include <GLFW\glfw3.h>

namespace Engine {




	Camera::Camera(CAMERA_TYPE type) {
		m_Type = type;
	}

	void Camera::SetType(CAMERA_TYPE type)
	{
		m_Type = type;
	}

	void Camera::Update(Timestep ts) {
		
		if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
			Translation.x += 2.0f * ts;
		if (Input::IsKeyPressed(GLFW_KEY_LEFT))
			Translation.x -= 2.0f * ts;
		if (Input::IsKeyPressed(GLFW_KEY_UP))
			Translation.y += 2.0f * ts;
		if (Input::IsKeyPressed(GLFW_KEY_DOWN))
			Translation.y -= 2.0f * ts;
		if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
			Translation.z += 2.0f * ts;
		if (Input::IsKeyPressed(GLFW_KEY_SPACE))
			Translation.z -= 2.0f * ts;
	}

	glm::vec3 Camera::GetTranslation() {
		return Translation;
	}

	glm::mat4 Camera::GetPerspective()
	{
		if (m_Type == CAMERA_TYPE::ORTHO) {
			m_Perspective = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);
		}
		else if (m_Type == CAMERA_TYPE::PERSPECTIVE) {
			m_Perspective = glm::perspectiveFov(glm::radians(Fov), 16.0f, 9.0f, 0.0f, 1000.0f);
		}

		return m_Perspective;
	}

	glm::mat4 Camera::GetTransform() {
		glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
		return glm::scale(glm::mat4(1.0f), Scale) * rotation * glm::translate(glm::mat4(1.0f), Translation);
	}
}

