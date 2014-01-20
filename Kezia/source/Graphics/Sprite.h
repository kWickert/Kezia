#pragma once

#include "Core/HashedRegistry.h"
#include "Core/IResourceManager.h"
#include "Core/FileResource.h"
#include "Core/Types.h"
#include "Core/XmlNode.h"

#include "Graphics/BufferObject.h"
#include "Graphics/glError.h"
#include "Graphics/Material.h"
#include "Graphics/OpenGL_Renderer.h"
#include "Graphics/Texture.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace Kezia
{
	/*
	inline void DrawSprite(const Vector3<F32> & center, F32 width, F32 height, const Texture & texture, const Color & color = Color(1.0f))
	{
		static Material * material = GenerateSpriteMaterial();

		material->SetUniformValue("u_ModelViewProjectionMatrix", Matrix4<F32>::Translate(center.x(), center.y(), center.z()));

		material->SetUniformValue("u_SpriteColor", color);
		material->SetUniformValue("u_SpriteTexture", texture);

		material->Bind();

		Vector3<F32> xOffset(width * 0.5f, 0, 0);
		Vector3<F32> yOffset(0, height * 0.5f, 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	*/

	class Camera;
	class Sprite;

	class SpriteAnimation
	{
	public:
		SpriteAnimation(const XmlNode & node);
		SpriteAnimation(const Texture & texture);
		~SpriteAnimation();

		void AddFrame(const Vector2<F32> & ul, const Vector2<F32> & br);
	private:
		friend class Sprite;

		typedef BufferObject< Vector3<F32>, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW> PositionBufferObject;
		typedef BufferObject< Vector2<F32>, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW > TextureCoordinateBufferObject;

		static PositionBufferObject * CreateOrGetPositions();
		static TextureCoordinateBufferObject * CreateOrGetTextureCoordinates();
		static Material * CreateOrGetSpriteMaterial();

		void Draw(const Camera * camera);
		void Update(const F64 deltaTime);

		void Play(bool loop);

		Texture m_Texture;

		bool m_IsPlaying;
		bool m_IsLooping;

		F64 m_ElapsedTime;
		F64 m_FrameDuration;

		struct Frame
		{
			Vector2<F32> ul;
			Vector2<F32> br;
		};

		std::vector<Frame> m_Frames;
	};

	class Sprite
	{
	public:
		Sprite(const XmlNode & node);
		Sprite(const Sprite & other);
		~Sprite();

		void Draw(const Camera * camera);
		void Update(const F64 deltaTime);

		void Play(const std::string & animationName, bool loop);

		const std::string GetCurrentAnimationName();

		void AddAnimation(const std::string & animationName, const Texture & animationSheet);
		SpriteAnimation * GetAnimation(const std::string & animationName);
	private:
		HashedRegistry<SpriteAnimation> m_Animations;

		SpriteAnimation * m_CurrentAnimation;
		std::string m_CurrentAnimationName;
	};
}
