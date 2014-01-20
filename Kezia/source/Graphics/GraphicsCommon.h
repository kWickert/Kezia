#pragma once

#include "Core/IResourceManager.h"
#include "Core/FileResource.h"
#include "Core/Types.h"

#include "Graphics/BufferObject.h"
#include "Graphics/Camera.h"
#include "Graphics/glError.h"
#include "Graphics/Material.h"
#include "Graphics/OpenGL_Renderer.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace Kezia
{
	typedef BufferObject< Vector3<F32>, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW> PositionBufferObject;
	typedef BufferObject< Vector2<F32>, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW> TextureCoordinateBufferObject;

	static Vector2<F32> defaultQuadTexCoords[4] = 
	{	
		Vector2<F32>(0.0f, 0.0f),
		Vector2<F32>(1.0f, 0.0f),
		Vector2<F32>(0.0f, 1.0f),
		Vector2<F32>(1.0f, 1.0f)
	};

	static Vector3<F32> defaultQuadPositions[4] =
	{	
		Vector3<F32>(-1.0f, -1.0f, 0.0f),
		Vector3<F32>(1.0f, -1.0f, 0.0f),
		Vector3<F32>(-1.0f, 1.0f, 0.0f),
		Vector3<F32>(1.0f, 1.0f, 0.0f)
	};

	inline Material * CreateOrGetTexturedQuadMaterial()
	{
		static PositionBufferObject * positionData = new PositionBufferObject(defaultQuadPositions, 4);

		static TextureCoordinateBufferObject * textureCoordinateData = new TextureCoordinateBufferObject(defaultQuadTexCoords, 4);

		Material * texturedQuadMaterial = reinterpret_cast<Material *>(g_Renderer->GetMaterial("TexturedQuad"));

		texturedQuadMaterial->GetAttribute("a_Position")->SetBufferObject(positionData);
		texturedQuadMaterial->GetAttribute("a_TextureCoordinates")->SetBufferObject(textureCoordinateData);

		return texturedQuadMaterial;
	}

	inline void DrawTexturedQuad(const Camera * camera, const Texture & texture, const Vector2<F32> * textureCoordinates = nullptr, const Vector3<F32> & center = Vector3<F32>(), const Vector2<F32> dimensions = Vector2<F32>(1.0f, 1.0f), const Matrix3<F32> & orientation = Matrix3<F32>(), const Color & color = Color(1.0f))
	{
		static Material * material = CreateOrGetTexturedQuadMaterial();

		material->SetUniformValue("u_QuadTexture", texture);

		if(textureCoordinates == nullptr)
		{
			textureCoordinates = defaultQuadTexCoords;
		}
		reinterpret_cast<TextureCoordinateBufferObject *>(material->GetAttribute("a_TextureCoordinates"))->UpdateData(textureCoordinates, 4);

		Matrix4<F32> modelViewProjection;
		Matrix4<F32> model = Matrix4<F32>::Translate(center.x(), center.y(), center.z()) * Matrix4<F32>::Scale(dimensions.x(), dimensions.y(), 0.0f);
		if(camera != nullptr)
		{
			modelViewProjection = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;
		}
		else
		{
			modelViewProjection = model;
		}

		Vector4<F32> testOrigin = model * Vector4<F32>(0, 0, 0, 1);
		Vector4<F32> testCorner = model * Vector4<F32>(1, 1, 0, 1);

		material->SetUniformValue("u_ModelViewProjectionMatrix", modelViewProjection);

		material->SetUniformValue("u_QuadColor", color, true);

		material->Bind();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	/*	KEZIA: Draws a fullscreen textured quad centered at the origin, tinted with the given color */
	inline void DrawTexturedQuad(const Texture & texture, const Color & color = Color::White)
	{
		DrawTexturedQuad(nullptr, texture, nullptr, Vector3<F32>(), Vector2<F32>(1.0f, 1.0f), Matrix3<F32>(), color);
	}

	bool LoadMeshData(const std::string & fileName, std::vector< Vector3<F32> > & outPositions, std::vector< Vector2<F32> > & outTextureCoordinates, std::vector< Vector3<F32> > & outNormals, std::vector<U32> & outIndices);
}
