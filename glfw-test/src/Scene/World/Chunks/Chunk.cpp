#include "Chunk.h"
#include "SubChunk.h"
#include "../ChunkManager.h"

namespace Engine {
	Chunk::Chunk(glm::vec2 position)
	{
		m_Position = glm::vec2(position.x, position.y );
		m_Subchunks.reserve(SUBCHUNK_COUNT);

		for (int i = 0; i < SUBCHUNK_COUNT; i++) {
			m_Subchunks.push_back( new SubChunk(i, this ));
		}
		
	}

	void Chunk::Mesh() {
		for (int i = 0; i < SUBCHUNK_COUNT; i++) {
			m_Subchunks[i]->Mesh();
		}
		isMeshed = true;
	}

	SubChunk& Chunk::GetSubChunk(int idx)
	{
		return *m_Subchunks[idx];
	}



	void Chunk::CheckIfSurrounded() {
		bool leftIsLoaded  = ChunkManager::IsChunkLoaded(m_Position.x - 1, m_Position.y);
		bool rightIsLoaded = ChunkManager::IsChunkLoaded(m_Position.x + 1, m_Position.y);
		bool frontIsLoaded = ChunkManager::IsChunkLoaded(m_Position.x,     m_Position.y + 1);
		bool backIsLoaded  = ChunkManager::IsChunkLoaded(m_Position.x,     m_Position.y - 1);

		if (!leftIsLoaded || !rightIsLoaded || !frontIsLoaded || !backIsLoaded)
			isSurrounded = false;

		if (leftIsLoaded)
			Left = ChunkManager::GetChunk(m_Position.x - 1, m_Position.y);
		else
			Left = nullptr;

		if (rightIsLoaded)
			Right = ChunkManager::GetChunk(m_Position.x + 1, m_Position.y);
		else
			Right = nullptr;

		if (frontIsLoaded)
			Front = ChunkManager::GetChunk(m_Position.x, m_Position.y + 1);
		else
			Front = nullptr;

		if (backIsLoaded)
			Back = ChunkManager::GetChunk(m_Position.x , m_Position.y - 1);
		else
			Back = nullptr;

		isSurrounded = leftIsLoaded && rightIsLoaded && frontIsLoaded && backIsLoaded;
	}	   
		
	Chunk::~Chunk() {
		for (int i = 0; i < SUBCHUNK_COUNT; i++) {
			delete m_Subchunks[i];
			
		}
		m_Subchunks.clear();
	}

	void Chunk::Draw() {
		for (auto& subChunk : m_Subchunks) {

			subChunk->Draw();

		}
	}

	void Chunk::SetBlock(int x, int y, int z, int type) {
		int maxY = SUBCHUNK_COUNT * SubChunk::SIZE;
		int idx = (y / SubChunk::SIZE);
		int localY = y - (idx * SubChunk::SIZE);
		if (this == nullptr)
			return;
		if (x < 0) {
			Left->SetBlock(x + SubChunk::SIZE, y, z, type);
			return;
		}
		else if (x > SubChunk::SIZE) {
			Right->SetBlock(x - SubChunk::SIZE, y, z, type);
			return;
		}
		else if (z < 0) {
			Back->SetBlock(x, y, z + SubChunk::SIZE, type);
			return;
		}
		else if (z > SubChunk::SIZE) {
			Front->SetBlock(x, y, z - SubChunk::SIZE, type);
			return;
		}
		m_Subchunks[idx]->SetBlock(x, localY, z, type);
	}

	int Chunk::GetBlock(int x, int y, int z)
	{
		int maxY = SUBCHUNK_COUNT * SubChunk::SIZE;
		int idx = (y / SubChunk::SIZE);
		int localY = y - (idx * SubChunk::SIZE);

		return m_Subchunks[idx]->GetBlock(x, localY, z);
	}

	glm::vec2 Chunk::GetPosition() {
		return m_Position;
	}
}

