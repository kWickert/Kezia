#include "XmlNode.h"

#include "Core/Assert.h"
#include "Core/StringUtilities.h"

#include <iostream>
#include <fstream>
#include <streambuf>

namespace Kezia
{
	std::map<pugi::xml_node, ReferenceInfo> XmlNode::k_ReferenceMap;

	XmlNode::XmlNode(const std::string & filePath)
	{
		pugi::xml_document * doc = new pugi::xml_document();
		pugi::xml_parse_result result = doc->load_file(filePath.c_str());

		KeziaAssert(result);

		std::cout << result.description();

		m_Element = doc->root();

		k_ReferenceMap.insert(std::make_pair(m_Element, ReferenceInfo(filePath)));
		k_ReferenceMap[m_Element]++;
	}

	XmlNode::XmlNode(const pugi::xml_node & element)
		:	m_Element(element)
	{
		k_ReferenceMap[m_Element.root()]++;
	}

	XmlNode::~XmlNode()
	{
		pugi::xml_node root = m_Element.root();

		k_ReferenceMap[root]--;

		if(k_ReferenceMap[root].count == 0)
		{
			k_ReferenceMap.erase(root);
		}
	}

	XmlNode XmlNode::GetParentNode() const
	{
		return XmlNode(m_Element.parent());
	}

	XmlNode XmlNode::GetFirstChildNode() const
	{
		return XmlNode(m_Element.first_child());
	}

	XmlNode XmlNode::GetLastChildNode() const
	{
		return XmlNode(m_Element.last_child());
	}

	XmlNode XmlNode::GetNextSiblingNode() const
	{
		return XmlNode(m_Element.next_sibling());
	}

	Kezia::XmlNode XmlNode::GetPreviousSiblingNode() const
	{
		return XmlNode(m_Element.previous_sibling());
	}

	bool XmlNode::IsEmpty() const
	{
		return m_Element.empty();
	}

	const std::string XmlNode::GetName() const
	{
		return m_Element.name();
	}

	const std::string XmlNode::GetPCData() const
	{
		return m_Element.text().as_string();
	}

	const std::string XmlNode::GetAttributeValue(const std::string & attributeName) const
	{
		return m_Element.attribute(attributeName.c_str()).value();
	}

	bool XmlNode::ValidateChildElements(const std::string & csvRequiredChildren, const std::string &csvOptionalChildren, bool unexpectedCausesFail) const
	{
		std::vector<std::string> requiredChildren;
		ParseList(csvRequiredChildren, ',', requiredChildren);

		std::vector<std::string> optionalChildren;
		ParseList(csvOptionalChildren, ',', optionalChildren);

		std::list<pugi::xml_node> actualChildren;

		for(pugi::xml_node child = m_Element.first_child(); child; child = child.next_sibling())
		{
			actualChildren.push_back(child);
		}

		std::vector<std::string> missingChildren;

		bool valid = Validate(actualChildren, requiredChildren, optionalChildren, unexpectedCausesFail, missingChildren);
		if(!valid)
		{
			std::stringstream ss;

			ss << csvRequiredChildren << ',' << csvOptionalChildren;

			LogInvalid(missingChildren, actualChildren, csvRequiredChildren, csvOptionalChildren);
		}

		KeziaAssert(valid);

		return valid;
	}

	bool XmlNode::ValidateAttributes(const std::string & csvRequiredAttributes, const std::string & csvOptionalAttributes, bool unexpectedCausesFail) const
	{
		std::vector<std::string> requiredAttributes;
		ParseList(csvRequiredAttributes, ',', requiredAttributes);

		std::vector<std::string> optionalAttributes;
		ParseList(csvOptionalAttributes, ',', optionalAttributes);

		std::list<pugi::xml_attribute> actualAttributes;

		for(pugi::xml_attribute attr = m_Element.first_attribute(); attr; attr = attr.next_attribute())
		{
			actualAttributes.push_back(attr);
		}

		std::vector<std::string> missingAttributes;

		bool valid = Validate(actualAttributes, requiredAttributes, optionalAttributes, unexpectedCausesFail, missingAttributes);

		if(!valid)
		{
			std::stringstream ss;

			ss << csvRequiredAttributes;

			if(ss.str().size() > 0 && !csvOptionalAttributes.empty())
			{
				ss << ',';
			}

			ss << csvOptionalAttributes;

			LogInvalid(missingAttributes, actualAttributes, csvRequiredAttributes, csvOptionalAttributes);
		}

		KeziaAssert(valid);

		return valid;
	}

	const U32 XmlNode::GetLineNumber() const
	{
		std::string fileText;
		fileText.assign(std::istreambuf_iterator<char>(std::ifstream(GetFilePath())), std::istreambuf_iterator<char>());

		U32 lineNumber = 1;
		for(S32 i = 0; i < m_Element.offset_debug() && i < static_cast<S32>(fileText.size()); i++)
		{
			if(fileText[i] == '\n')
				lineNumber++;
		}
		
		return lineNumber;
	}

	const std::string & XmlNode::GetFilePath() const
	{
		return k_ReferenceMap[m_Element.root()].filePath;
	}
}