#pragma once
#include <map>
#include "Chunks/Chunk.h"
#include "../Core/Timestep.h"

namespace Engine {
	class Camera;
	struct ChunkPos;

	class ChunkManager
	{
	private:
		static std::map<ChunkPos, Chunk*> m_Chunks;
		static std::vector<Chunk*> m_GenerateChunk;
	public:
		static Camera* m_Camera;

		static int m_LoadedChunkCount;

		static void SetCam(Camera* cam);

		static Camera* GetCam();

		static void Init();

		static Chunk* GetChunk(int x, int z);

		static void CheckSurrounded();

		static bool IsChunkLoaded(int x, int z);

		static void LoadChunk(int x, int z);
	
		static int GetBlock(int gx, int gy, int gz);

		static void Generate();

		static void Mesh();
		
		static void Draw();

		static void Update(Timestep ts);

		static void CheckForUnload();

		static void CheckForLoad();

	};
}