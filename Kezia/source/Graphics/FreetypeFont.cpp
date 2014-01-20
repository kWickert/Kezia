#include "FreetypeFont.h"

#include "Core/Logger.h"

#include "Graphics/Material.h"
#include "Graphics/OpenGL_Renderer.h"

#include <vector>

namespace Kezia
{
	Texture * FreetypeFont::k_FontTexture;
	FT_Library FreetypeFont::k_Library;

	BufferObjectBase * FreetypeFont::k_TextureCoordinates = nullptr;
	BufferObjectBase * FreetypeFont::k_PositionData = nullptr;

	FreetypeFont::FreetypeFont(const std::string & fontPath, const U32 fontSize)
		:	m_CurrentColor(Color::Green)
	{
		static bool libraryInitResult = InitializeFreetype();
		static bool shaderInitResult = InitialzeFonts();

		if(!libraryInitResult)
		{
			//todo log this
			LOG("could not load freetype library");
		}
		else if(FT_New_Face(k_Library, fontPath.c_str(), 0, &m_FontFace) != 0)
		{
			LOG("could not open font, " << fontPath.c_str());
		}

		m_Material = dynamic_cast<Material *>(g_Renderer->GetMaterial("Font"));

		SetFontSize(fontSize);
		SetResolutionScale(2.0f / 800.0f, 2.0f / 600.0f);
	}

	FreetypeFont::~FreetypeFont()
	{}

	const F32 FreetypeFont::RenderText(const std::string & text, const Vector3<F32> & origin, const Vector3<F32> & forward, const Vector3<F32> & up)
	{
		Vector3<F32> cursor = origin;

		reinterpret_cast< UniformObject<Vector4<F32> > * >(m_Material->GetUniform("textColor"))->SetValue(m_CurrentColor);
		m_Material->GetAttribute("a_Position")->SetBufferObject(k_PositionData);

		m_Material->SetUniformValue("fontTexture", *k_FontTexture);

		m_Material->Bind();
		m_Material->GetAttribute("a_TextureCoordinates")->SetBufferObject(k_TextureCoordinates);

		for(auto it = text.begin(); it != text.end(); ++it)
		{
			char c = *it;

			if(FT_Load_Char(m_FontFace, c, FT_LOAD_RENDER))
			{
				LOG("could not load character, " << c);
			}

			FT_GlyphSlot g = m_FontFace->glyph;

			k_FontTexture->LoadTexture(g->bitmap.buffer, g->bitmap.width, g->bitmap.rows, true, FontTextureSetup);

			std::vector< Vector3<F32> > positions;
			positions.reserve(4);

			Vector3<F32> ul(cursor);
			ul += g->bitmap_left * m_ResolutionScale.x() * forward;
			ul += g->bitmap_top * m_ResolutionScale.y() * up;

			Vector3<F32> ur(ul);
			ur += g->bitmap.width * m_ResolutionScale.x() * forward;

			Vector3<F32> bl(ul);
			bl -= g->bitmap.rows * m_ResolutionScale.y() * up;

			Vector3<F32> br(bl);
			br += g->bitmap.width * m_ResolutionScale.x() * forward;

			positions.push_back(ul);
			positions.push_back(ur);
			positions.push_back(bl);
			positions.push_back(br);
			
			reinterpret_cast<PositionBufferObject * >(k_PositionData)->UpdateData(positions.data(), positions.size());

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			cursor += static_cast<F32>(g->advance.x >> 6) * m_ResolutionScale.x() * forward;
			cursor += static_cast<F32>(g->advance.y >> 6) * m_ResolutionScale.y() * up;
		}

		return Project(cursor - origin, forward).GetLength();
	}

	const F32 FreetypeFont::RenderText(const std::string & text, const Color & color, const Vector3<F32> & origin, const Vector3<F32> & forward, const Vector3<F32> & up)
	{
		SetFontColor(color);

		return RenderText(text, origin, forward, up);
	}

	const F32 FreetypeFont::Measure(const std::string & text, const Vector3<F32> & forward)
	{
		F32 length = 0.0f;

		for(auto it = text.begin(); it != text.end(); ++it)
		{
			char c = *it;

			if(FT_Load_Char(m_FontFace, c, FT_LOAD_RENDER))
			{
				LOG("could not load character, " << c);
			}

			FT_GlyphSlot g = m_FontFace->glyph;

			length += static_cast<F32>(g->advance.x >> 6) * m_ResolutionScale.x();
		}

		return length * forward.GetLength();
	}

	void FreetypeFont::SetFontSize(const U32 fontSize)
	{
		FT_Set_Pixel_Sizes(m_FontFace, 0, fontSize);
	}

	void FreetypeFont::SetFontColor(const Color & color)
	{
		m_CurrentColor = color;
	}

	void FreetypeFont::SetResolutionScale(const F32 xScale, const F32 yScale)
	{
		m_ResolutionScale = Vector2<F32>(xScale, yScale);
	}

	bool FreetypeFont::InitializeFreetype()
	{
		return FT_Init_FreeType(&k_Library) == 0;
	}

	bool FreetypeFont::InitialzeFonts()
	{
		Vector2<F32> textureCoords[4] = 
			{	
				Vector2<F32>(0.0f, 0.0f),
				Vector2<F32>(1.0f, 0.0f),
				Vector2<F32>(0.0f, 1.0f),
				Vector2<F32>(1.0f, 1.0f)
			};

		k_TextureCoordinates = new TextureCoordinateBufferObject(textureCoords, 4);

		Vector3<F32> positionData[4] =
			{	
				Vector3<F32>(0.0f, 0.0f, 0.0f),
				Vector3<F32>(0.0f, 0.0f, 0.0f),
				Vector3<F32>(0.0f, 0.0f, 0.0f),
				Vector3<F32>(0.0f, 0.0f, 0.0f)
			};

		k_PositionData = new PositionBufferObject(positionData, 4);

		k_FontTexture = new Texture();

		return true;
	}

	void FreetypeFont::FontTextureSetup(void * texels, U32 width, U32 height, bool isTransparent)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, texels);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}

	void FreetypeFont::CleanUpFonts()
	{
		delete k_PositionData;
		delete k_TextureCoordinates;
	}
}