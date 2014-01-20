#pragma once

#include "Core/Job.h"
#include "Core/IResourceManager.h"

namespace Kezia
{
	class LoadFileJob : public Job
	{
	public:
		LoadFileJob(const std::string & fileName, char ** buffer);
		virtual ~LoadFileJob();

		virtual void Execute();
	private:
		const std::string m_FileName;
		char ** m_DestinationBuffer;
	};
}