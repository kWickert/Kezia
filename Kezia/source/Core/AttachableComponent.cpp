#include "AttachableComponent.h"

namespace Kezia
{
	AttachableComponent::AttachableComponent()
		:	m_Node(nullptr)
	{}

	AttachableComponent::AttachableComponent(const AttachableComponent & other)
		:	m_Node(nullptr)
	{}

	AttachableComponent::~AttachableComponent()
	{}

	AttachableComponent * AttachableComponent::Clone() const
	{
		return new AttachableComponent(*this);
	}

	void AttachableComponent::Attach(TransformationNode * node)
	{
		if(node != m_Node)
		{
			Detach();

			m_Node = node;
			m_Node->NotifyAttach(this);

			PostAttach();
		}	
	}

	void AttachableComponent::PostAttach()
	{}

	void AttachableComponent::Detach()
	{
		if(m_Node != nullptr)
		{
			m_Node->NotifyDetach(this);
		}
	
		m_Node = nullptr;
	}

	void AttachableComponent::NotifyTransformUpdate()
	{}
}