#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float4.hpp>
#include <glm\ext\vector_float2.hpp>

namespace Engine {
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		QuadVertex(glm::vec3 pos, glm::vec3 normal, glm::vec4 color, glm::vec2 texCoord, float tex, float tile) {
			Position = pos;
			Normal = normal;
			Color = color;
			TexCoord = texCoord;
			TexIndex = tex;
			TilingFactor = tile;
		}
	};
}
