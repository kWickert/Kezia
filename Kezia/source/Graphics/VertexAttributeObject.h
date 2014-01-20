#pragma once

#include "Core/Assert.h"
#include "Core/Logger.h"

#include "Graphics/BufferObject.h"
#include "Graphics/glInclude.h"
#include "Graphics/glUtility.h"

#include "Math/Matrix4.h"

#include <iostream>

namespace Kezia
{
	class VertexAttributeObjectBase
	{
	public:
		virtual ~VertexAttributeObjectBase()
		{}

		virtual void Bind() = 0;
	
		virtual const std::type_info & GetDataTypeInfo() const = 0;
		virtual void SetBufferObject(BufferObjectBase * dataBuffer) = 0;
		virtual BufferObjectBase * GetBufferObject() = 0;

		virtual void EnableDivisor() = 0;
		virtual void DisableDivisor() = 0;

		virtual void SetDivisor(const U32 divisor) = 0;
	};

	template<typename Data_T>
	class VertexAttributeObject : public VertexAttributeObjectBase
	{
	public:
		VertexAttributeObject(const std::string & attributeName, const U32 programId)
			:	m_SurpressNoFind(false),
				m_AttributeName(attributeName),
				m_ProgramId(programId),
				m_BufferObject(nullptr),
				m_Divisor(-1)
		{}

		~VertexAttributeObject()
		{}

		virtual void Bind()
		{
			GLint location = glGetAttribLocation(m_ProgramId, m_AttributeName.c_str());

			if(location < 0)
			{
				if(!m_SurpressNoFind)
				{
					LOG("could not find attribute, " << m_AttributeName << ", it may have been compiled out");
					m_SurpressNoFind = true;
				}
			}
			else if(m_BufferObject == nullptr)
			{
				LOG("no data bound to attribute " << m_AttributeName);
			}
			else
			{
				m_BufferObject->Bind();

				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, glTranslator<Data_T>::DatumCount, glTranslator<Data_T>::EnumValue, GL_FALSE, 0, 0);

				if(m_Divisor > 0)
					glVertexAttribDivisor(location, static_cast<U32>(m_Divisor));
			}	
		}

		virtual const std::type_info & GetDataTypeInfo() const
		{
			return typeid(Data_T);
		}

		virtual void SetBufferObject(BufferObjectBase * dataBuffer)
		{
			KeziaAssert(dataBuffer->GetDataTypeInfo() == GetDataTypeInfo());

			if(dataBuffer != m_BufferObject)
			{
				//delete m_BufferObject;
				m_BufferObject = dataBuffer;
			}
		}

		virtual BufferObjectBase * GetBufferObject()
		{
			return m_BufferObject;
		}

		virtual void EnableDivisor()
		{
			m_Divisor = 1;
		}

		virtual void DisableDivisor()
		{
			m_Divisor = -1;
		}

		virtual void SetDivisor(const U32 divisor)
		{
			m_Divisor = divisor;
		}
	private:
		bool m_SurpressNoFind;
		S64 m_Divisor;

		const std::string m_AttributeName;
		const GLuint m_ProgramId;
		BufferObjectBase * m_BufferObject;
	};

	template<>
	inline void VertexAttributeObject< Matrix4<F32> >::Bind()
	{
		GLint location = glGetAttribLocation(m_ProgramId, m_AttributeName.c_str());

		if(location < 0)
		{
			if(!m_SurpressNoFind)
			{
				LOG("could not find attribute, " << m_AttributeName << ", it may have been compiled out");
				m_SurpressNoFind = true;
			}
		}
		else if(m_BufferObject == nullptr)
		{
			LOG("no data bound to attribute " << m_AttributeName);
		}
		else
		{
			m_BufferObject->Bind();

			for(U32 i = 0; i < 4; i++)
			{
				glEnableVertexAttribArray(location + i);
				glVertexAttribPointer(location + i, 4, glTranslator< Matrix4<F32> >::EnumValue, GL_FALSE, sizeof(Matrix4<F32>), reinterpret_cast<void *>(sizeof(F32) * i * 4));

				if(m_Divisor > 0)
					glVertexAttribDivisor(location + i, static_cast<U32>(m_Divisor));
			}
		}
	}

}
