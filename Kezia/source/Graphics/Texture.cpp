#include "Core/WindowsInclude.h"
#include <GL/glew.h>
#include <GL/GL.h>

#include "Texture.h"

#include "Core/Assert.h"

#include "Graphics/glError.h"

#include "FreeImage.h"

#include <fstream>
#include <string>

namespace Kezia
{
	U32 * Texture::k_DebugTexture = nullptr;
	std::map<U32, U32> Texture::k_TextureIds;

	Texture::Texture()
		:	m_TextureId(0)
	{
		if(k_DebugTexture == nullptr)
			GenerateDebugTexture();

		LoadTexture(k_DebugTexture, k_DebugWidth, k_DebugHeight, false, DefaultTextureSetup);
	}

	Texture::Texture(const std::string & path, TextureSetupFn setupFn)
		:	m_FilePath(path),
			m_TextureId(0)
	{
		if(k_DebugTexture == nullptr)
			GenerateDebugTexture();

		const char * file = path.c_str();

		FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(file, 0);
		if(fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(file);

		KeziaAssert(fif != FIF_UNKNOWN && FreeImage_FIFSupportsReading(fif));

		FIBITMAP * dib = FreeImage_Load(fif, file);

		U8 * texels = FreeImage_GetBits(dib);
		U32 width = FreeImage_GetWidth(dib);
		U32 height = FreeImage_GetHeight(dib);
		bool isTransparent = FreeImage_IsTransparent(dib) != 0;

		if(texels)
			LoadTexture(texels, width, height, isTransparent, setupFn);
		else //TODO log bad texture load
			LoadTexture(k_DebugTexture, k_DebugWidth, k_DebugHeight, false, setupFn);

		FreeImage_Unload(dib);
	}

	Texture::Texture(void * texels, U32 width, U32 height, bool isTransparent, TextureSetupFn setupFn)
		:	m_TextureId(0)
	{
		if(k_DebugTexture == nullptr)
			GenerateDebugTexture();

		LoadTexture(texels, width, height, isTransparent, setupFn);
	}

	Texture::Texture(const Texture & other)
		:	m_TextureId(other.m_TextureId),
			m_FilePath(other.m_FilePath),
			m_Width(other.m_Width),
			m_Height(other.m_Height)
	{
		k_TextureIds[m_TextureId]++;
	}

	Texture::~Texture()
	{
		k_TextureIds[m_TextureId]--;

		if(k_TextureIds[m_TextureId] == 0)
		{
			glDeleteTextures(1, &m_TextureId);
		}
	}

	Texture & Texture::operator=(const Texture & other)
	{
		m_TextureId = other.m_TextureId;
		const_cast<std::string &>(m_FilePath) = other.m_FilePath;
		m_Width = other.m_Width;
		m_Height = other.m_Height;

		k_TextureIds[m_TextureId]++;

		return *this;
	}

	void Texture::Bind(U8 unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);

		glBindTexture(GL_TEXTURE_2D, m_TextureId);	
	}

	void Texture::Unbind(U8 unit)
	{
		glBindTexture(GL_TEXTURE_2D, unit);
	}

	void Texture::UnbindAll()
	{
		GLint numUnits;

		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &numUnits);

		for(U8 i = numUnits - 1; i >= 0; i--)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void Texture::DefaultTextureSetup(void * texels, U32 width, U32 height, bool isTransparent)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, isTransparent ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, texels);
	}

	void Texture::GenerateDebugTexture()
	{
		const U32 width = k_DebugWidth;
		const U32 height = k_DebugHeight;

		k_DebugTexture = new U32[width * height];

		// Checkerboard.
		for( size_t y = 0; y < height; ++y )
		{
			for( size_t x = 0; x < width; ++x )
			{
				Texture::k_DebugTexture[ y * width + x ] = 
					((( x / 10 ) & 1 ) == (( y / 10 ) & 1 )) ? 
					0xFFFF00FF : // Pink if x and y are both odd or even
					0xFF000000;	// Black otherwise
			}
		}

		LoadTexture(k_DebugTexture, k_DebugWidth, k_DebugHeight, false, DefaultTextureSetup);
	}

	void Texture::LoadTexture(void * texels, U32 width, U32 height, bool isTransparent, TextureSetupFn setupFn)
	{
		if(m_TextureId == 0)
		{
			U32 textureId = 0;

			ReportAllGLErrors("before load texture");

			glGenTextures(1, &textureId);

			ReportAllGLErrors("after load texture");

			KeziaAssert(textureId != 0);

			m_TextureId = textureId;
			k_TextureIds[m_TextureId]++;
		}
		
		glBindTexture(GL_TEXTURE_2D, m_TextureId);

		setupFn(texels, width, height, isTransparent);

		m_Width = width;
		m_Height = height;
	}
}