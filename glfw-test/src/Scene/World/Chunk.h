#pragma once
#include "glm/glm.hpp"
#include <array>
#include "SubChunk.h"
namespace Engine {
	class Chunk
	{
	private:
		glm::vec2 m_Position; 
		std::array<SubChunk, 16> m_SubChunks;

		Chunk* m_Left;
		Chunk* m_Right;
		Chunk* m_Front;
		Chunk* m_Back;

	public:
		Chunk(glm::vec2 position);

		int GetBlock(int x, int y, int z);
		void SetBlock(int x, int y, int z, int type);

		void Render();
	};
}