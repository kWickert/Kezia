#include "NamedProperties.h"

namespace Kezia
{
	TypedDataBase::~TypedDataBase()
	{}

	NamedProperties::NamedProperties()
		:	m_Properties(HashedRegistry<TypedDataBase *>::CaseInsensitiveHash)
	{}

	//this might be inadequate
	NamedProperties::NamedProperties(const NamedProperties & other)
		:	m_Properties(other.m_Properties.GetHashFunction())
	{
		for(auto it = other.m_Properties.Begin(); it != other.m_Properties.End(); ++it)
		{
			m_Properties.Insert(it->first.key, it->second->Clone());
		}
	}

	NamedProperties::~NamedProperties()
	{
		for(auto it = m_Properties.Begin(); it != m_Properties.End(); ++it)
		{
			delete it->second;
		}
	}

	NamedProperties & NamedProperties::operator=(const NamedProperties & rhs)
	{
		NamedProperties tmp(rhs);
		swap(*this, tmp);
		return *this;
	}

	void swap(NamedProperties & lhs, NamedProperties & rhs)
	{
		swap(lhs.m_Properties, rhs.m_Properties);
	}
}