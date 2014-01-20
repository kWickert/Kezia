#pragma once

#include "Core/Logger.h"
#include "Core/Types.h"

#include "Graphics/glInclude.h"

#include "Core/WindowsInclude.h"

#include <map>

namespace Kezia
{
	class BufferObjectBase
	{
	public:
		virtual ~BufferObjectBase()
		{}

		virtual void Bind() = 0;

		virtual const U32 GetSize() const = 0;
		virtual const type_info & GetDataTypeInfo() const = 0;

		static BufferObjectBase * GetCurrentlyBoundBufferObject()
		{
			return k_CurrentlyBoundBufferObject;
		}
	protected:
		static BufferObjectBase * k_CurrentlyBoundBufferObject;
	};

	template<typename Data_T, U32 Buffer_T = GL_ARRAY_BUFFER, U32 Usage_T = GL_STATIC_DRAW>
	class BufferObject : public BufferObjectBase
	{
	public:
		BufferObject(const Data_T * data, U32 dataSize)
			:	m_DataBufferId(0),
				m_DataSize(dataSize)
		{
			glGenBuffers(1, &m_DataBufferId);
			Bind();

			glBufferData(Buffer_T, dataSize * sizeof(Data_T), data, Usage_T);

			k_ReferenceCounts[m_DataBufferId]++;
		}

		BufferObject(const BufferObject & other)
			:	m_DataBufferId(other.m_DataBufferId),
				m_DataSize(other.m_DataSize)
		{
			k_ReferenceCounts[m_DataBufferId]++;
		}

		virtual ~BufferObject()
		{
			k_ReferenceCounts[m_DataBufferId]--;

			if(k_ReferenceCounts[m_DataBufferId] == 0)
				glDeleteBuffers(1, &m_DataBufferId);
		}

		virtual void Bind()
		{
			if(k_CurrentlyBoundBufferObject != this)
			{
				glBindBuffer(Buffer_T, m_DataBufferId);
				k_CurrentlyBoundBufferObject = this;
			}
		}

		void UpdateData(const Data_T * data, U32 dataSize)
		{
			if(Usage_T == GL_STATIC_DRAW)
				LOG("warning: updating static buffer, consider using dynamic buffer instead");

			Bind();
			glBufferData(Buffer_T, dataSize * sizeof(Data_T), data, Usage_T);
		}

		virtual const U32 GetSize() const
		{
			return m_DataSize;
		}

		virtual const type_info & GetDataTypeInfo() const
		{
			return typeid(Data_T);
		}
	private:
		static std::map<U32, U32> k_ReferenceCounts;

		U32 m_DataBufferId;
		const U32 m_DataSize;
	};

	template<typename Data_T, U32 Buffer_T, U32 Usage_T>
	std::map<U32, U32> BufferObject<Data_T, Buffer_T, Usage_T>::k_ReferenceCounts;
}
