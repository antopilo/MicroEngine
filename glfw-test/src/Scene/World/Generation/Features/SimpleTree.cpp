#include "SimpleTree.h"
#include <vcruntime_string.h>
#include <stdlib.h> 
#include <glm\ext\vector_float3.hpp>
#include <string>
#include "../FastNoise.h"

namespace Engine {
	SimpleTree::SimpleTree() {
		memset(&m_Blocks, 0, MAX_SIZE * MAX_SIZE * MAX_SIZE);

		// Generate data
	}

	// Return generated data
	void* SimpleTree::GetData() {
		return &m_Blocks;
	}
}