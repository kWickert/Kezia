#pragma once

#include "Core/HashedRegistry.h"

#include <map>
#include <string>
#include <vector>

namespace Kezia
{
	class BufferObjectBase;
	class Camera;
	class Material;
	class NamedProperties;
	class StaticMeshBatchRender;
	class StaticMeshComponent;

	class StaticMesh
	{
	public:
		StaticMesh(const std::string & fileName);
		~StaticMesh();

		StaticMeshComponent * CreateNewInstance();

		void ExecuteQueuedDraws();

		static StaticMesh * CreateOrGetStaticMesh(const std::string & fileName);

		friend class StaticMeshComponent;
	protected:
		const bool IsDrawQueueEmpty() const;
		void ClearDrawQueue();
		void EnqueueDraw(StaticMeshComponent * component, const Camera * camera);

		std::map<const Camera *, std::vector<StaticMeshComponent *> > m_DrawQueue;

	private:
		static HashedRegistry<StaticMesh *> k_StaticMeshes;
		std::string m_Key;

		std::vector<StaticMeshComponent *> m_Instances;

		StaticMeshBatchRender * m_RenderOperation;
		
		BufferObjectBase * m_IndexData;

		BufferObjectBase * m_PositionData;
		BufferObjectBase * m_TextureCoordinateData;
		BufferObjectBase * m_NormalData;
	};
}