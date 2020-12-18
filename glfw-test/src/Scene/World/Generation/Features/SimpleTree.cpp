#include "SimpleTree.h"
#include <vcruntime_string.h>
#include <stdlib.h> 
#include <glm\ext\vector_float3.hpp>
#include <string>
#include "../FastNoise.h"

namespace Engine {
	SimpleTree::SimpleTree() {
		memset(&m_Blocks, 0, MAX_SIZE * MAX_SIZE * MAX_SIZE);

		int leafType = rand() % 3 + 6;
		// Generate data
		int trunkWidth = (rand() % 2 );
		int middle = MAX_SIZE / 2;
		// between 6 and 12
		int height = (rand() % 10) + 10;
		for(auto x = middle - trunkWidth; x < middle + trunkWidth; x++)
			for (auto z = middle - trunkWidth; z < middle + trunkWidth; z++) {
				for (auto y = height; y > 0; y--) {
					m_Blocks[x][y][z] = 5; // wood

					if (y == 1) {
						m_Blocks[x + 1][y][z + 1] = 5;
						m_Blocks[x - 1][y][z - 1] = 5;
						m_Blocks[x + 1][y][z - 1] = 5;
						m_Blocks[x - 1][y][z + 1] = 5;
					}
				}

				int numBalls = (rand() % 2) + 3;
				for (auto i = 0; i < numBalls; i++) {
					int ballX = middle + ((rand() % 10) - 5);
					int ballY = height + ((rand() % 10) - 5);
					int ballZ = middle + ((rand() % 10) - 5);
					GenerateLeafBall(ballX, ballY, ballZ, leafType);
				}
			}
	}

	void SimpleTree::GenerateLeafBall(int x, int y, int z, int leafType) {
		float yScale = 1.5f;
		float radius = rand() % 5 + 3;
		
		for(auto lx = -radius; lx < radius; lx++)
			for (auto ly = -radius; ly < radius; ly++)
				for (auto lz = -radius; lz < radius; lz++) {
					auto minus = glm::vec3(lx, ly * 1.5f, lz);
					auto distance = sqrt(minus.x * minus.x + minus.y * minus.y + minus.z * minus.z);
					if (distance < radius) {
						int gx = x + lx;
						int gy = y + ly;
						int gz = z + lz;

						if(gy > 0)
							m_Blocks[gx][gy][gz] = leafType; // leaves
					}
				}
	}

	// Return generated data
	void* SimpleTree::GetData() {
		return &m_Blocks;
	}
}