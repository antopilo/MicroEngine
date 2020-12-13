#include "SubChunk.h"
#include "ChunkMesher.h"
#include "../Rendering/Renderer.h"
#include "../Generation/FastNoise.h"
#include "../Rendering/VertexBuffer.h"
#include "../Rendering/Vertex.h"
#include "../Rendering/IndexBuffer.h"
#include "../Rendering/VertexArray.h"	
#include "../Rendering/VertexBufferLayout.h"
#include "../Rendering/Vertex.h"
#include "../ChunkManager.h"
#include "../Scene/Camera.h"
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
					if((noise.GetNoise((float)(i + gx) * 0.25, (float)(k + gz) * 0.25) + 1.0f) / 2 * 255 > gy + j) {
					//if (noise.GetNoise((float)i + gx, (float)j + gy, (float)k + gz) > 0.5f) {
						m_Blocks[i][j][k] = (char)1;
						m_Count++;
					}
						
					else
						m_Blocks[i][j][k] = (char)0;
					//m_Blocks[i][j][k] = 1;
				}
			}
		}
		
		printf( ( std::to_string(sizeof(m_Blocks)) + std::string("\n") ).c_str() ) ;
		// Create buffer
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		//glBindBuffer(GL_VERTEX_ARRAY, VAO);
		unsigned int size = sizeof(QuadVertex) * 60000;

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

		Renderer::m_IndexBuffer->Bind();
		//Renderer::m_VertexArray->Bind();

		// pos vec3
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), 0);
		// normal vec3
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(sizeof(GL_FLOAT) * 3));
		// color vec4
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(sizeof(GL_FLOAT) * 6));
		// texture pos vec2
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(sizeof(GL_FLOAT) * 10));
		// texture float
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(sizeof(GL_FLOAT) * 12));
		// tiling float
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(sizeof(GL_FLOAT) * 13));

		glBindVertexArray(0);
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
		if (m_Count == 0)
			return;

		this->m_Mesh = ChunkMesher::MeshSubChunk(this);

		this->m_IndexCount = m_Mesh.size() * 1.5;
		unsigned int indices = m_IndexCount * sizeof(QuadVertex);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_Mesh.size() * sizeof(QuadVertex), m_Mesh.data());
		m_Mesh.clear();
		m_Mesh.shrink_to_fit();
	}

	// Push the mesh to the renderer 
	void SubChunk::Draw() {
		if (m_IndexCount == 0 || !ChunkManager::GetCam()->IsChunkVisible(this))
			return;

		Renderer::TextureShader->Bind();
		//Renderer::m_IndexBuffer->Bind();

		glBindVertexArray(VAO);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//Renderer::m_IndexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);

		Renderer::DrawCalls += 1;
	}

	Chunk* SubChunk::GetParent() {
		return m_Parent;
	}

	int SubChunk::GetIndex() {
		return m_Index;
	}
}