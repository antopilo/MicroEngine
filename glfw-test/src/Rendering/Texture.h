#pragma once
#include "Renderer.h"
#include "stb_image/stb_image.h"

namespace Engine {
	class Texture 
	{
	private:
		unsigned int m_RendererId;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer;
		int m_Width;
		int m_Height;
		int m_BPP; // byte per pixel.

	public:
		Texture(const std::string& path);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
	};
}


