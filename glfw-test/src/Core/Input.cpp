#include "Input.h"
#include "../Application.h"

namespace Engine {
	bool Input::IsKeyPressed(int keycode) 
	{
		auto window = (Application::Get()).GetWindow()->GetNative();

		int state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = Application::Get().GetWindow()->GetNative();
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	float Input::GetMouseX()
	{
		auto window = Application::Get().GetWindow()->GetNative();

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return (float)xpos;
	}

	float Input::GetMouseY()
	{
		auto window = Application::Get().GetWindow()->GetNative();

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return (float)ypos;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = Application::Get().GetWindow()->GetNative();

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };	
	}
}