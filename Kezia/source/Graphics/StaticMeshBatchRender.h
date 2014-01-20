#pragma once

#include "Graphics/RenderOperation.h"

namespace Kezia
{
	class StaticMesh;

	class StaticMeshBatchRender : public RenderOperation
	{
	public:
		StaticMeshBatchRender(StaticMesh * mesh);
		virtual ~StaticMeshBatchRender();

		virtual void Execute();

	private:
		StaticMesh * m_StaticMesh;
	};
}