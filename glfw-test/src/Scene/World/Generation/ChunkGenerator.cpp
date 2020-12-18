#pragma once
#include "ChunkGenerator.h"
#include "../Chunks/SubChunk.h"
#include "FastNoise.h"
#include "Features/Boulder.h"
#include "Features/Plateau.h"
#include "Features/SimpleTree.h"

namespace Engine {
	void ChunkGenerator::GenerateHeightPass(Chunk* chunk) {
		
		FastNoiseLite noise;
		float amplitude = 128;
		noise.SetFractalOctaves(4);
		noise.SetFractalLacunarity(0.95f);
		noise.SetFractalWeightedStrength(1.2f);

		FastNoiseLite noise2;
		float amplitude2 = 128;
		noise2.SetFractalLacunarity(0.9f);
		noise2.SetFractalOctaves(4);
		noise2.SetFrequency(0.06f);

		FastNoiseLite noise3;
		noise3.SetFractalLacunarity(0.9f);
		noise3.SetFractalOctaves(8);
		noise3.SetFrequency(0.01);

		noise.SetDomainWarpAmp(0.1f);

		float gx = chunk->GetPosition().x * SubChunk::SIZE;
		float gz = chunk->GetPosition().y * SubChunk::SIZE;
		float pathWidth = 10.0f;
		float scale = 0.2f;
		for (int x = 0; x < SubChunk::SIZE; x++) {
			for (int z = 0; z < SubChunk::SIZE; z++) {
				float noisex = (gx + x) * scale;
				float noisez = (gz + z) * scale;
				noise.DomainWarp(noisex, noisez);
				float noiseValue = ((noise.GetNoise(noisex * 0.5f, noisez * 0.5f) + 1.0f) / 2.0f);
				float noiseValue2 = ((noise2.GetNoise(noisex, noisez ) + 1.0f) / 2.0f);
				float noiseValue3 = ((noise3.GetNoise(noisex * 0.5f, noisez * 0.5f) + 1.0f) / 2.0f);
				float result = (noiseValue + (noiseValue2 * 0.2f) + (noiseValue3 * 3)) / 3.0f;
				//noiseValue += ((noise3.GetNoise(noisex, noisez) + 1.0f) / 2.0f);
				//noiseValue;
				int height = (int)(result * (result)*amplitude);
				int depth = 6;

				float riverX = (gx + x) * 0.1f;
				float riverZ = (gz + z) * 0.1f;
				noise.DomainWarp(riverX, riverZ);
				float riverWiggle = ((noise3.GetNoise(riverX, riverZ) + 1.0f) / 2.0f) * 0.1f;
				float river1 = ((noise2.GetNoise(riverX * 0.2f, riverZ * 0.2f) + 1.0f) / 2.0f) + riverWiggle;
				//iver1 < 0.55 && river1 > 0.5
				if (river1 > 0.45 && river1 < 0.6f) {
					float distFromRiver = abs((0.525f - river1)) / (0.525f - 1.16f);
					height -= (distFromRiver ) * 5.0f;
				}
				
				for (auto i = height + depth; i > 1 && i > height; i--) {
					float dirtX = (gx + x) * 0.5f;
					float dirtZ = (gz + z) * 0.5f;
					noise.DomainWarp(dirtX, dirtZ);

					float path2 = ((noise.GetNoise(dirtX, dirtZ) + 1.0f) / 2.0f);
					float path1 = ((noise2.GetNoise(dirtX * 0.25f, dirtZ * 0.25f) + 1.0f) / 2.0f);;

					float pathResult = (path1 + (path2 * 0.2f) ) / 2.0f;
					//float path2 = ((noise2.GetNoise((noisex + pathWidth) * 0.25f, (noisez) * 0.5f) + 1.0f) / 2.0f);;
					if (i == height + depth) {
						if (!(pathResult > 0.5f))
							chunk->SetBlock(x, i, z, 1);
					}
					else
						chunk->SetBlock(x, i, z, 2);

					
				}
				// Rivers
				if (height - depth > 1) {
					if (river1 < 0.55 && river1 > 0.5) {
						for (int i = height + depth; i > 0 && i > height - 2; i--) {
							if (i == height || i == height + 1)
								chunk->SetBlock(x, i, z, 4);
							else
								chunk->SetBlock(x, i, z, 0);
						}
					}
				}
				
			}
		}
	}

	void ChunkGenerator::GenerateDecoration(Chunk* chunk) {
		float plateauChance = rand() % 100;
		if (plateauChance < 5) {
			Plateau plateau;
			float height = Chunk::SUBCHUNK_COUNT * SubChunk::SIZE - 1;
			// Find lowest height in the chunk
			for(auto x = 0; x < SubChunk::SIZE - 1; x++)
				for (auto z = 0; z < SubChunk::SIZE - 1; z++)
					for (auto y = Chunk::SUBCHUNK_COUNT * SubChunk::SIZE - 1; y > 0; y--) {
						if (chunk->GetBlock(Plateau::MAX_SIZE / 2, y, Plateau::MAX_SIZE / 2) != 0 && y < height) {
							height = y;
							break;
						}
					}

			// Place blocks in chunks
			for (int x = 0; x < Plateau::MAX_SIZE; x++)
				for (int y = 0; y < Plateau::MAX_SIZE; y++) {
					for (int z = 0; z < Plateau::MAX_SIZE; z++) {
						int block = (int)plateau.m_Blocks[x][y][z];
						int localY = (height) + y - 5;
						if (block != 0 && localY > 0)
							chunk->SetBlock(x, localY, z, block);
					}
				}
		}

		float boulderChange = rand() % 100;
		if (boulderChange < 10) {
			Boulder boulder;
			float height = 0;
			// find height
			for (auto i = Chunk::SUBCHUNK_COUNT * SubChunk::SIZE - 1; i > 0; i--) {
				if (chunk->GetBlock(Boulder::MAX_SIZE / 2, i, Boulder::MAX_SIZE / 2) != 0) {
					height = i;
					break;
				}
			}
			for (int x = 0; x < Boulder::MAX_SIZE; x++)
				for (int y = 0; y < Boulder::MAX_SIZE; y++) {
					for (int z = 0; z < Boulder::MAX_SIZE; z++) {
						int block = (int)boulder.m_Blocks[x][y][z];
						int localY = (height - Boulder::MAX_SIZE / 2) + y;
						if(block != 0 && localY > 0)
							chunk->SetBlock(x, localY, z, block);
					}
				}
		}

		float treeChance = rand() % 100;
		if (treeChance < 96) {

			int amount = rand() % 3;

			for (int i = 0; i < 1; i++) {
				int randomx = rand() % 8 + 4;
				int randomz = rand() % 8 + 4;
				SimpleTree tree;
				float height = 0;

				// find height
				for (auto i = Chunk::SUBCHUNK_COUNT * SubChunk::SIZE - 1; i > 0; i--) {
					if (chunk->GetBlock(randomx, i, randomz) != 0) {
						height = i;
						break;
					}
				}
				for (int x = 0; x < SimpleTree::MAX_SIZE; x++)
					for (int y = 0; y < SimpleTree::MAX_SIZE; y++) {
						for (int z = 0; z < SimpleTree::MAX_SIZE; z++) {
							int block = (int)tree.m_Blocks[x][y][z];
							int localY = (height)+y;
							if (block != 0 && localY > 0)
								chunk->SetBlock(x + randomx, localY, z + randomz, block);
						}
					}
			}
			
		}

		chunk->isGenerated = true;
	}
}
