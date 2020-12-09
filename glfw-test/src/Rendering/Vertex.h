#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float4.hpp>
#include <glm\ext\vector_float2.hpp>
struct QuadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float TexIndex;
	float TilingFactor;
};