#pragma once

#include "Core/Assert.h"
#include "Core/HashedRegistry.h"
#include "Core/Types.h"

#include "Graphics/Shader.h"
#include "Graphics/UniformObject.h"
#include "Graphics/VertexAttributeObject.h"

#include <map>

namespace Kezia
{
	class Material
	{
	public:
		Material(const std::string & name, const VertexShader & vertexShader, const FragmentShader & fragmentShader);
		virtual ~Material();

		virtual void Bind();
		static void UnbindAllMaterials();
		
		template<typename Uniform_T>
		void CreateUniform(const std::string & uniformName)
		{
			auto findResult = m_Uniforms.Find(uniformName);

			if(findResult == m_Uniforms.End())
				m_Uniforms.Insert(uniformName, new UniformObject<Uniform_T>(uniformName, m_ProgramId));
			else
				LOG("uniform, " << uniformName <<", already exists!");
		}

		template<typename Attribute_T>
		void CreateAttribute(const std::string & attributeName)
		{
			auto findResult = m_Attributes.Find(attributeName);

			if(findResult == m_Attributes.End())
				m_Attributes.Insert(attributeName, new VertexAttributeObject<Attribute_T>(attributeName, m_ProgramId));
			else
				LOG("uniform, " << attributeName <<", already exists!");
		}

		virtual UniformObjectBase * GetUniform(const std::string & uniformName);
		virtual VertexAttributeObjectBase * GetAttribute(const std::string & attributeName);

		template<typename Uniform_T>
		void SetUniformValue(const std::string & uniformName, const Uniform_T & value, bool ignoreTypeCheck = false);

		template<typename Attribute_T>
		void UpdateAttributeData(const std::string & attributeName, Attribute_T * attributes, const U32 attributeCount);

		inline const std::string & GetName() const;

		static inline Material * GetCurrentlyBoundMaterial();
	protected:
		static Material * k_CurrentlyBoundMaterial;

	private:
		std::string m_Name;

		//VariableObject_T is a template argument, inside CreateNewVariableObject specializations
		//of that template are created and returned as VariableObjectBase_T pointers
		template<class VariableObjectBase_T, template<class> class VariableObject_T>
		void PopulateVariables(Shader::VariableIterator begin, Shader::VariableIterator end, HashedRegistry<VariableObjectBase_T *> & outVariables);

		template<class VariableObjectBase_T, template<class> class VariableObject_T>
		VariableObjectBase_T * CreateNewVariableObject(const std::string & type, const std::string & name, const U32 programId);

		HashedRegistry<UniformObjectBase *> m_Uniforms;
		HashedRegistry<VertexAttributeObjectBase *> m_Attributes;

		U32 m_ProgramId;
	};

	template<typename Uniform_T>
	void Material::SetUniformValue(const std::string & uniformName, const Uniform_T & value, bool ignoreTypeCheck)
	{
		UniformObjectBase * uniform = GetUniform(uniformName);

		if(!ignoreTypeCheck)
			KeziaAssert(uniform->GetDataTypeInfo() == typeid(Uniform_T));

		reinterpret_cast< UniformObject<Uniform_T> * >(uniform)->SetValue(value);
	}

	template<typename Attribute_T>
	void Material::UpdateAttributeData(const std::string & attributeName, Attribute_T * attributes, const U32 attributeCount)
	{
		VertexAttributeObject<Attribute_T> * vaObject = dynamic_cast< VertexAttributeObject<Attribute_T> *>(GetAttribute(attributeName));

		if(vaObject == nullptr)
		{
			LOG("couldn't get vertex attribute object");
			return;
		}

		BufferObjectBase * bInterface = vaObject->GetBufferObject();

		if(bInterface == nullptr)
		{
			vaObject->SetBufferObject(new BufferObject<Attribute_T, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW>(attributes, attributeCount));
		}

		BufferObject<Attribute_T, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW> * bObject = dynamic_cast< BufferObject<Attribute_T, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW> *>(vaObject->GetBufferObject());

		if(bObject == nullptr)
		{
			LOG("couldn't get buffer object");
			return;
		}

		bObject->UpdateData(attributes, attributeCount);
	}

	template<class VariableObjectBase_T, template<class> class VariableObject_T>
	void Material::PopulateVariables(Shader::VariableIterator begin, Shader::VariableIterator end, HashedRegistry<VariableObjectBase_T *> & outVariables)
	{
		for(auto variable = begin; variable != end; ++variable)
		{
			const Shader::VariableInfo & vInfo = *variable;

			auto findResult = m_Uniforms.Find(vInfo.name);

			if(findResult == m_Uniforms.End())
			{
				VariableObjectBase_T * newVariable = CreateNewVariableObject<VariableObjectBase_T, VariableObject_T>(vInfo.type, vInfo.name, m_ProgramId);

				KeziaAssert(newVariable != nullptr);

				outVariables.Insert(vInfo.name, newVariable);
			}
		}
	}

	template<class VariableObjectBase_T, template<class> class VariableObject_T>
	VariableObjectBase_T * Material::CreateNewVariableObject(const std::string & type, const std::string & name, const U32 programId)
	{
		const U32 typeHash = SuperFastHash(type);

		VariableObjectBase_T * variable = nullptr;

		if(typeHash == glslTypeHashTable::i)
			variable = new VariableObject_T<U32>(name, programId);
		else if(typeHash == glslTypeHashTable::f)
			variable = new VariableObject_T<F32>(name, programId);
		else if(typeHash == glslTypeHashTable::sampler2D)
			variable = new VariableObject_T<Texture>(name, programId);
		else if(typeHash == glslTypeHashTable::vec2)
			variable = new VariableObject_T< Vector2<F32> >(name, programId);
		else if(typeHash == glslTypeHashTable::vec3)
			variable = new VariableObject_T< Vector3<F32> >(name, programId);
		else if(typeHash == glslTypeHashTable::vec4)
			variable = new VariableObject_T< Vector4<F32> >(name, programId);
		else if(typeHash == glslTypeHashTable::mat4)
			variable = new VariableObject_T< Matrix4<F32> >(name, programId);

		return variable;
	}

	inline Material * Material::GetCurrentlyBoundMaterial()
	{
		return k_CurrentlyBoundMaterial;
	}

	inline const std::string & Material::GetName() const
	{
		return m_Name;
	}

}