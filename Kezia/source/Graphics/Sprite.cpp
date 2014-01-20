#include "Sprite.h"

#include "Core/Logger.h"
#include "Core/IResourceManager.h"

#include "Math/MathUtility.h"

namespace Kezia
{
	SpriteAnimation::SpriteAnimation(const XmlNode & node)
		:	m_IsPlaying(false),
			m_IsLooping(false),
			m_ElapsedTime(0.0),
			m_FrameDuration(0.1)
	{
		node.ValidateAttributes("texture,layoutType", "frameWidth,frameHeight");

		const std::string texturePath = GetAttribute<std::string>(node, "texture","blank.png");
		m_Texture = Texture(texturePath);

		node.ValidateChildElements("Frame", "");

		const std::string type = GetAttribute<std::string>(node, "layoutType", "manual");

		XmlNode frameNode = node.GetFirstChildNode();
		if(type == "manual")
		{
			Vector2<F32> ul, br;
			while(!frameNode.IsEmpty())
			{
				frameNode.ValidateAttributes("x,y,w,h","");

				ul.x() = GetAttribute<F32>(frameNode, "x", 0.0f);
				ul.y() = GetAttribute<F32>(frameNode, "y", 0.0f);
				br.x() = GetAttribute<F32>(frameNode, "w", 0.0f);
				br.y() = GetAttribute<F32>(frameNode, "h", 0.0f);
				br += ul;

				AddFrame(ul, br);

				frameNode.GetNextSiblingNode();
			}
		}
		else if(type == "grid")
		{
			F32 width = GetAttribute(node, "frameWidth", 1.0f);
			F32 height = GetAttribute(node, "frameHeight", 1.0f);

			while(!frameNode.IsEmpty())
			{
				frameNode.ValidateAttributes("x,y", "");

				U32 x = GetAttribute<U32>(frameNode, "x", 0);
				U32 y = GetAttribute<U32>(frameNode, "y", 0);

				AddFrame(Vector2<F32>(width * x, height * y), Vector2<F32>(width * (x + 1), height * (y + 1)));

				frameNode.GetNextSiblingNode();
			}
		}
		else
		{
			LOG("unrecognized layout type");
		}
	}

	SpriteAnimation::SpriteAnimation(const Texture & texture)
		:	m_IsPlaying(false),
		 	m_IsLooping(false),
		 	m_ElapsedTime(0.0),
		 	m_FrameDuration(0.1),
		 	m_Texture(texture)
	{}

	SpriteAnimation::~SpriteAnimation()
	{}

