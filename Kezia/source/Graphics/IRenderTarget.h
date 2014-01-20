#pragma once

namespace Kezia
{
	class IRenderTarget
	{
	public:
		virtual ~IRenderTarget()
		{}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Clear() = 0;
	};
}