#include "Camera.h"
#include <glm\gtx\quaternion.hpp>
#include "../Core/Input.h"
#include <GLFW\glfw3.h>
#include "../Core/Timestep.h"
#include "../Scene/World/Chunks/SubChunk.h"
#include "../Scene/World/Chunks/Chunk.h"
namespace Engine {
	Camera::Camera(CAMERA_TYPE type) {
		m_Type = PERSPECTIVE;
		
		Translation = glm::vec3(-10.0f, 0.0f, -10.0f);
		cameraDirection = glm::vec3(0, 0, 1);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		cameraUp = glm::cross(cameraDirection, cameraRight);
		Frustrum = new Frustum(GetTransform());
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
		if (Input::IsKeyPress(GLFW_KEY_UP))
			Speed += 0.1f;
		else if (Input::IsKeyPress(GLFW_KEY_DOWN))
			Speed -= 0.1f;
		if (Speed < 0)
			Speed = 0;
		// Keyboard
		if (!controlled) {

			return;
		}
			


		if (m_Type == CAMERA_TYPE::ORTHO) {
			if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
				Translation.x += Speed * ts;
			if (Input::IsKeyPressed(GLFW_KEY_LEFT))
				Translation.x -= Speed * ts;

			if (Input::IsKeyPressed(GLFW_KEY_UP))
				Translation.y += Speed * ts;
			if (Input::IsKeyPressed(GLFW_KEY_DOWN))
				Translation.y -= Speed * ts;
		}
		else {

			if (Input::IsKeyPressed(GLFW_KEY_D ))
				Translation -= cameraRight * (Speed * ts);
			if (Input::IsKeyPressed(GLFW_KEY_A))
				Translation += cameraRight * (Speed * ts);

			if (Input::IsKeyPressed(GLFW_KEY_W))
				Translation += cameraDirection * (Speed * ts);
			if (Input::IsKeyPressed(GLFW_KEY_S))
				Translation -= cameraDirection * (Speed * ts);
			if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
				Translation -= up * (Speed * ts);
			if (Input::IsKeyPressed(GLFW_KEY_SPACE))
				Translation += up * (Speed * ts);
		}


		delete Frustrum;
		Frustrum = new Frustum(GetPerspective() * GetTransform());

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
		m_Perspective = glm::perspectiveFov(glm::radians(Fov), 16.0f, 9.0f, 0.1f, 2000.0f);
		//}

		return m_Perspective;
	}

	glm::mat4 Camera::GetTransform() 
	{
		glm::mat4 tr = lookAt(Translation, Translation + cameraFront, cameraUp);
		return tr;

	}
	bool Camera::IsChunkVisible(SubChunk* chunk)
	{
		auto sub = chunk;
		auto parent = sub->GetParent();
		glm::vec2 parentPos = parent->GetPosition();
		glm::vec3 lowp = glm::vec3(parentPos.x * SubChunk::SIZE, sub->GetIndex() * SubChunk::SIZE, parentPos.y * SubChunk::SIZE);;
		glm::vec3 highp = glm::vec3((parentPos.x * SubChunk::SIZE) + SubChunk::SIZE, sub->GetIndex() * SubChunk::SIZE + SubChunk::SIZE, parentPos.y * SubChunk::SIZE + SubChunk::SIZE);
		return Frustrum->IsBoxVisible(lowp, highp);
	}

	bool Camera::IsPositionInFrustrum(glm::vec2 pos) {

		glm::vec3 low = glm::vec3(pos.x, 0, pos.y);
		glm::vec3 high = glm::vec3(pos.x + SubChunk::SIZE, Chunk::SUBCHUNK_COUNT * SubChunk::SIZE, pos.y + SubChunk::SIZE);
		return Frustrum->IsBoxVisible(low, high);
	}

}