	void SpriteAnimation::Draw(const Camera * camera)
	{
		static Material * spriteMaterial = CreateOrGetSpriteMaterial();

		if(!m_Frames.empty())
		{
			spriteMaterial->SetUniformValue("u_ModelViewProjectionMatrix", Matrix4<F32>::Translate(0, 0, 0));

			spriteMaterial->SetUniformValue("u_SpriteColor", Color::White);

			spriteMaterial->SetUniformValue("u_SpriteTexture", m_Texture);

			U32 currentFrameIndex = static_cast<U32>(m_ElapsedTime / m_FrameDuration);

			Frame & currentFrame = m_Frames[currentFrameIndex];

			Vector2<F32> textureCoords[4] =
			{
					Vector2<F32>(currentFrame.ul.x(), currentFrame.br.y()),	 	//bl
					currentFrame.br,
					currentFrame.ul,
					Vector2<F32>(currentFrame.br.x(), currentFrame.ul.y()) 		//ur
			};

			TextureCoordinateBufferObject * bufferObject = dynamic_cast< TextureCoordinateBufferObject * >(spriteMaterial->GetAttribute("a_TextureCoordinates")->GetBufferObject());

			if(bufferObject != nullptr)
			{
				bufferObject->UpdateData(textureCoords, 4);

				spriteMaterial->Bind();

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
			else
			{
				LOG("couldn't get texture coordinates buffer object");
			}
		}
	}

	void SpriteAnimation::Update(F64 deltaTime)
	{
		if(m_IsPlaying)
		{
			m_ElapsedTime += deltaTime;

			U32 totalFrames = m_Frames.size();
			F64 totalTime = totalFrames * m_FrameDuration;
			U32 currentFrame = static_cast<U32>(m_ElapsedTime / m_FrameDuration);

			if(currentFrame > totalFrames - 1 && m_IsLooping)
			{
				m_ElapsedTime = Mod(m_ElapsedTime, totalTime);
			}
		}
	}

	void SpriteAnimation::Play(bool loop)
	{
		m_ElapsedTime = 0.0;
		m_IsPlaying = true;
		m_IsLooping = loop;
	}

	void SpriteAnimation::AddFrame(const Vector2<F32> & ul, const Vector2<F32> & br)
	{
		Frame frame;
		frame.ul = ul;
		frame.br = br;

		m_Frames.push_back(frame);
	}

	SpriteAnimation::PositionBufferObject * SpriteAnimation::CreateOrGetPositions()
	{
		Vector3<F32> positionData[4] =
		{
			Vector3<F32>(-1, -1, 0),
			Vector3<F32>(1, -1, 0),
			Vector3<F32>(-1, 1, 0),
			Vector3<F32>(1, 1, 0)
		};

		BufferObject< Vector3<F32>, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW> * pBuffer = new BufferObject< Vector3<F32>, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW>(positionData, 4);

		return pBuffer;
	}

	SpriteAnimation::TextureCoordinateBufferObject * SpriteAnimation::CreateOrGetTextureCoordinates()
	{
		Vector2<F32> textureCoords[4] =
		{
			Vector2<F32>(0.0f, 0.5f),
			Vector2<F32>(0.5f, 0.5f),
			Vector2<F32>(0.0f, 0.0f),
			Vector2<F32>(0.5f, 0.0f)
		};

		TextureCoordinateBufferObject * tcBuffer = new TextureCoordinateBufferObject(textureCoords, 4);

		return tcBuffer;
	}

	Material * SpriteAnimation::CreateOrGetSpriteMaterial()
	{
		static PositionBufferObject * positionData = CreateOrGetPositions();
		static TextureCoordinateBufferObject * textureCoordinateData = CreateOrGetTextureCoordinates();

		Material * spriteMaterial = nullptr;

		if(g_Renderer != nullptr)
		{
			spriteMaterial = reinterpret_cast<Material *>(g_Renderer->GetMaterial("Sprite"));
		}
		else
		{
			FileResource * vFile = dynamic_cast<FileResource *>(g_FileResourceManager->OpenResourceFile("shaders/Sprite.vertex.glsl"));

			if(vFile == nullptr)
			{
				LOG("couldn't cast to file resource");
				exit(-1);
			}

			VertexShader vs(vFile->GetByteData());


			FileResource * fFile = dynamic_cast<FileResource *>(g_FileResourceManager->OpenResourceFile("shaders/Sprite.fragment.glsl"));

			if(fFile == nullptr)
			{
				LOG("couldn't cast to file resource");
				exit(-1);
			}

			FragmentShader fs(fFile->GetByteData());

			spriteMaterial = new Material("Sprite", vs, fs);

			spriteMaterial->CreateUniform<Color>("u_SpriteColor");

			spriteMaterial->CreateUniform<Texture>("u_SpriteTexture");

			spriteMaterial->CreateUniform< Matrix4<F32> >("u_ModelViewProjectionMatrix");

			spriteMaterial->CreateAttribute< Vector3<F32> >("a_Position");
			spriteMaterial->CreateAttribute< Vector2<F32> >("a_TextureCoordinates");

			spriteMaterial->GetAttribute("a_Position")->SetBufferObject(positionData);
			spriteMaterial->GetAttribute("a_TextureCoordinates")->SetBufferObject(textureCoordinateData);
		}

		return spriteMaterial;
	}

	Sprite::Sprite(const XmlNode & node)
		:	m_CurrentAnimation(nullptr)
	{
		node.ValidateChildElements("Animation", "");

		XmlNode animationNode = node.GetFirstChildNode();

		while(!animationNode.IsEmpty())
		{
			animationNode.ValidateAttributes("name","");

			const std::string animationName = GetAttribute<std::string>(node, "name", "");
			
			m_Animations.Insert(animationName, SpriteAnimation(animationNode));
		}
	}

	Sprite::Sprite(const Sprite & other)
		:	m_Animations(other.m_Animations)
	{
		m_CurrentAnimationName = other.m_CurrentAnimationName;
		m_CurrentAnimation = &m_Animations.Find(m_CurrentAnimationName)->second;
	}

	Sprite::~Sprite()
	{}

	void Sprite::Draw(const Camera * camera)
	{
		if(m_CurrentAnimation != nullptr)
		{
			m_CurrentAnimation->Draw(camera);
		}
	}

	void Sprite::Update(F64 deltaTime)
	{
		if(m_CurrentAnimation != nullptr)
		{
			m_CurrentAnimation->Update(deltaTime);
		}
	}

	void Sprite::Play(const std::string & animationName, bool loop)
	{
		auto findResult = m_Animations.Find(animationName);

		if(findResult != m_Animations.End())
		{
			m_CurrentAnimation = &findResult->second;
			m_CurrentAnimation->Play(loop);
		}
		else
		{
			LOG("couldn't find animation with name " << animationName);
		}
	}

	void Sprite::AddAnimation(const std::string & animationName, const Texture & animationSheet)
	{
		auto findResult = m_Animations.Find(animationName);

		if(findResult == m_Animations.End())
		{
			m_Animations.Insert(animationName, SpriteAnimation(animationSheet));
		}
		else
		{
			LOG("animation with name " << animationName << " already exists!");
		}
	}

	SpriteAnimation * Sprite::GetAnimation(const std::string & animationName)
	{
		auto findResult = m_Animations.Find(animationName);

		SpriteAnimation * animation = nullptr;
		if(findResult != m_Animations.End())
		{
			animation = &findResult->second;
		}

		return animation;
	}
}




