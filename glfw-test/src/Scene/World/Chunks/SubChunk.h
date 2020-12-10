#pragma once
#include "Chunk.h"

namespace Engine {
	class SubChunk {
		const static int SIZE = 32;
	private:
		int*** m_Blocks;
		int m_Index;
		Chunk* m_Parent;

		 std::vector<QuadVertex> m_Mesh;

	public:
		SubChunk(int idx, Chunk* chunk);

		void SetBlock(int x, int y, int z, int type);
		int GetBlock(int x, int y, int z);

		void Mesh();

		void Draw();

		~SubChunk();
	};
}