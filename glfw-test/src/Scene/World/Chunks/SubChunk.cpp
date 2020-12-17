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
#include "../Blocks.h"
namespace Engine {
	SubChunk::SubChunk(int idx, Chunk* chunk)
	{
		// Setup ref
		m_Index = idx;
		m_Parent = chunk;
		memset(&m_Blocks, 0, SIZE * SIZE * SIZE);
		//m_Blocks = new char * *[SIZE];
		//for (int i = 0; i < SIZE; i++) {
		//	m_Blocks[i] = new char* [SIZE];
		//	for (int j = 0; j < SIZE; j++) {
		//		m_Blocks[i][j] = new char[SIZE];
		//	} 
		//}
		//Generate();
		//printf( ( std::to_string(sizeof(m_Blocks)) + std::string("\n") ).c_str() ) ;
		// Create buffer
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		//glBindBuffer(GL_VERTEX_ARRAY, VAO);
		unsigned int size = sizeof(QuadVertex) * 100000;

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

		Renderer::m_IndexBuffer->Bind();
		//Renderer::m_VertexArray->Bind();

		// pos vec3
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), 0);
		// normal vec3
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(sizeof(GL_FLOAT) * 3));
		// color vec4
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(sizeof(GL_FLOAT) * 3));
		// texture pos vec2
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(sizeof(GL_FLOAT) * 7));
		// texture float
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(sizeof(GL_FLOAT) * 9));
		// tiling float
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (void*)(sizeof(GL_FLOAT) * 11));

		glBindVertexArray(0);
	}

	void SubChunk::Generate() {
		FastNoiseLite noise;

		auto parent = m_Parent;
		float gx = parent->GetPosition().x * SIZE;
		float gy = m_Index * SIZE;
		float gz = parent->GetPosition().y * SIZE;

		for (auto i = 0; i < SIZE; i++) {
			for (auto j = 0; j < SIZE; j++) {
				for (auto k = 0; k < SIZE; k++) {
					if (noise.GetNoise(gx + i, gy + j, gz + k) > 0) {
						m_Blocks[i][j][k] = (char)1;
						m_Count += 1;
					}
						
				}
			}
		}
	}

	SubChunk::~SubChunk() { // Delete the blocks
		//delete m_Blocks;
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		
		//glDeleteBuffers(1, &VAO);
	}


	void SubChunk::SetBlock(int x, int y, int z, int type)
	{
		int oldType = m_Blocks[x][y][z];
		m_Blocks[x][y][z] = type;
		if (type != 0 && oldType == 0) m_Count++;
	}

	int SubChunk::GetBlock(int x, int y, int z)
	{
		return (int)m_Blocks[x][y][z];
	}

	// Creates a mesh with the blocks inside the chunk.
	void SubChunk::Mesh() {
		//if (m_Count == 0)
		//	return;

		this->m_Mesh = ChunkMesher::MeshSubChunk(this);
		this->m_IndexCount = m_Mesh->size() * 1.5;
		unsigned int indices = m_IndexCount * sizeof(QuadVertex);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_Mesh->size() * sizeof(QuadVertex), m_Mesh->data());
		ChunkMesher::Clear();
	}

	// Push the mesh to the renderer 
	void SubChunk::Draw() {
		if (this == nullptr)
			return;
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