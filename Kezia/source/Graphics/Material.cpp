#include "Graphics/Material.h"

namespace Kezia
{
	Material * Material::k_CurrentlyBoundMaterial = nullptr;

	Material::Material(const std::string & name, const VertexShader & vertexShader, const FragmentShader & fragmentShader)
		:	m_Name(name)
	{
		m_ProgramId = glCreateProgram();

		glAttachShader(m_ProgramId, vertexShader.GetShaderId());
		glAttachShader(m_ProgramId, fragmentShader.GetShaderId());

		glLinkProgram(m_ProgramId);

		GLint linkStatus;
		glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &linkStatus);

		//link failure
		if(!linkStatus)
		{
			int logLength;
			glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &logLength);

			char * log = new char[logLength];
			glGetProgramInfoLog(m_ProgramId, logLength, &logLength, log);

			MessageBoxA(NULL, log, "Error Linking Shader Program", MB_OK);

			delete [] log;

			exit(-1);
		}

		PopulateVariables<UniformObjectBase, UniformObject>(vertexShader.UniformBegin(), vertexShader.UniformEnd(), m_Uniforms);
		PopulateVariables<UniformObjectBase, UniformObject>(fragmentShader.UniformBegin(), fragmentShader.UniformEnd(), m_Uniforms);

		PopulateVariables<VertexAttributeObjectBase, VertexAttributeObject>(vertexShader.AttributeBegin(), vertexShader.AttributeEnd(), m_Attributes);
	}

	Material::~Material()
	{
		for(auto it = m_Attributes.Begin(); it != m_Attributes.End(); ++it)
		{
			delete it->second;
		}

		for(auto it = m_Uniforms.Begin(); it != m_Uniforms.End(); ++it)
		{
			delete it->second;
		}
	}

	void Material::Bind()
	{
		glUseProgram(m_ProgramId);

		glTextureUnitManager::ResetTextureBindings();

		k_CurrentlyBoundMaterial = this;

		for(auto uniform = m_Uniforms.Begin(); uniform != m_Uniforms.End(); ++uniform)
			uniform->second->Bind();

		for(auto attribute = m_Attributes.Begin(); attribute != m_Attributes.End(); ++attribute)
			attribute->second->Bind();
	}

	void Material::UnbindAllMaterials()
	{
		glUseProgram(0);

		glTextureUnitManager::ResetTextureBindings();

		k_CurrentlyBoundMaterial = nullptr;
	}

	UniformObjectBase * Material::GetUniform(const std::string & uniformName)
	{
		auto findResult = m_Uniforms.Find(uniformName);

		UniformObjectBase * uniform = nullptr;

		if(findResult != m_Uniforms.End())
			uniform = findResult->second;

		return uniform;
	}

	VertexAttributeObjectBase * Material::GetAttribute(const std::string & attributeName)
	{
		auto findResult = m_Attributes.Find(attributeName);

		VertexAttributeObjectBase * attribute = nullptr;

		if(findResult != m_Attributes.End())
			attribute = findResult->second;

		return attribute;
	}
}