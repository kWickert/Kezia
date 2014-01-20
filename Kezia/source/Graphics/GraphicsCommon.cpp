#include "GraphicsCommon.h"

#include "Core/FileResource.h"
#include "Core/Windows_ResourceManager.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Kezia
{
	void ExtractFromNode(const aiScene * scene, const aiNode * node, std::vector< Vector3<F32> > & vertices, std::vector< Vector2<F32> > & textureCoordinates, std::vector< Vector3<F32> > & normals, std::vector<U32> & indices)
	{
		for(U32 meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex)
		{
			const struct aiMesh * mesh = scene->mMeshes[node->mMeshes[meshIndex]];

			const U32 indexOffset = vertices.size();

			for(U32 vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex)
			{
				normals.push_back(Vector3<F32>(&mesh->mNormals[vertexIndex].x));
				vertices.push_back(Vector3<F32>(&mesh->mVertices[vertexIndex].x));

				if(mesh->HasTextureCoords(0))
					textureCoordinates.push_back(Vector2<F32>(&mesh->mTextureCoords[0][vertexIndex].x));
			}

			//apply material

			for(U32 faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
			{
				const aiFace * face = &mesh->mFaces[faceIndex];

				for(U32 i = 0; i < face->mNumIndices; ++i)
				{
					indices.push_back(face->mIndices[i] + indexOffset);					
				}
			}
		}

		for(U32 childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
		{
			ExtractFromNode(scene, node->mChildren[childIndex], vertices, textureCoordinates, normals, indices);
		}
	}

	bool LoadMeshData(const std::string & fileName, std::vector< Vector3<F32> > & outPositions, std::vector< Vector2<F32> > & outTextureCoordinates, std::vector< Vector3<F32> > & outNormals, std::vector<U32> & outIndices)
	{
		std::string filePath;

		Windows_ResourceManager * winFileMgr = dynamic_cast<Windows_ResourceManager *>(g_FileResourceManager);

		if(winFileMgr != nullptr)
		{
			filePath = winFileMgr->GetFilePath(fileName);
		}

		bool loadSuccess = false;

		if(filePath != "")
		{
			const struct aiScene * scene = aiImportFile(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

			outPositions.clear();
			outTextureCoordinates.clear();
			outNormals.clear();
			outIndices.clear();

			ExtractFromNode(scene, scene->mRootNode, outPositions, outTextureCoordinates, outNormals, outIndices);

			loadSuccess = true;
		}
		else
		{
			LOG("could not load static mesh");
		}

		return loadSuccess;
	}
}