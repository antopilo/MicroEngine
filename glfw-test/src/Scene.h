#pragma once
#include "Rendering/Renderer.h"
#include "Core/Timestep.h"

namespace Engine {
	class Scene {

	private:
		int m_ViewportWidth = 0, m_ViewportHeight = 0;

	public:
		Scene();

		void Update(Timestep ts);

		void Draw();
	};
}
