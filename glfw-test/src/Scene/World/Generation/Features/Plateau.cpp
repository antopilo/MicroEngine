#include "Plateau.h"
#include <vcruntime_string.h>
#include <stdlib.h> 
#include <glm\ext\vector_float2.hpp>
#include <string>
#include "../FastNoise.h"

namespace Engine {
	Plateau::Plateau() {
		memset(&m_Blocks, 0, MAX_SIZE * MAX_SIZE * MAX_SIZE);

		int size = (rand() % (MAX_SIZE - 8)) / 2 + 8;
		Height = rand() % (16 - 6) + 6;
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
					float x2 = x;
					float z2 = z;

					glm::vec2 sub = glm::vec2(x2 - (MAX_SIZE / 2), z2 - (MAX_SIZE / 2));
					float distanceFromCenter = sqrt(sub.x * sub.x + sub.y * sub.y);

					float newSize = size + noise.GetNoise(x2 + offsetX, (float)y, z2 + offsetZ) * 10.0f;
					if (distanceFromCenter < newSize)
					{
						float value = (noise.GetNoise(x2 + offsetX, z2 + offsetZ) + 1.0f) / 2.0f;

						if (y < Height + value * 10) {
							if (y + 1 >= Height + value * 10) // if the block above doesnt pass the check, place grass
								m_Blocks[x][y][z] = 1;
							else
								m_Blocks[x][y][z] = 2;
						}
					}
				}
			}
		}
	}

	// Return generated data
	void* Plateau::GetData() {
		return &m_Blocks;
	}
}