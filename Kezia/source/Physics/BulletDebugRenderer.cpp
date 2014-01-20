#include "BulletDebugRenderer.h"

#include "Core/Logger.h"

#include "Graphics/glInclude.h"
#include "Graphics/Camera.h"
#include "Graphics/IRenderer.h"
#include "Graphics/Material.h"

#include "Physics/BulletUtility.h"

namespace Kezia
{
	BulletDebugRenderer::BulletDebugRenderer(BulletManagerImplementation * managerImplementation)
		:	m_ManagerImplementation(managerImplementation)
	{
		TransformationNode * node = new TransformationNode();
		Attach(node);
	}

	BulletDebugRenderer::~BulletDebugRenderer()
	{}

	void BulletDebugRenderer::Draw(const Camera * camera)
	{
		Material::UnbindAllMaterials();

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(camera->GetViewMatrix());

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(camera->GetProjectionMatrix());

		m_ManagerImplementation->dynamicsWorld->debugDrawWorld();
	}

	void BulletDebugRenderer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
	{
		glColor3f(color.x(), color.y(), color.z());
		glBegin(GL_LINES);
			glVertex3f(from.x(), from.y(), from.z());
			glVertex3f(to.x(), to.y(), to.z());
		glEnd();
	}

	void BulletDebugRenderer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
	{

	}

	void BulletDebugRenderer::draw3dText(const btVector3& location,const char* textString)
	{

	}

	void BulletDebugRenderer::reportErrorWarning(const char* warningString)
	{
		LOG("BULLET: " << warningString);
	}

	void BulletDebugRenderer::setDebugMode(int debugMode)
	{
		m_DebugMode = debugMode;
	}

	int	BulletDebugRenderer::getDebugMode() const
	{
		return m_DebugMode;
	}
}