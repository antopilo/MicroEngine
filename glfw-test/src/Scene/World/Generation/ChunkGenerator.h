#pragma once
#include "../Chunks/Chunk.h"

namespace Engine {

	class ChunkGenerator
	{
	public:
		static void GenerateHeightPass(Chunk* chunk);

		static void GenerateDecoration(Chunk* chunk);


	};
}
