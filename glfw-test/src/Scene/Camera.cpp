#include "Camera.h"
#include <glm\gtx\quaternion.hpp>
#include "../Core/Input.h"
#include <GLFW\glfw3.h>

namespace Engine {

	void Camera::Update(Timestep ts) {
		
		if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
			Translation.x += 2.0f * ts;
		if (Input::IsKeyPressed(GLFW_KEY_LEFT))
			Translation.x -= 2.0f * ts;
		if (Input::IsKeyPressed(GLFW_KEY_UP))
			Translation.y += 2.0f * ts;
		if (Input::IsKeyPressed(GLFW_KEY_DOWN))
			Translation.y -= 2.0f * ts;
	}

	glm::mat4 Camera::GetPerspective()
	{
		return m_Perspective;
	}

	glm::mat4 Camera::GetTransform() {
		glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
		return glm::scale(glm::mat4(1.0f), Scale) * rotation * glm::translate(glm::mat4(1.0f), Translation);
	}
}

