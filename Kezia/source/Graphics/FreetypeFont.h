#pragma once

#include "Core/Types.h"

#include <gl/glew.h>

#include "Graphics/BufferObject.h"
#include "Graphics/Color.h"
#include "Graphics/Texture.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Kezia
{
	class BufferObjectBase;
	class Material;

	class FreetypeFont
	{
	public:
		FreetypeFont(const std::string & fontPath, const U32 fontSize = 24);
		~FreetypeFont();

		const F32 RenderText(const std::string & text, const Vector3<F32> & origin, const Vector3<F32> & forward = Vector3<F32>(1, 0, 0), const Vector3<F32> & up = Vector3<F32>(0, 1, 0));
		const F32 RenderText(const std::string & text, const Color & color, const Vector3<F32> & origin, const Vector3<F32> & forward = Vector3<F32>(1, 0, 0), const Vector3<F32> & up = Vector3<F32>(0, 1, 0));
		const F32 Measure(const std::string & text, const Vector3<F32> & forward = Vector3<F32>(1, 0, 0));

		void SetMaterial(Material * material);
		void SetFontSize(const U32 fontSize);
		void SetFontColor(const Color & color);
		void SetResolutionScale(const F32 xScale, const F32 yScale);

		static void CleanUpFonts();
	private:
		static Texture * k_FontTexture;
		static FT_Library k_Library;

		typedef BufferObject< Vector3<F32>, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW> PositionBufferObject;
		typedef BufferObject< Vector2<F32> > TextureCoordinateBufferObject;

		Vector2<F32> m_ResolutionScale;

		FT_Face m_FontFace;

		Color m_CurrentColor;

		Material * m_Material;

		static BufferObjectBase * k_TextureCoordinates;
		static BufferObjectBase * k_PositionData;

		static bool InitializeFreetype();
		static bool InitialzeFonts();
		static void FontTextureSetup(void * texels, U32 width, U32 height, bool isTransparent);
	};
}