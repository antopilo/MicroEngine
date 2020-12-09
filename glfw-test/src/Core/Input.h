#pragma once
#include <utility>
namespace Engine {
	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);

		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();

		Input* Get() { return s_Instance; }

	private:
			static Input* s_Instance;
	};
}