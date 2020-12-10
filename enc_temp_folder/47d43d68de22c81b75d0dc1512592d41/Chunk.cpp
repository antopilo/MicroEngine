#include "Chunk.h"
#include "SubChunk.h"

namespace Engine {
	Chunk::Chunk(glm::vec2 position)
	{
		m_Position = position;

		for (int i = 0; i < SUBCHUNK_COUNT; i++) {
			SubChunk* newSc = new SubChunk(i, this);
			m_Subchunks.push_back(newSc);

		}
	}

	Chunk::~Chunk() {
		for (int i = 0; i < SUBCHUNK_COUNT; i++) {
			delete m_Subchunks[i];
			m_Subchunks.clear();
		}
	}
}

