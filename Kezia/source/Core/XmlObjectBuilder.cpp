#include "XmlObjectBuilder.h"

#include "Core/Assert.h"
#include "Core/Logger.h"
#include "Core/XmlNode.h"

namespace Kezia
{
	XmlObjectBuilder::XmlObjectBuilder()
		:	m_IsLogging(true)
	{}

	XmlObjectBuilder::~XmlObjectBuilder()
	{}

	void * XmlObjectBuilder::BuildObject(const XmlNode & node) const
	{
		auto findResult = m_Builders.Find(node.GetName());

		void * newObj = nullptr;

		if(findResult != m_Builders.End())
		{
			newObj = findResult->second.BuildObjectFromNode(node);
		}
		else
		{
			for(auto it = m_ChildBuilders.begin(); it != m_ChildBuilders.end(); ++it)
			{
				newObj = (*it)->BuildObject(node);

				if(newObj != nullptr)
					break;
			}

			if(newObj == nullptr && m_IsLogging)
				LOG("no xml builder registered for node type, " << node.GetName());
		}

		return newObj;
	}

	void XmlObjectBuilder::RegisterObjectBuilder(const std::string & nodeType, BuilderFn builderFunction)
	{
		m_Builders.Insert(nodeType, ObjectBuilder(nodeType, builderFunction));
	}

	void XmlObjectBuilder::RegisterChildBuilder(XmlObjectBuilder * builder)
	{
		auto findResult = std::find(m_ChildBuilders.begin(), m_ChildBuilders.end(), builder);

		KeziaAssert(findResult == m_ChildBuilders.end());

		m_ChildBuilders.push_back(builder);
	}

	const std::string XmlObjectBuilder::GetRegisteredBuildersCSV() const
	{
		std::string validComponents;

		for(auto it = m_Builders.Begin(); it != m_Builders.End(); ++it)
		{
			validComponents += (*it).second.GetNodeType() + ",";
		}

		for(auto it = m_ChildBuilders.begin(); it != m_ChildBuilders.end(); ++it)
		{
			validComponents += (*it)->GetRegisteredBuildersCSV() + ",";
		}

		validComponents.resize(validComponents.size() - 1);

		return validComponents;
	}

	void XmlObjectBuilder::EnableLogging()
	{
		m_IsLogging = true;
	}

	void XmlObjectBuilder::DisableLogging()
	{
		m_IsLogging = false;
	}
}