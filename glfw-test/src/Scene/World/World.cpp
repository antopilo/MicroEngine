#include "World.h"
#include "ChunkManager.h"
#include "../Scene/Camera.h"
namespace Engine {


	World::World(int seed, Camera* camera) {
		m_Seed = seed;
		m_Camera = camera;
		ChunkManager::SetCam(camera);
	}

	World::~World() {
	}

	void World::Init(){
		
	}

	void World::Update(Timestep ts) {
		ChunkManager::Update(ts);
	}

	void World::Draw() {
		ChunkManager::Draw();
	}
}