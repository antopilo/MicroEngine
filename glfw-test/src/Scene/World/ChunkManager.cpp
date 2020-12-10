#include "ChunkManager.h"

void Engine::ChunkManager::LoadChunk(int x, int z)
{
	if (m_Chunks.find(glm::vec2(x, z)) != m_Chunks.end()) {
		// Already contains chunk!
		return;
	}

	glm::vec2 pos = glm::vec2(x, z);

	Chunk* newChunk = new Chunk(pos);

	m_Chunks.insert(pos, newChunk);
}
