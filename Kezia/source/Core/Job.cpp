#include "Job.h"

namespace Kezia
{
	Job::Job(const F32 priority)
		:	m_Priority(priority)
	{}

	const F32 Job::GetPriority() const
	{
		return m_Priority;
	}
}