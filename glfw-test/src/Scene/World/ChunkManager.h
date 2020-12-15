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
		static std::map<ChunkPos, std::shared_ptr<Chunk>>* m_Chunks;

	public:
		static Camera* m_Camera;

		static int m_LoadedChunkCount;

		static void SetCam(Camera* cam);

		static Camera* GetCam();

		static void Init();

		static bool IsChunkLoaded(int x, int z);

		static void LoadChunk(int x, int z);

		static void Mesh();
		
		static void Draw();

		static void Update(Timestep ts);

		static void CheckForUnload();

		static void CheckForLoad();

		static int GetBlock(int x, int y, int z);
	};
}