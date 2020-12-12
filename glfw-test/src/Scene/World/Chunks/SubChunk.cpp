#include "SubChunk.h"
#include "ChunkMesher.h"
#include "../Rendering/Renderer.h"
#include "../Generation/FastNoise.h"
#include "../Rendering/VertexBuffer.h"
#include "../Rendering/Vertex.h"
#include "../Rendering/IndexBuffer.h"
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

		m_VertexBuffer = new VertexBuffer(nullptr, sizeof(QuadVertex) * MaxVertexCount);
		Renderer::SetBufferLayout(m_VertexBuffer);
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

		m_IndexCount = m_Mesh.size() * 1.5;

		m_VertexBuffer->Bind();
		m_VertexBuffer->SetData(m_Mesh.data(), m_Mesh.size() * sizeof(QuadVertex));
		m_Mesh.clear();

	}

	// Push the mesh to the renderer
	void SubChunk::Draw() {
		//Renderer::PushMesh(m_Mesh);

		Renderer::TextureShader->Bind();
		m_VertexBuffer->Bind();
		
		//if (lastIndicesCount != IndicesCount)
		//	m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size() * sizeof(QuadVertex));
		
		Renderer::m_IndexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
		Renderer::DrawCalls += 1;
	}

	Chunk* SubChunk::GetParent() {
		return m_Parent;
	}

	int SubChunk::GetIndex() {
		return m_Index;
	}
}