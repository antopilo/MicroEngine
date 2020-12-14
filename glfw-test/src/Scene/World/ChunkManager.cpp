#include "ChunkManager.h"
#include "../../Core/Timestep.h"
#include "../Camera.h"
#include "Chunks/SubChunk.h"

namespace Engine {
	struct ChunkPos {
		int x;
		int y;

		bool operator<(const ChunkPos& comp) const {
			return std::tie(x, y) < std::tie(comp.x, comp.y);
		}

		bool operator==(const ChunkPos& o) const {
			return x == o.x && y == o.y;
		}

	};
	
	int RenderDistance = 2;

	std::map<ChunkPos, std::shared_ptr<Chunk>> ChunkManager::m_Chunks;
	Camera* ChunkManager::m_Camera;

	int ChunkManager::m_LoadedChunkCount = 0;

	void ChunkManager::SetCam(Camera* cam) {
		m_Camera = cam;
	}

	Camera* ChunkManager::GetCam() {
		return m_Camera;
	}

	void ChunkManager::Init() {
		//m_Chunks.reserve(2000);
		//LoadChunk(0, 0);
		//LoadChunk(1, 0);
		//LoadChunk(0, 1);
		//LoadChunk(1, 1);
	}


	bool ChunkManager::IsChunkLoaded(int x, int z) {
		if (m_Chunks.find(ChunkPos{ x, z }) != m_Chunks.end()) {
			return true;
		}
		return false;
	}

	void ChunkManager::LoadChunk(int x, int z)
	{
		m_LoadedChunkCount += 1;
		m_Chunks[ChunkPos{ x,z }] = std::shared_ptr<Chunk>(new Chunk(glm::vec2(x, z)));
		//newChunk->Draw();
	}

	void ChunkManager::Mesh() {
		int meshedCount = 0;
		for (auto c : m_Chunks)
			if (!c.second->isMeshed) {
				c.second->Mesh();
				meshedCount++;
			}
	}

	void ChunkManager::Draw()
	{
		for (auto chunk : m_Chunks)
			chunk.second->Draw();
	}

	void ChunkManager::Update(Timestep ts)
	{
			CheckForLoad();
			Mesh();
			CheckForUnload();
	}

	void ChunkManager::CheckForUnload() {
		glm::vec3 pos = m_Camera->GetTranslation();
		int camZ = pos.z / SubChunk::SIZE;
		int camX = pos.x / SubChunk::SIZE;
		for (auto chunk : m_Chunks) {
			glm::vec2 minus = chunk.second->GetPosition() - glm::vec2(camX, camZ);
			float dist = sqrt(minus.x * minus.x + minus.y * minus.y);
			if (dist > RenderDistance * 2) {
				chunk.second = nullptr;
				m_Chunks.erase(chunk.first);
				return;
			}
		}
	}

	void ChunkManager::CheckForLoad() {
		glm::vec3 pos = m_Camera->GetTranslation();
		int camZ = pos.z / SubChunk::SIZE;
		int camX = pos.x / SubChunk::SIZE;

		int numLoaded = 0;
		for(auto x = camX - RenderDistance; x < camX + RenderDistance; x++)
			for (auto z = camZ - RenderDistance; z < camZ + RenderDistance; z++) {
				if (numLoaded > 4)
					return;
				if (!IsChunkLoaded(x, z)) {
					LoadChunk(x, z);
					numLoaded++;
				}
			}
				
	}

	int ChunkManager::GetBlock(int x, int y, int z)
	{
		return 0;
	}
}

