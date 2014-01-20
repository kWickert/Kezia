#include "AttachmentTarget.h"

#include "Core/TransformationNode.h"

namespace Kezia
{
	static TransformationNode * g_AttachmentTarget = nullptr;

	void ClearAttachmentTarget()
	{
		g_AttachmentTarget = nullptr;
	}

	void SetAttachmentTarget(TransformationNode * node)
	{
		g_AttachmentTarget = node;
	}

	TransformationNode * GetAttachmentTarget()
	{
		return g_AttachmentTarget;
	}
}
