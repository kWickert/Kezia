#include "StaticMeshComponent.h"

#include "Core/XmlNode.h"

#include "Graphics/StaticMesh.h"

namespace Kezia
{
	StaticMeshComponent::StaticMeshComponent(const XmlNode & node)
	{
		node.ValidateAttributes("source", "");

		std::string fileName = GetAttribute<std::string>(node, "source", "");

		m_BaseMesh = StaticMesh::CreateOrGetStaticMesh(fileName);
		m_BaseMesh->m_Instances.push_back(this);
	}

	StaticMeshComponent::StaticMeshComponent(StaticMesh * baseMesh)
		:	m_BaseMesh(baseMesh)
	{}

	StaticMeshComponent::StaticMeshComponent(const StaticMeshComponent & other)
		:	m_BaseMesh(other.m_BaseMesh)
	{
		m_BaseMesh->m_Instances.push_back(this);
	}

	StaticMeshComponent::~StaticMeshComponent()
	{
		auto findResult = std::find(m_BaseMesh->m_Instances.begin(), m_BaseMesh->m_Instances.end(), this);

		m_BaseMesh->m_Instances.erase(findResult);
	}

	AttachableComponent * StaticMeshComponent::Clone() const
	{
		return new StaticMeshComponent(*this);
	}

	void StaticMeshComponent::Update(const F64 deltaTime)
	{
		m_BaseMesh->ClearDrawQueue();
	}

	void StaticMeshComponent::Draw(const Camera * camera)
	{
		m_BaseMesh->EnqueueDraw(this, camera);
	}
}