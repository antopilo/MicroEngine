#pragma once
#include <unordered_map>
#include "Chunks/Chunk.h"
#include "../Core/Timestep.h"

namespace Engine {
	class Camera;
	class ChunkManager
	{
	private:
		static std::unordered_map<int, std::unordered_map<int, Chunk*>> m_Chunks;

	public:
		static Camera* m_Camera;

		static int m_LoadedChunkCount;

		static void SetCam(Camera* cam);

		static Camera* GetCam();

		static void Init();

		static bool IsChunkLoaded(int x, int z);

		static void LoadChunk(int x, int z);
		
		static void Draw();

		static void Update(Timestep ts);

		static void CheckForLoad();

		static int GetBlock(int x, int y, int z);
	};
}