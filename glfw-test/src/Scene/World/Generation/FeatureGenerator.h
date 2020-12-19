#pragma once
#include <map>
namespace Engine {
	class Chunk;
	struct StructurePos;
	struct Structure;
	class Camera;
	class FeatureGenerator {
	public:
		static std::map<StructurePos, Structure*> m_Structures;
		static void Init();
		static void SetCam(Camera* cam);
		static bool IsStructureLoaded(int x, int z);
		static void CheckLoadStructure();
		static void CheckUnloadStructure();
		static void GetChunkStructure(Chunk* chunk);

		static Camera* m_Cam;

		static int RenderDistance;
	};
}