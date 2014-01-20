#include "StaticMesh.h"

#include "Event/EventManager.h"

#include "Graphics/BufferObject.h"
#include "Graphics/GraphicsCommon.h"
#include "Graphics/Camera.h"
#include "Graphics/Material.h"
#include "Graphics/IRenderer.h"
#include "Graphics/StaticMeshBatchRender.h"
#include "Graphics/StaticMeshComponent.h"

namespace Kezia
{
	HashedRegistry<StaticMesh *> StaticMesh::k_StaticMeshes;

	StaticMesh::StaticMesh(const std::string & fileName)
	{
		m_Instances.reserve(100);

		std::vector< Vector3<F32> > positions, normals;
		std::vector< Vector2<F32> > textureCoorinates;
		std::vector<U32> indices;

		m_Key = fileName;

		if(LoadMeshData(fileName, positions, textureCoorinates, normals, indices))
		{
			m_PositionData = new BufferObject< Vector3<F32> >(positions.data(), positions.size());
			m_TextureCoordinateData = new BufferObject< Vector2<F32> >(textureCoorinates.data(), textureCoorinates.size());
			m_NormalData = new BufferObject< Vector3<F32> >(normals.data(), normals.size());

			m_IndexData = new BufferObject<U32, GL_ELEMENT_ARRAY_BUFFER>(indices.data(), indices.size());

			Material * m = reinterpret_cast<Material *>(g_Renderer->GetMaterial("StaticMesh"));

			KeziaAssert(m != nullptr);

			m->UpdateAttributeData("a_Position", positions.data(), positions.size());
			m->UpdateAttributeData("a_TextureCoordinates", textureCoorinates.data(), textureCoorinates.size());
			m->UpdateAttributeData("a_Normal", normals.data(), normals.size());

			VertexAttributeObjectBase * a = m->GetAttribute("a_Model");

			KeziaAssert(a != nullptr);

			a->SetDivisor(1);

			m_RenderOperation = new StaticMeshBatchRender(this);
			g_Renderer->AddRenderOperation(m_RenderOperation);
		}
		else
		{
			LOG("could not load mesh data, " << fileName);
		}
	}

	StaticMesh::~StaticMesh()
	{
		g_Renderer->RemoveRenderOperation(m_RenderOperation);
		delete m_RenderOperation;

		for(auto it = m_Instances.begin(); it != m_Instances.end(); ++it)
		{
			delete *it;
		}

		delete m_IndexData;
		delete m_NormalData;
		delete m_PositionData;
		delete m_TextureCoordinateData;
		delete m_NormalData;

		k_StaticMeshes.Erase(k_StaticMeshes.Find(m_Key));
	}

	StaticMeshComponent * StaticMesh::CreateNewInstance()
	{
		KeziaAssert(m_Instances.size() < 100);
		m_Instances.push_back(new StaticMeshComponent(this));

		return m_Instances.back();
	}

	StaticMesh * StaticMesh::CreateOrGetStaticMesh(const std::string & fileName)
	{
		auto findResult = k_StaticMeshes.Find(fileName);

		StaticMesh * mesh = nullptr;

		if(findResult != k_StaticMeshes.End())
		{
			mesh = findResult->second;
		}
		else
		{
			mesh = new StaticMesh(fileName);
			k_StaticMeshes.Insert(fileName, mesh);
		}

		return mesh;
	}

	void StaticMesh::ExecuteQueuedDraws()
	{
		static Material * staticMeshMaterial = reinterpret_cast<Material *>(g_Renderer->GetMaterial("StaticMesh"));

		for(auto it = m_DrawQueue.begin(); it != m_DrawQueue.end(); ++it)
		{
			const Camera * camera = it->first;
			std::vector<StaticMeshComponent *> visibleInstances = it->second;

			std::vector< Matrix4<F32> > transforms;
			transforms.reserve(visibleInstances.size());

			for(auto it = visibleInstances.begin(); it != visibleInstances.end(); ++it)
			{
				transforms.push_back((*it)->GetWorldTransform());
			}

			Matrix4<F32> vp = camera->GetProjectionMatrix() * camera->GetViewMatrix();
			staticMeshMaterial->SetUniformValue("u_ViewProjection", vp);

			staticMeshMaterial->UpdateAttributeData("a_Model", transforms.data(), transforms.size());

			staticMeshMaterial->Bind();

			m_IndexData->Bind();

			glDrawElementsInstanced(GL_TRIANGLES, m_IndexData->GetSize(), GL_UNSIGNED_INT, 0, m_Instances.size());
		}
	}

	const bool StaticMesh::IsDrawQueueEmpty() const
	{
		return m_DrawQueue.empty();
	}

	void StaticMesh::ClearDrawQueue()
	{
		m_DrawQueue.clear();
	}

	void StaticMesh::EnqueueDraw(StaticMeshComponent * component, const Camera * camera)
	{
		m_DrawQueue[camera].push_back(component);
	}
}