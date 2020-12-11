#pragma once
#include "Chunk.h"

namespace Engine {
	class SubChunk {
		


	public:
		const static int SIZE = 8;
		SubChunk(int idx, Chunk* chunk);

		void SetBlock(int x, int y, int z, int type);
		int GetBlock(int x, int y, int z);

		void Mesh();

		void Draw();

		Chunk* GetParent();

		int GetIndex();

		~SubChunk();
	private:
		int m_Blocks[SIZE][SIZE][SIZE];
		int m_Index;
		Chunk* m_Parent;

		std::vector<QuadVertex> m_Mesh;
	};
}