#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace Kezia
{
	class CommandLineParser
	{
	public:

		typedef std::vector<std::string> CommandData;

		class CommandIterator
		{
		public:
			CommandIterator(std::vector<CommandData>::iterator it)
				:	m_Iterator(it)
			{}

			CommandData operator*()
			{
				return *m_Iterator;
			}

			CommandIterator & operator++()
			{
				++m_Iterator;
				return *this;
			}

			CommandIterator operator++(int)
			{
				CommandIterator tmp(m_Iterator);

				++(*this);

				return tmp;
			}

			bool operator==(const CommandIterator & rhs) const
			{
				return m_Iterator == rhs.m_Iterator;
			}

			bool operator!=(const CommandIterator & rhs) const
			{
				return !(*this == rhs);
			}

		private:
			std::vector<CommandData>::iterator m_Iterator;
		};

		CommandLineParser(const std::string & commandLine);

		CommandIterator Begin();
		CommandIterator End();

	private:
		bool ParseArgument(std::stringstream & argumentString);

		std::vector<CommandData> m_Commands;
	};
}