#include "BlockColorer.h"

namespace Engine {
	glm::vec4 BlockColorer::GetBlockColor(int type) {
		switch (type) {
		case 1: // grass
			return glm::vec4(0.227f, 0.839f, 0.298f, 1.0f);
			break;
		case 2: // dirt
			return glm::vec4(1.0f, 0.831f, 0.141f, 1.0f);
			break;
		case 3: // stone
			return glm::vec4(0.427f, 0.427f, 0.435f, 1.0f);
			break;
		case 4: // water
			return glm::vec4(0, 0.443f, 0.858f, 1.0f);
			break;

		}
	}

	glm::vec4 BlockColorer::MixColor(glm::vec4 input, float value, float amount) {
		return glm::vec4((input.x + value) / 2.0f, (input.y + value) / 2.0f, (input.z + value) / 2.0f, 1.0f);
	}

	glm::vec4 BlockColorer::Darken(glm::vec4 input, float amount) {
		return glm::vec4(input.r * amount, input.g * amount, input.b * amount, 1.0f);
	}

	glm::vec4 BlockColorer::MixColor(glm::vec4 input, glm::vec4 value, float amount) {
		return glm::vec4((input.x + value.r) / 2.0f, (input.y + value.g) / 2.0f, (input.z + value.b) / 2.0f, (input.a + input.a) / 2.0f);
	}


}