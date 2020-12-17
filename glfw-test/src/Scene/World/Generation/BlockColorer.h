#pragma once
#include <glm\ext\vector_float4.hpp>
namespace Engine
{

	class BlockColorer
	{
	public:
		static glm::vec4 GetBlockColor(int type);
		static glm::vec4 MixColor(glm::vec4 input, float value, float amount);
		static glm::vec4 Darken(glm::vec4 input, float amount);
		static glm::vec4 MixColor(glm::vec4 input, glm::vec4 value, float amount);
	};
}