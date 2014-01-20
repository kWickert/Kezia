#pragma once

#include "NetworkAddress.h"

#include "Core/Assert.h"
#include "Core/HashedRegistry.h"
#include "Core/Logger.h"
#include "Core/Types.h"

namespace Kezia
{
	class PacketTemplate
	{
	public:
		~PacketTemplate();

		const std::string & GetTemplateName() const;

		const U32 GetPacketSize() const;

		const type_info & GetFieldType(const std::string & fieldName) const;		
		const U32 GetFieldOffset(const std::string & fieldName) const;

		bool FieldExists(const std::string & fieldName) const;
		
		template<typename Field_T>
		void SetFieldType(const std::string & fieldName);

		static PacketTemplate * CreatePacketTemplate(const std::string & templateName, bool guaranteeDelivery);
		static PacketTemplate * GetPacketTemplate(const std::string & templateName);

		friend class Packet;
	private:
		PacketTemplate(const std::string & templateName, const U32 templateID, bool guaranteeDelivery);

		static bool InitPacketTemplates();

		static HashedRegistry<PacketTemplate> k_PacketTemplates;

		struct FieldInfo
		{
			FieldInfo(const type_info & type, const U32 offset)
				:	type(type), 
					offset(offset)
			{}

			const type_info & type;
			const U32 offset;
		};

		const std::string m_TemplateName;
		U32 m_TemplateID;

		bool m_IsGuaranteed;

		U32 m_CurrentOffset;

		HashedRegistry<FieldInfo> m_Fields;
	};

	template<typename Field_T>
	void PacketTemplate::SetFieldType(const std::string & fieldName)
	{
		m_Fields.Insert(fieldName, FieldInfo(typeid(Field_T), m_CurrentOffset));

		m_CurrentOffset += sizeof(Field_T);
	}

	class Packet
	{
	public:
		Packet();
		Packet(const std::string & templateName);
		Packet(const Packet & other);
		
		~Packet();

		const std::string & GetTemplateName() const;
		const NetworkAddress & GetSenderAddress() const;

		bool IsGuaranteed() const;
		const U32 GetPacketID() const;

		const U32 Serialize(char * buffer) const;
		const U32 Deserialize(const char * buffer);

		template<typename Value_T>
		void SetFieldValue(const std::string & fieldName, const Value_T & value);

		template<typename Value_T>
		void GetFieldValue(const std::string & fieldName, Value_T & outValue) const;

		bool HasField(const std::string & fieldName) const;

		friend class WindowsSocket;
	private:	
		static const U32 k_PacketSize = 512;
		static HashedRegistry<PacketTemplate> k_PacketTemplates;

		template<typename Value_T>
		char * WriteToBuffer(char * buffer, const Value_T & value, U32 & outBytesWritten) const;

		template<typename Value_T>
		const char * ReadFromBuffer(const char * buffer, Value_T & outValue, U32 & outBytesRead) const;

		char m_Data[k_PacketSize];
		NetworkAddress m_SenderAddress;

		const PacketTemplate * m_PacketTemplate;
	};

	template<typename Value_T>
	void Packet::SetFieldValue(const std::string & fieldName, const Value_T & value)
	{	
		const type_info & expected = m_PacketTemplate->GetFieldType(fieldName);

		char * tmp = m_Data + m_PacketTemplate->GetFieldOffset(fieldName);

		if(expected == typeid(Value_T))
		{
			memcpy(tmp, &value, sizeof(Value_T));
		}
		else
		{
			LOG("SetFieldValue: incorrect type provided for field " << fieldName << ", expected type is " << expected.name());
		}
	}

	template<typename Value_T>
	void Packet::GetFieldValue(const std::string & fieldName, Value_T & value) const
	{
		const type_info & expected = m_PacketTemplate->GetFieldType(fieldName);

		const char * tmp = m_Data + m_PacketTemplate->GetFieldOffset(fieldName);

		if(expected == typeid(Value_T))
		{
			memcpy(&value, tmp, sizeof(Value_T));
		}
		else
		{
			LOG("GetFieldValue: incorrect type provided for field " << fieldName << ", expected type is " << expected.name());
		}
	}

	template<typename Value_T>
	char * Packet::WriteToBuffer(char * buffer, const Value_T & value, U32 & outBytesWritten) const
	{
		outBytesWritten = sizeof(Value_T);
		memcpy(buffer, &value, outBytesWritten);

		return buffer + outBytesWritten;
	}

	template<typename Value_T>
	const char * Packet::ReadFromBuffer(const char * buffer, Value_T & outValue, U32 & outBytesRead) const
	{
		outBytesRead = sizeof(Value_T);
		memcpy(&outValue, buffer, outBytesRead);

		return buffer + outBytesRead;
	}
}