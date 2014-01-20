#pragma once

#include "Core/Logger.h"

#include "Graphics/glUtility.h"
#include "Graphics/Material.h"

#include <iostream>

namespace Kezia
{
	class UniformObjectBase
	{
	public:
		virtual ~UniformObjectBase()
		{}

		virtual void Bind() = 0;
		virtual const type_info & GetDataTypeInfo() const = 0;
	};

	template<typename Data_T>
	class UniformObject : public UniformObjectBase
	{
	public:
		UniformObject(const std::string & uniformName, const U32 programId)
			:	m_SurpressNoFind(false),
				m_UniformName(uniformName),
				m_ProgramId(programId),
				m_Value()
		{}

		virtual ~UniformObject()
		{}

		virtual void Bind()
		{
			GLint location = glGetUniformLocation(m_ProgramId, m_UniformName.c_str());

			if(location < 0)
			{
				if(!m_SurpressNoFind)
				{
					LOG("could not find attribute, " << m_UniformName << ", it may have been compiled out");
					m_SurpressNoFind = true;
				}
			}
			else
			{
				glTranslator<Data_T>::BindUniform(location, m_Value);
			}
		}

		virtual const type_info & GetDataTypeInfo() const
		{
			return typeid(Data_T);
		}

		void SetValue(const Data_T & value)
		{
			m_Value = value;
		}
	private:
		bool m_SurpressNoFind;

		const std::string m_UniformName;
		const U32 m_ProgramId;

		Data_T m_Value;
	};
}