#include "BulletComponentBuilder.h"

#include "Core/AttachmentTarget.h"
#include "Core/XmlObjectBuilder.h"

#include "Physics/BoxPhysicsComponent.h"
#include "Physics/CapsulePhysicsComponent.h"
#include "Physics/PhysicsManager.h"
#include "Physics/StaticPlanePhysicsComponent.h"
#include "Physics/SpherePhysicsComponent.h"

namespace Kezia
{
	template<typename Component_T>
	static void * BuildBulletObject(const XmlNode & node)
	{
		Component_T * component = new Component_T(node);

		component->Attach(GetAttachmentTarget());

		//we're making an archetype, don't actually manage it
		g_PhysicsManager->RemovePhysicsComponent(component);

		return component;
	}

	static XmlObjectBuilder * CreateBulletComponentBuilder()
	{
		XmlObjectBuilder * bulletComponentBuilder = new XmlObjectBuilder();

		bulletComponentBuilder->RegisterObjectBuilder("StaticPlane", BuildBulletObject<StaticPlanePhysicsComponent>);
		bulletComponentBuilder->RegisterObjectBuilder("Sphere", BuildBulletObject<SpherePhysicsComponent>);
		bulletComponentBuilder->RegisterObjectBuilder("Capsule", BuildBulletObject<CapsulePhysicsComponent>);
		bulletComponentBuilder->RegisterObjectBuilder("Box", BuildBulletObject<BoxPhysicsComponent>);

		return bulletComponentBuilder;
	}

	XmlObjectBuilder * CreateOrGetBulletComponentBuilder()
	{
		static XmlObjectBuilder * bulletComponentBuilder = CreateBulletComponentBuilder();

		return bulletComponentBuilder;
	}
}