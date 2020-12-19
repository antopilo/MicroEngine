#include "ChunkManager.h"
#include "../../Core/Timestep.h"
#include "../Camera.h"

#include "Generation/ChunkGenerator.h"
#include "Chunks/Chunk.h"
#include "Chunks/SubChunk.h"
#include <thread>
#include "Generation/FeatureGenerator.h"
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
	
	int RenderDistance = 7;

	std::map<ChunkPos, Chunk*> ChunkManager::m_Chunks;// = new std::map<ChunkPos, std::shared_ptr<Chunk>>();
	std::vector<Chunk*> ChunkManager::m_GenerateChunk;
	Camera* ChunkManager::m_Camera;

	int ChunkManager::m_LoadedChunkCount = 0;

	void ChunkManager::SetCam(Camera* cam) {
		m_Camera = cam;
		FeatureGenerator::SetCam(cam);
	}

	Camera* ChunkManager::GetCam() {
		return m_Camera;
	}



	void ChunkManager::Init() {
		
		//LoadChunk(0, 0);
		//LoadChunk(1, 0);
		//LoadChunk(0, 1);
		//LoadChunk(1, 1);
		FeatureGenerator::Init();
		//std::thread* tread  = new std::thread(ThreadedUpdate);
	}

	bool ChunkManager::IsChunkLoaded(int x, int z) {
		if (m_Chunks.find(ChunkPos{ x, z }) != m_Chunks.end()) {
			return true;
		}
		return false;
	}

	Chunk* ChunkManager::GetChunk(int x, int z) {
		return m_Chunks[ChunkPos{ x, z }];
	}

	void ChunkManager::CheckSurrounded() {
		for (auto c : m_Chunks) {
			if (!c.second->isSurrounded)
				c.second->CheckIfSurrounded();
		}
	}

	void ChunkManager::LoadChunk(int x, int z)
	{
		m_LoadedChunkCount += 1;
		m_Chunks[ChunkPos{ x, z }] = new Chunk(glm::vec2(x, z));
		m_GenerateChunk.push_back(m_Chunks[ChunkPos{ x, z }]);

		//newChunk->Draw();
	}

	void ChunkManager::Generate() {
		for (auto c : m_GenerateChunk) {
			ChunkGenerator::GenerateHeightPass(c);
		}
		m_GenerateChunk.clear();
		m_GenerateChunk.shrink_to_fit();
	}

	void ChunkManager::UpdateBuffers() {
		for (auto c : m_Chunks) {
			if (!c.second->isMeshed && c.second->isSurrounded && c.second->isGenerated && c.second->MeshChanged) {
				c.second->UpdateBuffers();
			}
		}
	}

	void ChunkManager::Mesh() {
		int meshedCount = 0;
		for (auto c : m_Chunks) {
			if (meshedCount > 8)
				return;
			if (!c.second->isMeshed && c.second->isSurrounded && c.second->isGenerated) {
				c.second->Mesh();
				meshedCount++;
			}
		}
	}

	void ChunkManager::EndGenerate() {
		int decorated = 0;
		for (auto c : m_Chunks) {
			if (decorated > 8)
				return;

			if (!c.second->isMeshed && c.second->isSurrounded && !c.second->isGenerated) {
				ChunkGenerator::GenerateDecoration(c.second);
				decorated++;
			}
		}
	}

	void ChunkManager::Draw()
	{
		for (auto chunk : m_Chunks)
			chunk.second->Draw();
	}

	void ChunkManager::Update(Timestep ts)
	{
		FeatureGenerator::CheckLoadStructure();
		//FeatureGenerator::CheckLoadStructure();
		CheckForLoad();
		Generate();
		CheckSurrounded();
		EndGenerate();
		Mesh();
		UpdateBuffers();
		CheckForUnload();
			
	}

	void ChunkManager::ThreadedUpdate() {
		while (true) {
			CheckForLoad();
			Generate();
			CheckSurrounded();
			EndGenerate();
			Mesh();
			CheckForUnload();
		}
	}

	void ChunkManager::CheckForUnload() {
		glm::vec3 pos = m_Camera->GetTranslation();
		int camZ = pos.z / SubChunk::SIZE;
		int camX = pos.x / SubChunk::SIZE;
		std::map<ChunkPos, Chunk*>::iterator itr = m_Chunks.begin();
		while (itr != m_Chunks.end()) {
			glm::vec2 minus = itr->second->GetPosition() - glm::vec2(camX, camZ);

			float dist = sqrt(minus.x * minus.x + minus.y * minus.y);

			if (dist > RenderDistance) {
				delete (itr)->second;
				itr = m_Chunks.erase(itr);
			}
			else {
				++itr;
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

				auto minus = glm::vec2(camX, camZ) - glm::vec2(x, z);
				float dist = sqrt(minus.x * minus.x + minus.y * minus.y);
				if (numLoaded > 4)
					return;

				if (dist < RenderDistance && !IsChunkLoaded(x, z)) {
					LoadChunk(x, z);
					numLoaded++;
				}
			}
				
	}

	int ChunkManager::GetBlock(int gx, int gy, int gz)
	{
		// Chunk position
		int cx = gx / SubChunk::SIZE;
		int cz = gz / SubChunk::SIZE;

		// Local position
		int lx = gx - (cx * SubChunk::SIZE);
		int ly = gy;
		int lz = gz - (cz * SubChunk::SIZE);

		if (ly > Chunk::SUBCHUNK_COUNT * SubChunk::SIZE - 1 || ly < 0)
			return 0;

		if (gx < 0)
		{
			if (gx % SubChunk::SIZE != 0)
				cx = gx / SubChunk::SIZE - 1;

			lx = gx - (SubChunk::SIZE * cx);
		}
		if (gz < 0)
		{
			if (gz % SubChunk::SIZE != 0)
				cz = gz / SubChunk::SIZE - 1;

			lz = gz - (SubChunk::SIZE * cz);
		}

		//Renderer.AddDebugBox(new DebugBox(new Vector3(gx + 0.5f, gy + 0.5f, gz + 0.5f), Color.Green));
		ChunkPos cv = ChunkPos{ cx, cz };
		if (m_Chunks.find(cv) != m_Chunks.end())
		{
			return m_Chunks[cv]->GetBlock(lx, ly, lz);
		}
		return 0;
	}
}

