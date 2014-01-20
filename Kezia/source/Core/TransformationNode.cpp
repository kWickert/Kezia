#include "TransformationNode.h"

#include "Core/AttachableComponent.h"

#include <algorithm>
#include <string>

namespace Kezia
{
	TransformationNode::TransformationNode()
		:	m_Position(),
			m_Orientation(),
			m_Scale(1),
			m_InverseScale(1),
			m_Name("")
	{
		UpdateWorldTransform(MatrixStack<F32>());
	}

	TransformationNode::TransformationNode(const TransformationNode & other)
		:	m_Position(other.m_Position),
			m_Orientation(other.m_Orientation),
			m_Scale(other.m_Scale),
			m_InverseScale(other.m_Scale),
			m_Name(other.m_Name)
	{}

	TransformationNode::~TransformationNode()
	{
		//TODO decide what to do with this:
		//should it destroy child nodes or reparent?
	}

	void TransformationNode::AddChild(TransformationNode * node)
	{
		m_ChildNodes.push_back(node);
	}

	TransformationNode * TransformationNode::GetChildNode(const std::string & name)
	{
		TransformationNode * node = nullptr;

		for(auto it = m_ChildNodes.begin(); it != m_ChildNodes.end(); ++it)
		{
			const std::string childName = (*it)->GetNodeName();

			if(childName == name)
			{
				node = *it;
				break;
			}
		}

		return node;
	}

	TransformationNode * TransformationNode::FullCopy() const
	{
		TransformationNode * fullCopy = new TransformationNode(*this);

		for(auto it = m_ChildNodes.begin(); it != m_ChildNodes.end(); ++it)
		{
			fullCopy->AddChild((*it)->FullCopy());
		}

		for(auto it = m_AttachedComponents.begin(); it != m_AttachedComponents.end(); ++it)
		{
			AttachableComponent * newComp = (*it)->Clone();
			newComp->Attach(fullCopy);
		}

		fullCopy->UpdateWorldTransform(MatrixStack<F32>());

		return fullCopy;
	}

	void TransformationNode::UpdateWorldTransform(MatrixStack<F32> & matrixStack)
	{
		matrixStack.PushCurrentMatrix();
		matrixStack.Translate(m_Position);
		matrixStack.MultMatrix(Matrix4<F32>(m_Orientation));

		//use the scaling for this node but don't pass it on to children
		m_WorldTransform = matrixStack.GetCurrentMatrix() * Matrix4<F32>::Scale(m_Scale.x(), m_Scale.y(), m_Scale.z());
	
		for(auto it = m_ChildNodes.begin(); it != m_ChildNodes.end(); ++it)
		{
			(*it)->UpdateWorldTransform(matrixStack);
		}

		for(auto it = m_AttachedComponents.begin(); it != m_AttachedComponents.end(); ++it)
		{
			(*it)->NotifyTransformUpdate();
		}

		matrixStack.PopMatrix();
	}

	void TransformationNode::NotifyAttach(AttachableComponent * component)
	{
		m_AttachedComponents.push_back(component);
	}

	void TransformationNode::NotifyDetach(AttachableComponent * component)
	{
		m_AttachedComponents.erase(std::find(m_AttachedComponents.begin(), m_AttachedComponents.end(), component));
	}
}