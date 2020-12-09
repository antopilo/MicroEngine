#pragma once

#include "../Scene/Scene.h"

namespace Engine {

	class Entity 
	{

	public:
		Entity(entt::entity entityHandle, Scene* scene);

		template<typename T>
		bool HasComponent() {
			return m_Scene->m_EnttRegistry.has<T>(m_EntityHandle);
		}
	private:
		entt::entity m_EntityHandle;
		Scene* m_Scene;
	};
}