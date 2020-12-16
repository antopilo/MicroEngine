#pragma once
#include "ChunkGenerator.h"
#include "../Chunks/SubChunk.h"
#include "FastNoise.h"

namespace Engine {
	void ChunkGenerator::GenerateHeightPass(Chunk* chunk) {

		FastNoiseLite noise;
		float amplitude = 64;
		noise.SetFractalOctaves(4);
		noise.SetFractalLacunarity(0.95f);
		noise.SetFractalWeightedStrength(1.2f);

		FastNoiseLite noise2;
		float amplitude2 = 128;
		noise2.SetFractalLacunarity(0.9f);
		noise2.SetFractalOctaves(8);
		noise2.SetFrequency(2.0f);

		FastNoiseLite noise3;
		noise3.SetFractalLacunarity(0.9f);
		noise3.SetFractalOctaves(8);
		noise3.SetFrequency(0.1);

		float gx = chunk->GetPosition().x * SubChunk::SIZE;
		float gz = chunk->GetPosition().y * SubChunk::SIZE;

		float scale = 0.2f;
		for (int x = 0; x < SubChunk::SIZE; x++) {
			for (int z = 0; z < SubChunk::SIZE; z++) {
				float noisex = (gx + x) * scale;
				float noisez = (gz + z) * scale;
				float noiseValue =  ((noise.GetNoise(noisex, noisez) + 1.0f) / 2.0f) * amplitude;
				// +((noise2.GetNoise(noisex, noisez) + 1.0f) / 2.0f) * amplitude2;

				
				chunk->SetBlock(x, noiseValue, z, 1);
			}
		}
	}
}
