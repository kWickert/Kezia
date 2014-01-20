#pragma once

namespace Kezia
{
	class TransformationNode;

	void ClearAttachmentTarget();
	void SetAttachmentTarget(TransformationNode * node);
	TransformationNode * GetAttachmentTarget();
}