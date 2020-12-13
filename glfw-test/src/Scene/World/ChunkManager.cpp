#include "ChunkManager.h"
#include "../../Core/Timestep.h"
#include "../Camera.h"
#include "Chunks/SubChunk.h"

namespace Engine {

	std::unordered_map<int, std::unordered_map<int, Chunk*>> ChunkManager::m_Chunks;
	Camera* ChunkManager::m_Camera;

	int ChunkManager::m_LoadedChunkCount = 0;

	void ChunkManager::SetCam(Camera* cam) {
		m_Camera = cam;
	}

	void ChunkManager::LoadChunk(int x, int z)
	{
		if (m_Chunks.find(x) != m_Chunks.end()) {
			// Already contains chunk!
			std::unordered_map<int, Chunk* > test = m_Chunks[x];
			if (test.find(z) != test.end()) {
				printf("Tried to load already loaded chunks \n");
				return;
			}
				
		}
		
		glm::vec2 pos = glm::vec2(x, z);
		
		Chunk* newChunk = new Chunk(pos);

		m_LoadedChunkCount += 1;
		m_Chunks[x][z] = newChunk;
		//newChunk->Draw();
	}

	void ChunkManager::Draw()
	{
		for (auto x : m_Chunks)
			for (auto chunk : x.second)
				chunk.second->Draw();
	}

	void ChunkManager::Update(Timestep ts)
	{
		//CheckForLoad();
		LoadChunk(1, 1);
	}

	void ChunkManager::CheckForLoad() {
		glm::vec3 pos = m_Camera->GetTranslation();
		int camZ = pos.z / SubChunk::SIZE;
		int camX = pos.x / SubChunk::SIZE;
		LoadChunk(camX, camZ);
	}

	int ChunkManager::GetBlock(int x, int y, int z)
	{
		return 0;
	}
}

