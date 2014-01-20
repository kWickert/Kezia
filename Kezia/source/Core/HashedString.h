#pragma once

#include "Core/Types.h"
#include "SuperFastHash.h"

#include <algorithm>
#include <string>

namespace
{
	const Kezia::U32 GenerateLowerCaseHash(const std::string & key)
	{
		std::string lowerKey;
		lowerKey.resize(key.size());
		std::transform(key.begin(), key.end(), lowerKey.begin(),
			[&](const char & c)->char
		{
			return tolower(c);	
		});

		return SuperFastHash(key);
	}
}

namespace Kezia
{
	class HashedString
	{
	public:
		HashedString(const char * string)
			:	m_String(string),
				m_CaseSensitiveHash(SuperFastHash(m_String)),
				m_CaseInsensitiveHash(GenerateLowerCaseHash(m_String))
		{}

		HashedString(const std::string & string)
			:	m_String(string),
				m_CaseSensitiveHash(SuperFastHash(m_String)),
				m_CaseInsensitiveHash(GenerateLowerCaseHash(m_String))
		{}

		HashedString(const HashedString & other)
			:	m_String(other.m_String),
				m_CaseSensitiveHash(other.m_CaseSensitiveHash),
				m_CaseInsensitiveHash(other.m_CaseInsensitiveHash)
		{}

		~HashedString()
		{}

		inline const U32 GetHashValue() const
		{
			return m_CaseSensitiveHash;
		}

		inline const U32 GetCaseInsensitiveHash() const
		{
			return m_CaseInsensitiveHash;
		}

		inline const std::string & GetString() const
		{
			return m_String;
		}
	private:
		const std::string m_String;
		const U32 m_CaseSensitiveHash;		
		const U32 m_CaseInsensitiveHash;
	};
}