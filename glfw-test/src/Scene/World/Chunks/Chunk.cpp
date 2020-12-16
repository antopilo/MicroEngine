#include "Chunk.h"
#include "SubChunk.h"

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

