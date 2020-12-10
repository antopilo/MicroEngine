#include "SubChunk.h"
#include "ChunkMesher.h"
#include "../Rendering/Renderer.h"

namespace Engine {
	SubChunk::SubChunk(int idx, Chunk* chunk)
	{
		// Setup ref
		m_Index = idx;
		m_Parent = chunk;

		// Fill array with air.
		m_Blocks = new int** [SIZE];
		for (int i = 0; i < SIZE; i++) {
			m_Blocks[i] = new int* [SIZE];
			for (int j = 0; j < SIZE; j++) {
				m_Blocks[i][j] = new int[SIZE];
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


	// Creates a mesh with the blocks inside the chunk.
	void SubChunk::Mesh() {
		m_Mesh = ChunkMesher::MeshSubChunk(this);
	}

	// Push the mesh to the renderer
	void SubChunk::Draw() {
		Renderer::PushMesh(m_Mesh);
	}

}