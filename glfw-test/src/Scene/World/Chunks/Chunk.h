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
		
		std::vector<SubChunk*> m_Subchunks;

		//std::array<QuadVertex, 999> m_Mesh;
		//std::array<int, 999> m_Indices;
	public:
		Chunk(glm::vec2 position);
		const static int SUBCHUNK_COUNT = 8;

		SubChunk& GetSubChunk(int idx);
		int GetBlock(int x, int y, int z);
		void SetBlock(int x, int y, int z, int type);

		void Mesh();

		void Render();

		Chunk* Left = nullptr;
		Chunk* Front = nullptr;
		Chunk* Right = nullptr;
		Chunk* Back = nullptr;

		glm::vec2 GetPosition();
		void CheckIfSurrounded();
		bool isMeshed = false;
		bool isGenerated = false;
		bool isSurrounded = false;

		~Chunk();
		void Draw();
	};
}