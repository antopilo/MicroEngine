#pragma once
#include "../Rendering/Renderer.h"
#include "../Core/Timestep.h"
#include "entt/entt.hpp"
#include "Entities/Components.h"
#include "Camera.h"

namespace Engine
{
	class Entity;

	class Scene {

	private:
		int m_ViewportWidth = 0;
		int m_ViewportHeight = 0;
		entt::registry m_EnttRegistry;
		Camera* m_Camera;

	public:
		Scene();

		void Update(Timestep ts);

		void Draw();

		Entity CreateEntity();

		void DestroyEntity(entt::entity);

		Entity GetEntity(entt::entity);

		friend class Entity;
	};
}
