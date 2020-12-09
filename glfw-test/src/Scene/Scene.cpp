#pragma once

#include "Scene.h"
#include "Entities/Entity.h"

namespace Engine {

	Scene::Scene() {
		m_Camera = new Camera();
	}

	Engine::Entity Scene::CreateEntity() {
		return { m_EnttRegistry.create(), this };
	}

	void Scene::Update(Timestep ts) {

		auto group = m_EnttRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			//Renderer::DrawQuad(transform.GetTransform(), sprite.Color);
		}
	}
	void Scene::Draw()
	{
	}
}