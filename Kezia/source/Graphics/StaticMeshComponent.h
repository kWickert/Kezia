#pragma once

#include "Graphics/RenderableComponent.h"

namespace Kezia
{
	class Camera;
	class StaticMesh;
	class XmlNode;

	class StaticMeshComponent : public RenderableComponent
	{
	public:
		StaticMeshComponent(const XmlNode & node);
		StaticMeshComponent(StaticMesh * baseMesh);
		StaticMeshComponent(const StaticMeshComponent & other);
		virtual ~StaticMeshComponent();

		virtual AttachableComponent * Clone() const;

		virtual void Update(const F64 deltaTime);
		virtual void Draw(const Camera * camera);
	private:
		StaticMesh * m_BaseMesh;
	};
}