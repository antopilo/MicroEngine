#pragma once
#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Engine {
	class Renderer {
	public:
		static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
		static void Clear();
	};
}








