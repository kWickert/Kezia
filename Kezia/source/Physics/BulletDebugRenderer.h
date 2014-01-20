#pragma once

#include "Graphics/RenderableComponent.h"

#include <btBulletDynamicsCommon.h>

namespace Kezia
{
	class Camera;

	struct BulletManagerImplementation;

	class BulletDebugRenderer : public btIDebugDraw, public RenderableComponent
	{
	public:
		BulletDebugRenderer(BulletManagerImplementation * managerImplementation);
		virtual ~BulletDebugRenderer();

		virtual void Draw(const Camera * camera);

		virtual void drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color);
		virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
		virtual void draw3dText(const btVector3& location,const char* textString);

		virtual void reportErrorWarning(const char* warningString);

		virtual void setDebugMode(int debugMode);
		virtual int	getDebugMode() const;
	private:
		BulletManagerImplementation * m_ManagerImplementation;

		int m_DebugMode;
	};
}