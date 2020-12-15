#pragma once

#include "Scene.h"
#include "Entities/Entity.h"
#include "Entities/Components.h"
#include "Camera.h"
#include "../Rendering/Renderer.h"
#include "World/Generation/FastNoise.h"

#define FNL_IMPL
namespace Engine {

	Scene::Scene() {
		m_Camera = new Camera(ORTHO);
		m_World = new World(1337, m_Camera);
		m_World->Init();
	}

	Scene::~Scene() {
		delete m_Camera;
		delete m_World;
	}

	Engine::Entity Scene::CreateEntity() {
		return { m_EnttRegistry.create(), this };
	}

	Camera* Scene::GetCamera() {
		return m_Camera;
	}

	void Scene::Update(Timestep ts) {

		m_Camera->Update(ts);
		m_World->Update(ts);
		//auto group = m_EnttRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		//for (auto entity : group)
		//{
		//	auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
		//
		//	//Renderer::DrawQuad(transform.GetTransform(), sprite.Color);
		//}
	}

	FastNoiseLite noise;
	

	void Scene::Draw()
	{
		Renderer::BeginScene(m_Camera->GetPerspective(), m_Camera->GetTransform());
		m_World->Draw();
	}
}