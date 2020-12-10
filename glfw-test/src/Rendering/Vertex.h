#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float4.hpp>
#include <glm\ext\vector_float2.hpp>

namespace Engine {
	struct QuadVertex
	{
		glm::vec2 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		QuadVertex(glm::vec2 pos, glm::vec4 color, glm::vec2 texCoord, float tex, float tile) {
			Position = pos;
			Color = color;
			TexCoord = texCoord;
			TexIndex = tex;
			TilingFactor = tile;
		}

		QuadVertex() {};
	};
}
