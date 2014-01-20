#include "LoadFileJob.h"

#include "Core/IResource.h"
#include "Core/Logger.h"

#include <iostream>

namespace Kezia
{
	extern IResourceManager * g_ResourceManager;

	LoadFileJob::LoadFileJob(const std::string & fileName, char ** buffer)
		:	m_FileName(fileName),
			m_DestinationBuffer(buffer)
	{}

	LoadFileJob::~LoadFileJob()
	{}

	void LoadFileJob::Execute()
	{
		IResource * resource = g_ResourceManager->OpenResourceFile(m_FileName);

		if(resource == nullptr)
		{
			LOG("could not find resource " << m_FileName);
		}

		U32 size = resource->GetDataSize();
		
		(*m_DestinationBuffer) = new char[size];
		resource->ReadIntoBuffer(*m_DestinationBuffer);
	}
}