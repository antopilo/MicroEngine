#include "FeatureGenerator.h"
#include "../../Camera.h"
#include "../Chunks/Chunk.h"
#include "../Chunks/SubChunk.h"
#include <string>
#include <glm\ext\scalar_common.hpp>


namespace Engine {

	struct Structure {
		char m_Blocks[512][512][512];
		glm::vec2 Position;
		float Height;
	};	
	struct StructurePos {
		int x;
		int y;

		bool operator<(const StructurePos& comp) const {
			return std::tie(x, y) < std::tie(comp.x, comp.y);
		}

		bool operator==(const StructurePos& o) const {
			return x == o.x && y == o.y;
		}

	};

	Structure test;

	float random(int seed, float x, float y) {
		//float value = modf(((x + y) * 1927475756), seed);
		return 1.0f;
	}

	std::vector<Structure> m_Structures;

	void FeatureGenerator::Init() {
		memset(&test.m_Blocks, 0, 512 * 512 * 512);
		test.Position = glm::vec2(12 * SubChunk::SIZE, 12 * SubChunk::SIZE);
		test.Height = 25;
		
		int height = 512 / 4;
		int center = 512 / 2;
		for (int y = 0; y < height; y++) {
			int length = height - y;
			for(int x = -length; x < length; x++)
				for (int z = -length; z < length; z++) {
					test.m_Blocks[center + x][y][center + z] = 2;
				}
		}
		
	}

	void FeatureGenerator::SetCam(Camera* cam) {
		m_Cam = cam;
	}


	int FeatureGenerator::RenderDistance = 12;

	std::map<StructurePos, Structure*> FeatureGenerator::m_Structures;
	Camera* FeatureGenerator::m_Cam;

	bool FeatureGenerator::IsStructureLoaded(int x, int z) {
		if (m_Structures.find(StructurePos{ x, z }) != m_Structures.end()) {
			return true;
		}
		return false;
	}


	void FeatureGenerator::CheckLoadStructure() {
		int camZ = m_Cam->GetTranslation().z / SubChunk::SIZE * 2;
		int camX = m_Cam->GetTranslation().x / SubChunk::SIZE * 2;
		
		for (auto x = camX - RenderDistance; x < camX + RenderDistance; x++)
			for (auto z = camZ - RenderDistance; z < camZ + RenderDistance; z++) {
		
				auto minus = glm::vec2(camX, camZ) - glm::vec2(x, z);
				float dist = sqrt(minus.x * minus.x + minus.y * minus.y);
		
				if (dist < RenderDistance && !IsStructureLoaded(x, z)) {
					float value = rand() % 100;// random(1337, x, z);

					//if (value < 10) {
					//	m_Structures.emplace(StructurePos{x , z}, Structure{})
					//}
					//printf(std::to_string(value).c_str());
				}
			}
	}

	void FeatureGenerator::CheckUnloadStructure() {

	}

	void FeatureGenerator::GetChunkStructure(Chunk* chunk)
	{
		// check if structure close
		glm::vec2 pos = glm::vec2(chunk->GetPosition().x * SubChunk::SIZE, chunk->GetPosition().y * SubChunk::SIZE);
		glm::vec2 sub = (pos - test.Position);
		float dist = sqrt(sub.x * sub.x + sub.y * sub.y);

		int structureSize = 512 / 2;
		if (dist < structureSize) { // is inside structure!
			chunk->HasStructure = true;
			int minX = sub.x + structureSize ;
			int minZ = sub.y + structureSize ;
			for (int lx = 0; lx < SubChunk::SIZE; lx++) {
				minX++;
				minZ = sub.y + structureSize;
				for (int lz = 0; lz < SubChunk::SIZE; lz++) {

					minZ++;
					for (int ly = 1; ly < 512; ly++) {
						int type = test.m_Blocks[minX - 1][ly][minZ - 1];
						if(type != 0)
							chunk->SetBlock(lx, ly + test.Height, lz, type);
					}

				}
			}
		}
	}



}