#include "SubChunk.h"
#include "ChunkMesher.h"
#include "../Rendering/Renderer.h"
#include "../Generation/FastNoise.h"

namespace Engine {
	SubChunk::SubChunk(int idx, Chunk* chunk)
	{
		// Setup ref
		m_Index = idx;
		m_Parent = chunk;

		FastNoiseLite noise;

		float gx = chunk->GetPosition().x * SIZE;
		float gy = m_Index * SIZE;
		float gz = chunk->GetPosition().y * SIZE;
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				for (int k = 0; k < SIZE; k++) {
					if(noise.GetNoise((float)i + gx, (float)j + gy, (float)k + gz) > 0.5f)
						m_Blocks[i][j][k] = 1;
					else
						m_Blocks[i][j][k] = 0;
				}
			}
		}

	}

	SubChunk::~SubChunk() { // Delete the blocks
		for (int i = 0; i < SIZE; ++i) {
			for (int j = 0; j < SIZE; ++j) {
				delete[] m_Blocks[i][j];
			}
			delete[] m_Blocks[i];
		}
		delete[] m_Blocks;
	}


	int SubChunk::GetBlock(int x, int y, int z)
	{
		return m_Blocks[x][y][z];
	}

	// Creates a mesh with the blocks inside the chunk.
	void SubChunk::Mesh() {
		m_Mesh = ChunkMesher::MeshSubChunk(this);
	}

	// Push the mesh to the renderer
	void SubChunk::Draw() {
		Renderer::PushMesh(m_Mesh);
	}

	Chunk* SubChunk::GetParent() {
		return m_Parent;
	}

	int SubChunk::GetIndex() {
		return m_Index;
	}
}