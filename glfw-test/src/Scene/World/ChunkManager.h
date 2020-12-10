#pragma once
#include <unordered_map>
#include "Chunk.h"

namespace Engine {
	class ChunkManager
	{
		static std::unordered_map<glm::vec2, Chunk*> m_Chunks;

		static void LoadChunk(int x, int z);

		static void Render();

		static void Update();

	};
}