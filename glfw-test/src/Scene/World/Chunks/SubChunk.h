#pragma once
#include "Chunk.h"
#include <memory>
namespace Engine {
	class VertexBuffer;

	class SubChunk {
	public:
		const static int SIZE = 64;
		SubChunk(int idx, Chunk* chunk);

		void Generate();

		void SetBlock(int x, int y, int z, int type);
		int GetBlock(int x, int y, int z);

		void Mesh();

		void UpdateBuffer();

		void Draw();

		Chunk* GetParent();

		int GetIndex();

		~SubChunk();
		bool MeshChanged = false;
	private:
		char m_Blocks[SIZE][SIZE][SIZE];
		int m_Index;
		Chunk* m_Parent;
		int m_Count = 0;
		unsigned int VBO;
		unsigned int VAO;
		std::vector<QuadVertex>* m_Mesh;
		int m_IndexCount = 0;
		
	};
}