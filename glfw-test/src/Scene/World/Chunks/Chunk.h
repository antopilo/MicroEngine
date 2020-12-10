#pragma once
#include "glm/glm.hpp"
#include <array>
#include "../Rendering/Vertex.h"
#include <vector>

namespace Engine {
	class SubChunk;

	class Chunk
	{
	private:
		glm::vec2 m_Position; 
		//std::array<SubChunk, 16> m_SubChunks;

		Chunk* m_Left;
		Chunk* m_Right;
		Chunk* m_Front;
		Chunk* m_Back;
		
		std::vector<SubChunk*> m_Subchunks;

		const static int SUBCHUNK_COUNT = 16;

		//std::array<QuadVertex, 999> m_Mesh;
		//std::array<int, 999> m_Indices;
	public:
		Chunk(glm::vec2 position);

		int GetBlock(int x, int y, int z);
		void SetBlock(int x, int y, int z, int type);

		void Mesh();

		void Render();

		~Chunk();
		void Draw();
	};
}