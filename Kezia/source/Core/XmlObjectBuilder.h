#pragma once

#include "Core/HashedRegistry.h"
#include "Core/XmlNode.h"

#include <vector>

namespace Kezia
{
	class XmlObjectBuilder
	{
	public:
		XmlObjectBuilder();
		~XmlObjectBuilder();

		class ObjectBuilder;
		typedef void * (*BuilderFn)(const XmlNode &);

		template<typename Builder_T>
		static void * DirectBuild(const XmlNode & node);

		void * BuildObject(const XmlNode & node) const;
		void RegisterObjectBuilder(const std::string & nodeType, BuilderFn builderFunction);

		void RegisterChildBuilder(XmlObjectBuilder * builder);

		void EnableLogging();
		void DisableLogging();

		const std::string GetRegisteredBuildersCSV() const;
	private:
		HashedRegistry<ObjectBuilder> m_Builders;
		std::vector<XmlObjectBuilder *> m_ChildBuilders;

		bool m_IsLogging;
	};

	class XmlObjectBuilder::ObjectBuilder
	{
	public:
		ObjectBuilder(const std::string & nodeType, XmlObjectBuilder::BuilderFn builderFn)
			:	m_NodeType(nodeType),
				m_ObjectBuilder(builderFn)
		{}

		void * BuildObjectFromNode(const XmlNode & node) const
		{
			return m_ObjectBuilder(node);
		}

		const std::string & GetNodeType() const
		{
			return m_NodeType;
		}

	private:
		std::string m_NodeType;

		XmlObjectBuilder::BuilderFn m_ObjectBuilder;
	};

	template<typename Builder_T>
	void * XmlObjectBuilder::DirectBuild(const XmlNode & node)
	{
		return new Builder_T(node);
	}
}