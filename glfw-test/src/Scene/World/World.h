#pragma once
#include "ChunkManager.h"
#include "../Core/Timestep.h"
namespace Engine {

	class World 
	{
	private:
		int m_Seed;
		std::string m_Name;
		ChunkManager* m_ChunkManager;
	public:

		World(int Seed);
		~World();

		void Init();
		void Update(Timestep ts);
		void Draw();
	};

}