#pragma once
#include <vector>
#include <glm\ext\vector_float3.hpp>
#include <memory>
namespace Engine {
	class SubChunk;
	struct QuadVertex;

	static class ChunkMesher
	{
	public:
		static void Init();
		static std::vector<QuadVertex>* MeshSubChunk(SubChunk* subchunk);
		static std::vector<QuadVertex>* CurrentArray;
		static void Clear();
	private:
        const float CUBE_SIZE = 1.0f;
        const static glm::vec3 CUBE_VERTICES[];
		const static glm::vec3 CUBE_NORMALS[];
        
		static bool Left;
		static bool Right;
		static bool Front;
		static bool Back;
		static bool Top;
		static bool Bottom;
		static glm::vec3 Position;

		//static QuadVertex newArray[100000];
		//static std::unique_ptr<std::vector<QuadVertex>> CurrentArray;

		static void CreateBlock(int x, int y, int z, int type, SubChunk* chunk);

		static void PushQuadAO(int face, int x, int y, int z, int c1, int c2, int c3, int c4, int type);

		static float AOLookUp(int face,int x, int y, int z, int c1);

		static float PushTriangleAO(int x, int y, int z, int v1, int v2, int v3, float a1, float a2, float a3);
		static void PushQuad(int face, int x, int y, int z, int c1, int c2, int c3, int c4);
		static int VertexAO(int side1, int side2, int corner);
	};
}