#pragma once
#include <vector>

namespace Engine {
	class SubChunk;
	struct QuadVertex;

	static class ChunkMesher
	{
	public:
		static std::vector<QuadVertex> MeshSubChunk(SubChunk* subchunk);
	};
}