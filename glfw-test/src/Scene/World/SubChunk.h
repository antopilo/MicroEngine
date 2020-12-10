#pragma once
#include "Chunk.h"

namespace Engine {
	class SubChunk {
		const static int SIZE = 32;
	private:
		int** m_Blocks[SIZE][SIZE];
		int m_Index;
		Chunk* m_Parent;
	public:
		SubChunk(int idx, Chunk* chunk);

		void SetBlock(int x, int y, int z, int type);
		int GetBlock(int x, int y, int z);

	};
}