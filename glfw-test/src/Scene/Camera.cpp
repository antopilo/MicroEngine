#include "Camera.h"
#include <glm\gtx\quaternion.hpp>
#include "../Core/Input.h"
#include <GLFW\glfw3.h>

namespace Engine {

	Camera::Camera(CAMERA_TYPE type) {
		m_Type = PERSPECTIVE;
		
		Translation = glm::vec3();
		cameraDirection = glm::vec3(0, 0, 1);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		cameraUp = glm::cross(cameraDirection, cameraRight);
	}

	void Camera::SetType(CAMERA_TYPE type)
	{
		m_Type = type;
	}

	void Camera::Update(Timestep ts) {
		float x = Input::GetMouseX();
		float y = Input::GetMouseY();

		if (Input::IsKeyPress(GLFW_KEY_ESCAPE)) {
			controlled = !controlled;

			if (!controlled)
				Input::ShowMouse();
			else
				Input::HideMouse();
			mouseLastX = x;
			mouseLastY = y;
		}

		// Keyboard
		if (!controlled) {

			return;
		}
			


		if (m_Type == CAMERA_TYPE::ORTHO) {
			if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
				Translation.x += 2.0f * ts;
			if (Input::IsKeyPressed(GLFW_KEY_LEFT))
				Translation.x -= 2.0f * ts;

			if (Input::IsKeyPressed(GLFW_KEY_UP))
				Translation.y += 2.0f * ts;
			if (Input::IsKeyPressed(GLFW_KEY_DOWN))
				Translation.y -= 2.0f * ts;
		}
		else {

			if (Input::IsKeyPressed(GLFW_KEY_D ))
				Translation -= cameraRight * (2.0f * ts);
			if (Input::IsKeyPressed(GLFW_KEY_A))
				Translation += cameraRight * (2.0f * ts);

			if (Input::IsKeyPressed(GLFW_KEY_W))
				Translation += cameraDirection * (2.0f * ts);
			if (Input::IsKeyPressed(GLFW_KEY_S))
				Translation -= cameraDirection * (2.0f * ts);
			if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
				Translation -= up * (2.0f * ts);
			if (Input::IsKeyPressed(GLFW_KEY_SPACE))
				Translation += up * (2.0f * ts);
		}


		if (firstMouse)
		{
			mouseLastX = x;
			mouseLastY = y;
			firstMouse = false;
		}
		// mouse
		

		float diffx = x - mouseLastX;
		float diffy = mouseLastY - y;
		mouseLastX = x;
		mouseLastY = y;

		const float sensitivity = 0.1f;
		diffx *= sensitivity;
		diffy *= sensitivity;

		Yaw += diffx;
		Pitch += diffy;

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;

		cameraDirection.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		cameraDirection.y = sin(glm::radians(Pitch));
		cameraDirection.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		cameraFront = glm::normalize(cameraDirection);
		cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	}

	glm::vec3 Camera::GetTranslation() {
		return Translation;
	}

	glm::vec3 Camera::GetDirection() {
		return cameraDirection;
	}

	glm::mat4 Camera::GetPerspective()
	{
		//if (m_Type == CAMERA_TYPE::ORTHO) {
		//	m_Perspective = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);
		//}
		//else if (m_Type == CAMERA_TYPE::PERSPECTIVE) {
		m_Perspective = glm::perspectiveFov(glm::radians(Fov), 16.0f, 9.0f, 0.1f, 200.0f);
		//}

		return m_Perspective;
	}

	glm::mat4 Camera::GetTransform() 
	{
		glm::mat4 tr = lookAt(Translation, Translation + cameraFront, cameraUp);
		return tr;

	}
}

