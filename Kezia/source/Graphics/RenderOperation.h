#pragma once

namespace Kezia
{
	class RenderOperation
	{
	public:
		RenderOperation();
		virtual ~RenderOperation();

		virtual void Execute();
	};
}