#pragma once

#include "Core/Types.h"

#include <map>
#include <string>

namespace Kezia
{
	typedef void(*TextureSetupFn)(void *, U32, U32, bool); 

	class Texture
	{
	public:
		Texture();
		Texture(const std::string & path, TextureSetupFn setupFn = DefaultTextureSetup);
		Texture(void * texels, U32 width, U32 height, bool isTransparent, TextureSetupFn setupFn = DefaultTextureSetup);

		Texture(const Texture & other);

		~Texture();

		Texture & operator=(const Texture & rhs);

		void Bind(U8 unit = 0) const;

		bool IsLoaded() const	{ return m_TextureId != 0; }

		U32 GetWidth() const	{ return m_Width; }
		U32 GetHeight() const	{ return m_Height; }
		U32 GetId() const		{ return m_TextureId; }

		/*	KEZIA: Loads image data into texture object, this function is called internally on contruction */
		void LoadTexture(void * texels, U32 width, U32 height, bool isTransparent, TextureSetupFn setupFn = DefaultTextureSetup);

		static void Unbind(U8 unit = 0);
		static void UnbindAll();

		static void DefaultTextureSetup(void * texels, U32 width, U32 height, bool isTransparent);
	private:
		void GenerateDebugTexture();
		
		static U32 * k_DebugTexture;
		static const U32 k_DebugWidth = 100;
		static const U32 k_DebugHeight = 100;

		static std::map<U32, U32> k_TextureIds;

		U32 m_TextureId;
		const std::string m_FilePath;

		U32 m_Width;
		U32 m_Height;

		bool m_GLTextureGenerated;
	};
}