#pragma once

namespace Kezia
{
	class NonCopyable
	{
	public:
		NonCopyable()
		{}

		~NonCopyable()
		{}

	private:
		NonCopyable(const NonCopyable &);
		NonCopyable & operator=(const NonCopyable &);
	};
}