#include "Input.h"
#include "../Application.h"

namespace Engine {
	// TODO: Key press, and release system.

	bool Input::IsKeyPressed(int keycode) 
	{
		auto window = (Application::Get()).GetWindow()->GetNative();

		int state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsKeyPress(int keycode)
	{
		auto window = (Application::Get()).GetWindow()->GetNative();
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS;
	}

	bool Input::IsKeyReleased(int keycode)
	{
		auto window = (Application::Get()).GetWindow()->GetNative();
		int state = glfwGetKey(window, keycode);
		return state == GLFW_RELEASE;
	}

	void Input::HideMouse() {
		auto window = (Application::Get()).GetWindow()->GetNative();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	// TODO: Rename
	bool Input::IsMouseHidden() {
		auto window = (Application::Get()).GetWindow()->GetNative();
		return glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
	}

	void Input::ShowMouse() {
		auto window = (Application::Get()).GetWindow()->GetNative();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

	bool Input::Init()
	{
		//auto window = Application::Get().GetWindow()->GetNative();
		//glfwSetKeyCallback(window, Input::HandleInputCallback);
		return false;
	}

	
}