#pragma once

#include "Physics/BulletPhysicsComponent.h"

namespace Kezia
{
	class XmlNode;

	class BoxPhysicsComponent : public BulletPhysicsComponent
	{
	public:
		BoxPhysicsComponent(const XmlNode & node);
		BoxPhysicsComponent(const BoxPhysicsComponent & other);
		virtual ~BoxPhysicsComponent();

		virtual AttachableComponent * Clone() const;

		virtual void InitializeBulletComponent(BulletComponentImplementation * component);

		const Vector3<F32> & GetHalfExtentVector() const;

	private:
		Vector3<F32> m_HalfExtentVector;
	};

	inline const Vector3<F32> & BoxPhysicsComponent::GetHalfExtentVector() const
	{
		return m_HalfExtentVector;
	}
}