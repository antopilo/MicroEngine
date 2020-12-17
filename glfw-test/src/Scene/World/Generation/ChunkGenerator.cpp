#pragma once
#include "ChunkGenerator.h"
#include "../Chunks/SubChunk.h"
#include "FastNoise.h"
#include "Features/Boulder.h"

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

		float gx = chunk->GetPosition().x * SubChunk::SIZE;
		float gz = chunk->GetPosition().y * SubChunk::SIZE;

		float scale = 0.2f;
		for (int x = 0; x < SubChunk::SIZE; x++) {
			for (int z = 0; z < SubChunk::SIZE; z++) {
				float noisex = (gx + x) * scale;
				float noisez = (gz + z) * scale;
				noise.DomainWarp(noisex, noisez);
				float noiseValue = ((noise.GetNoise(noisex * 0.2f, noisez * 0.2f) + 1.0f) / 2.0f);
				float noiseValue2 = ((noise2.GetNoise(noisex, noisez ) + 1.0f) / 2.0f);
				float noiseValue3 = ((noise3.GetNoise(noisex * 0.5f, noisez * 0.5f) + 1.0f) / 2.0f);
				float result = (noiseValue + (noiseValue2 * 0.2f) + (noiseValue3 * 3)) / 3.0f;
				//noiseValue += ((noise3.GetNoise(noisex, noisez) + 1.0f) / 2.0f);
				//noiseValue;
				int height = (int)(result * (result)*amplitude);
				for(auto i = height + 3; i > 1 && i > height; i --)
					chunk->SetBlock(x, i, z, 1);

				float boulderChange = rand() % 10000;

				//if (boulderChange < 0.01f) {
				//	Boulder boulder;
				//
				//	for (int x = 0; x < Boulder::MAX_SIZE; x++)
				//		for (int y = 0; y < Boulder::MAX_SIZE; y++) {
				//			for (int z = 0; z < Boulder::MAX_SIZE; z++) {
				//				int block = (int)boulder.m_Blocks[x][y][z];
				//				if(block != 0)
				//					chunk->SetBlock(x, (height - 5) + y, z, block);
				//			}
				//		}
				//}

				//bool touchedGround = false;
				//int offset = 0;
				//for (auto y = height; y < SubChunk::SIZE * Chunk::SUBCHUNK_COUNT; y++) {
				//	float noise3d = (noise.GetNoise(noisex * 6.0f, (float)y, noisez * 6.0f) + 1.0f) / 2.0f;
				//	float above = (noise.GetNoise(noisex * 6.0f, (float)y + 1, noisez * 6.0f) + 1.0f) / 2.0f;
				//	float maxHeight = height * amplitude;
				//	
				//	if (noise3d > 0.8 && y < maxHeight && above + 0.01f > noise3d ) {
				//		if (!touchedGround) {
				//			offset = height - y;
				//			touchedGround = true;
				//		}
				//			
				//		chunk->SetBlock(x, y + offset, z, 1);
				//	}
				//}
			}
		}

		
		

	}
}
