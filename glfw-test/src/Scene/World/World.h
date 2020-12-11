#pragma once
#include "../Core/Timestep.h"
#include <string>
namespace Engine {
	class Camera;
	class ChunkManager;
	class World 
	{
	private:
		int m_Seed;
		std::string m_Name;
		Camera* m_Camera;
		
	public:

		World(int Seed, Camera* camera);
		~World();

		void Init();
		void Update(Timestep ts);
		void Draw();
	};

}