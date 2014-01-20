#include "OpenGL_Renderer.h"

#include "Core/IResource.h"
#include "Core/StringUtilities.h"
#include "Core/FileResource.h"
#include "Core/WindowsInclude.h"
#include "Core/Windows_ResourceManager.h"

#include "Graphics/glInclude.h"
#include "Graphics/BaseRenderPass.h"
#include "Graphics/RenderableComponent.h"
#include "Graphics/Material.h"
#include "Graphics/RenderOperation.h"
#include "Graphics/Shader.h"

namespace
{
	bool IsTechniqueEnabled(GLenum technique)
	{
		GLboolean isEnabled;
		glGetBooleanv(technique, &isEnabled);

		return static_cast<bool>(isEnabled != GL_FALSE);
	}
}

namespace Kezia
{
	struct MaterialComponents
	{
		MaterialComponents(const std::string & name)
			:	name(name),
				vs(nullptr),
				fs(nullptr)
		{}

		~MaterialComponents()
		{
			delete vs;
			delete fs;
		}

		std::string name;
		VertexShader * vs;
		FragmentShader * fs;
	};

	OpenGL_Renderer::OpenGL_Renderer()
	{
		glewInit();

		std::vector<FileResource *> shaderFiles;

		Windows_ResourceManager * winFileMgr = dynamic_cast<Windows_ResourceManager *>(g_FileResourceManager);

		if(winFileMgr != nullptr)
		{
			winFileMgr->LoadDirectoryContents(g_FileResourceManager->GetWorkingDirectory() + "\\data\\shaders", true, shaderFiles);
		}

		std::string filePath;
		std::string shaderName;
		std::vector<std::string> tokens;
		tokens.reserve(3);

		HashedRegistry<MaterialComponents> shaders;

		for(auto it = shaderFiles.begin(); it != shaderFiles.end(); ++it)
		{
			FileResource * fileResource = *it;

			//expected format shaderName.shaderType.glsl
			tokens.clear();

			const std::string & path = fileResource->GetFilePath();
			ParseList(path.substr(path.find_last_of('\\') + 1), '.', tokens);

			//shader name
			shaderName = tokens[0];

			//file extension should be glsl
			if(tokens.back() != "glsl")
				continue;

			//shader type
			if(shaders.Find(shaderName) == shaders.End())
			{
				shaders.Insert(shaderName, MaterialComponents(shaderName));
			}

			if(tokens[1] == "vertex")
			{
				shaders.Find(shaderName)->second.vs = new VertexShader(fileResource->GetByteData());	
			}
			else if(tokens[1] == "fragment")
			{
				shaders.Find(shaderName)->second.fs = new FragmentShader(fileResource->GetByteData());
			}
			else
			{
				//don't know what the shader type is
			}
		}
		
		for(auto it = shaders.Begin(); it != shaders.End(); ++it)
		{
			MaterialComponents & components = it->second;

			if(components.vs != nullptr && components.fs != nullptr)
				m_Materials.Insert(components.name, new Material(components.name, *components.vs, *components.fs));
		}

		SetClearColor(Color(0.15f, 0.15f, 0.3f, 1.0f));
		EnableAlphaBlending();
		EnableDepthTesting();
		EnableTexturing();
	}

	OpenGL_Renderer::~OpenGL_Renderer()
	{
		for(auto it = m_Materials.Begin(); it != m_Materials.End(); ++it)
		{
			delete it->second;
		}
	}

	Material * OpenGL_Renderer::GetMaterial(const std::string & name)
	{
		auto findResult = m_Materials.Find(name);

		if(findResult == m_Materials.End())
			return nullptr;
		else
			return findResult->second;
	}

	void OpenGL_Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL_Renderer::Draw()
	{
		for(auto it = m_RenderPasses.begin(); it != m_RenderPasses.end(); ++it)
		{
			(*it)->PreRender(nullptr);
			(*it)->Render(this);
			(*it)->PostRender();
		}

		//this needs to be called externally
		//eg. rendering to viewports
		//m_Application->SwapBuffers();
	}

	void OpenGL_Renderer::Update(const F64 deltaTime)
	{
		for(auto renderable = m_RenderableComponents.begin(); renderable != m_RenderableComponents.end(); ++renderable)
		{
			(*renderable)->Update(deltaTime);
		}
	}

	void OpenGL_Renderer::AddRenderableComponent(RenderableComponent * component)
	{
		m_RenderableComponents.push_back(component);
	}

	void OpenGL_Renderer::RemoveRenderableComponent(RenderableComponent * component)
	{
		for(auto renderable = m_RenderableComponents.begin(); renderable != m_RenderableComponents.end();)
		{
			if(component == *renderable)
			{
				renderable = m_RenderableComponents.erase(renderable);
			}
			else
			{
				++renderable;
			}
		}
	}

	void OpenGL_Renderer::AddRenderOperation(RenderOperation * operation)
	{
		m_RenderOperations.push_back(operation);
	}

	void OpenGL_Renderer::RemoveRenderOperation(RenderOperation * operation)
	{
		auto findResult = std::find(m_RenderOperations.begin(), m_RenderOperations.end(), operation);
		
		m_RenderOperations.erase(findResult);
	}

	void OpenGL_Renderer::ExecuteRenderOperations()
	{
		for(auto it = m_RenderOperations.begin(); it != m_RenderOperations.end(); ++it)
		{
			(*it)->Execute();
		}
	}

	OpenGL_Renderer::ConstRenderableIterator OpenGL_Renderer::RenderableBegin() const
	{
		return m_RenderableComponents.begin();
	}

	OpenGL_Renderer::ConstRenderableIterator OpenGL_Renderer::RenderableEnd() const
	{
		return m_RenderableComponents.end();
	}

	void OpenGL_Renderer::AddRenderPass(BaseRenderPass * renderPass)
	{
		m_RenderPasses.push_back(renderPass);
	}

	const Viewport OpenGL_Renderer::GetCurrentViewport() const
	{
		Viewport viewport;

		glGetIntegerv(GL_VIEWPORT, &viewport.x);

		return viewport;
	}

	void OpenGL_Renderer::SetViewport(S32 x, S32 y, S32 width, S32 height)
	{
		glViewport(x, y, width, height);
	}

	bool OpenGL_Renderer::IsAlphaBlendingEnabled() const
	{
		return IsTechniqueEnabled(GL_BLEND);
	}

	void OpenGL_Renderer::EnableAlphaBlending()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGL_Renderer::DisableAlphaBlending()
	{
		glDisable(GL_BLEND);
	}

	bool OpenGL_Renderer::IsDepthTestingEnabled() const
	{
		return IsTechniqueEnabled(GL_DEPTH_TEST);
	}

	void OpenGL_Renderer::EnableDepthTesting()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGL_Renderer::DisableDepthTesting()
	{
		glDisable(GL_DEPTH_TEST);
	}

	bool OpenGL_Renderer::IsTexturingEnabled() const
	{
		return IsTechniqueEnabled(GL_TEXTURE_2D);
	}

	void OpenGL_Renderer::EnableTexturing()
	{
		glEnable(GL_TEXTURE_2D);
	}

	void OpenGL_Renderer::DisableTexturing()
	{
		glDisable(GL_TEXTURE_2D);
	}

	void OpenGL_Renderer::EnableWireframeMode()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void OpenGL_Renderer::DisableWireframeMode()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void OpenGL_Renderer::SetClearColor(const Color & color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
}
