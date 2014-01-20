#pragma once

#include "Core/Types.h"
#include "Core/DebugUtilities.h"
#include "Core/StringUtilities.h"

#define PUGIXML_HEADER_ONLY
#include <pugixml.cpp>

#include <algorithm>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace Kezia
{
	struct ReferenceInfo
	{
		ReferenceInfo(const std::string & filePath = "")
			:	count(0),
				filePath(filePath)
		{}

		ReferenceInfo & operator++()
		{
			count++;
			return *this;
		}

		ReferenceInfo operator++(int)
		{
			ReferenceInfo tmp(*this);

			++(*this);

			return tmp;
		}

		ReferenceInfo & operator--()
		{
			count--;
			return *this;
		}

		ReferenceInfo operator--(int)
		{
			ReferenceInfo tmp(*this);

			--(*this);

			return tmp;
		}

		U32 count;
		std::string filePath;
	};

	class XmlNode
	{
	public:
		XmlNode(const std::string & filePath);

		~XmlNode();

		XmlNode GetParentNode() const;

		XmlNode GetFirstChildNode() const;
		XmlNode GetLastChildNode() const;

		XmlNode GetNextSiblingNode() const;
		XmlNode GetPreviousSiblingNode() const;

		bool IsEmpty() const;

		const std::string & GetFilePath() const;

		const std::string GetName() const;

		const std::string GetPCData() const;

		const std::string GetAttributeValue(const std::string & attributeName) const;

		bool ValidateChildElements(const std::string & csvRequiredChildren, const std::string &csvOptionalChildren, bool unexpectedCausesFail = true) const;
		bool ValidateAttributes(const std::string & csvRequiredAttributes, const std::string & csvOptionalAttributes, bool unexpectedCausesFail = true) const;
	private:
		static std::map<pugi::xml_node, ReferenceInfo> k_ReferenceMap;

		XmlNode(const pugi::xml_node & element);

		const U32 GetLineNumber() const;

		template<typename Value_T>
		bool Validate(std::list<Value_T> & actual, std::vector<std::string> & required, std::vector<std::string> & optional, bool unexpectedCausesFail, std::vector<std::string> & outMissing) const;

		template<typename Value_T>
		void LogInvalid(std::vector<std::string> & missing, std::list<Value_T> & unexpected, const std::string & required, const std::string & optional) const;

		pugi::xml_node m_Element;
	};

	template<typename Value_T>
	bool XmlNode::Validate(std::list<Value_T> & actual, std::vector<std::string> & required, std::vector<std::string> & optional, bool unexpectedCausesFail, std::vector<std::string> & outMissing) const
	{
		outMissing.clear();

		U32 prevSize = actual.size();
		for(auto it = required.begin(); it != required.end(); ++it)
		{
			actual.erase(std::remove_if(actual.begin(), actual.end(), [&](const Value_T & node) { return node.name() == *it; }), actual.end());

			U32 newSize = actual.size();

			bool elementsRemoved = newSize < prevSize;
			if(!elementsRemoved)
			{
				outMissing.push_back(*it);
			}

			prevSize = newSize;
		}

		for(auto it = optional.begin(); it != optional.end(); ++it)
		{
			actual.erase(std::remove_if(actual.begin(), actual.end(), [&](const Value_T & node) { return node.name() == *it; }), actual.end());
		}

		bool valid = outMissing.empty() && (unexpectedCausesFail ? actual.empty() : true);

		return valid;
	}

	template<typename Value_T>
	void XmlNode::LogInvalid(std::vector<std::string> & missing, std::list<Value_T> & unexpected, const std::string & required, const std::string & optional) const
	{
		bool loggingChildren = typeid(Value_T) == typeid(pugi::xml_node);
		const std::string type = loggingChildren ? "Children" : "Attributes";

		const U32 lineNumber = GetLineNumber();

		std::stringstream modalStream, vsDebugStream;

		if(!required.empty())
			vsDebugStream << "\n\trequired " << type << ": " << required;

		if(!optional.empty())
			vsDebugStream << "\n\toptional " << type << ": " << optional;

		std::string rules = vsDebugStream.str();
		vsDebugStream.str("");

		std::string requiredDebugString, unexpectedDebugString;

		if(!missing.empty())
		{
			modalStream.str("");

			modalStream << "Incomplete node found on line " << lineNumber << " of " << GetFilePath() << ":\n\n";
				
			m_Element.print(modalStream);

			modalStream << "\nMissing " << type << ":\n\n";

			for(U32 i = 0; i < missing.size(); i++)
			{
				modalStream << '\t' << missing[i] << std::endl; 
			}

			ShowModalWindow("Incomplete Node: Missing " + type, modalStream.str());

			vsDebugStream << "missing required " << type << ": ";

			for(auto it = missing.begin(); it != missing.end(); ++it)
			{
				U32 distance = std::distance(it, missing.end());

				vsDebugStream << *it;

				if(distance > 1)
					vsDebugStream << ',';
			}

			vsDebugStream << rules;

			WriteDebugString(GenerateDebugString(GetFilePath(), lineNumber, vsDebugStream.str()));
		}

		if(!unexpected.empty())
		{
			modalStream.str("");

			std::vector<std::string> options;
			ParseList(required, ',', options, true);
			ParseList(optional, ',', options, true);

			modalStream << "Invalid " << (loggingChildren ? "children" : "attributes") << '\n'; 

			for(auto it = unexpected.begin(); it != unexpected.end(); ++it)
			{
				modalStream << "\n\t" << (*it).name();
			}

			modalStream << std::endl;

			modalStream << "\nfound on line " << lineNumber << " of " << GetFilePath() << ":\n\n";
			m_Element.print(modalStream);

			modalStream << "\nAcceptable " << (loggingChildren ? "children" : "attributes") << ":\n\n";

			for(U32 i = 0; i < options.size(); i++)
			{
				modalStream << '\t' << options[i] << std::endl; 
			}

			ShowModalWindow("Incomplete Node: Unexpected " + type, modalStream.str());

			vsDebugStream.str("");
			vsDebugStream << "unexpected " << type << ": ";

			for(auto it = unexpected.begin(); it != unexpected.end(); ++it)
			{
				U32 distance = std::distance(it, unexpected.end());

				vsDebugStream << (*it).name();

				if(distance > 1)
					vsDebugStream << ',';
			}

			vsDebugStream << rules;

			WriteDebugString(GenerateDebugString(GetFilePath(), lineNumber, vsDebugStream.str()));
		}
	}

	template<typename T>
	inline const T GetAttribute(const XmlNode & node, const std::string & attributeName, const T & defaultValue)
	{
		T retVal;

		std::string attributeValue = node.GetAttributeValue(attributeName);

		if(attributeValue.empty())
			retVal = defaultValue;
		else
			std::stringstream(attributeValue) >> retVal;

		return retVal;
	}

	template<>
	inline const std::string GetAttribute<std::string>(const XmlNode & node, const std::string & attributeName, const std::string & defaultValue)
	{
		std::string retVal;

		std::string attributeValue = node.GetAttributeValue(attributeName);

		if(attributeValue.empty())
			retVal = defaultValue;
		else
			retVal = attributeValue;

		return retVal;
	}

	template<>
	inline const bool GetAttribute<bool>(const XmlNode & node, const std::string & attributeName, const bool & defaultValue)
	{
		bool retVal;

		std::string attributeValue = node.GetAttributeValue(attributeName);

		if(attributeValue.empty())
			retVal = defaultValue;
		else
			std::stringstream(attributeValue) >> std::boolalpha >> retVal;

		return retVal;
	}
}
