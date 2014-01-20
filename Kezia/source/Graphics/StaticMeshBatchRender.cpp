#include "StaticMeshBatchRender.h"

#include "Graphics/StaticMesh.h"

namespace Kezia
{
	StaticMeshBatchRender::StaticMeshBatchRender(StaticMesh * mesh)
		:	m_StaticMesh(mesh)
	{}

	StaticMeshBatchRender::~StaticMeshBatchRender()
	{}

	void StaticMeshBatchRender::Execute()
	{
		m_StaticMesh->ExecuteQueuedDraws();
	}
}