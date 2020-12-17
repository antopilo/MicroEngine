#include "Boulder.h"
#include <vcruntime_string.h>
#include <stdlib.h> 
#include <glm\ext\vector_float3.hpp>
#include <string>
#include "../FastNoise.h"

namespace Engine {
	Boulder::Boulder() {
		memset(&m_Blocks, 0, MAX_SIZE * MAX_SIZE * MAX_SIZE);

		int size = (rand() % (MAX_SIZE - 8)) / 2 + 8;

		float scale = ((rand() % 50) + 50) / 100.0f;
		bool side = rand() % 2;
		int center = size / 2;
		float offsetX = rand() % 200;
		float offsetZ = rand() % 200;
		FastNoiseLite noise;
		noise.SetDomainWarpAmp(40.0f);
		for (auto x = 0; x < MAX_SIZE; x++) {
			for (auto z = 0; z < MAX_SIZE; z++) {
				for (auto y = 0; y < MAX_SIZE; y++) {

					glm::vec3 sub = glm::vec3(x - (MAX_SIZE / 2), y - (MAX_SIZE / 2), z - (MAX_SIZE / 2));
					float distanceFromCenter = sqrt((sub.x * sub.x + sub.y * sub.y) + sub.z * sub.z);
					float value = (noise.GetNoise(x + offsetX, (float)y, z + offsetZ) + 1.0f) / 2.0f;
					if (distanceFromCenter < center + value * 10.0f)
					{ 
						m_Blocks[x][y][z] = 3;
						
						
					}
				}
			}
		}
	}            

	// Return generated data
	void* Boulder::GetData() {
		return &m_Blocks;
	}
}