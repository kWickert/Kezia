#include "Packet.h"

#include "Core/Time.h"

namespace Kezia
{
	HashedRegistry<PacketTemplate> PacketTemplate::k_PacketTemplates(HashedRegistry<PacketTemplate>::CaseInsensitiveHash);

	#pragma region PacketTemplate
	PacketTemplate::PacketTemplate(const std::string & templateName, const U32 templateID, bool guaranteeDelivery)
		:	m_TemplateName(templateName),
			m_TemplateID(templateID),
			m_IsGuaranteed(guaranteeDelivery),
			m_CurrentOffset(sizeof(m_TemplateID))
	{
		SetFieldType<U32>("packetID");
		SetFieldType<TimeStamp>("timeStamp");
	}

	PacketTemplate::~PacketTemplate()
	{}

	const std::string & PacketTemplate::GetTemplateName() const
	{
		return m_TemplateName;
	}

	const U32 PacketTemplate::GetPacketSize() const
	{
		return m_CurrentOffset;
	}

	const type_info & PacketTemplate::GetFieldType(const std::string & fieldName) const
	{
		auto findResult = m_Fields.Find(fieldName);

		KeziaAssert(findResult != m_Fields.End());

		return findResult->second.type;
	}

	const U32 PacketTemplate::GetFieldOffset(const std::string & fieldName) const
	{
		auto findResult = m_Fields.Find(fieldName);

		KeziaAssert(findResult != m_Fields.End());

		return findResult->second.offset;
	}

	bool PacketTemplate::FieldExists(const std::string & fieldName) const
	{
		return m_Fields.Find(fieldName) != m_Fields.End();
	}

	PacketTemplate * PacketTemplate::CreatePacketTemplate(const std::string & templateName, bool guaranteeDelivery)
	{
		static bool initResult = InitPacketTemplates();

		PacketTemplate * result = nullptr;

		auto findResult = k_PacketTemplates.Find(templateName);

		if(findResult == k_PacketTemplates.End())
		{
			result = &k_PacketTemplates.Insert(templateName, PacketTemplate(templateName, k_PacketTemplates.Size(), guaranteeDelivery)).first->second;
		}

		return result;
	}

	PacketTemplate * PacketTemplate::GetPacketTemplate(const std::string & templateName)
	{
		PacketTemplate * result = nullptr;

		auto findResult = k_PacketTemplates.Find(templateName);

		if(findResult != k_PacketTemplates.End())
		{
			result = &findResult->second;
		}

		return result;
	}

	bool PacketTemplate::InitPacketTemplates()
	{
		PacketTemplate * packetTemplate = CreatePacketTemplate("InvalidPacket", false);

		packetTemplate = CreatePacketTemplate("Ping", false);
		packetTemplate = CreatePacketTemplate("PingReply", false);
		packetTemplate->SetFieldType<TimeStamp>("sourcePacketSendTime");
		
		packetTemplate = CreatePacketTemplate("ACK", false);
		packetTemplate->SetFieldType<U32>("sourcePacketID");

		return true;
	}
	#pragma endregion

	#pragma region Packet
	Packet::Packet()
		:	m_PacketTemplate(nullptr)
	{}

	Packet::Packet(const std::string & templateName)
		:	m_PacketTemplate(PacketTemplate::GetPacketTemplate(templateName))
	{
		U32 packetTemplateID = m_PacketTemplate->m_TemplateID;

		memcpy(&m_Data, &packetTemplateID, sizeof(packetTemplateID));
	}

	Packet::Packet(const Packet & other)
		:	m_PacketTemplate(other.m_PacketTemplate)
	{
		memcpy(&m_Data, other.m_Data, k_PacketSize);
	}

	Packet::~Packet()
	{}

	const std::string & Packet::GetTemplateName() const
	{
		return m_PacketTemplate->GetTemplateName();
	}

	const NetworkAddress & Packet::GetSenderAddress() const
	{
		return m_SenderAddress;
	}

	bool Packet::IsGuaranteed() const
	{
		return m_PacketTemplate->m_IsGuaranteed;
	}

	const U32 Packet::GetPacketID() const
	{
		U32 packetID;

		GetFieldValue("packetID", packetID);

		return packetID;
	}

	const U32 Packet::Serialize(char * buffer) const
	{
		const U32 packetSize = m_PacketTemplate->GetPacketSize();

		memcpy(buffer, &m_Data, packetSize);

		return packetSize;
	}

	const U32 Packet::Deserialize(const char * buffer)
	{
		memcpy(&m_Data, buffer, k_PacketSize);

		U32 packetTemplateID = *reinterpret_cast<U32 *>(m_Data);
		
		const HashedRegistry<PacketTemplate> & packetTemplate = PacketTemplate::k_PacketTemplates;

		m_PacketTemplate = nullptr;
		for(auto it = packetTemplate.Begin(); it != packetTemplate.End(); ++it)
		{
			if(it->second.m_TemplateID == packetTemplateID)
			{
				m_PacketTemplate = &it->second;
			}
		}

		return 0;
	}

	bool Packet::HasField(const std::string & fieldName) const
	{
		return m_PacketTemplate->FieldExists(fieldName);
	}
	#pragma endregion
}