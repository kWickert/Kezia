#include "CommandLineParser.h"

#include "Core/Types.h"

#include <sstream>

namespace Kezia
{
	CommandLineParser::CommandLineParser(const std::string & commandLine)
	{
		std::stringstream arguments(commandLine);

		while(arguments.good())
		{
			if(!ParseArgument(arguments))
				break;
		}
	}

	bool CommandLineParser::ParseArgument(std::stringstream & argumentString)
	{
		std::string argument;

		argumentString >> argument;

		if(argument.empty())
			return false;

		switch(argument[0])
		{
		case '-': //new command
			{
				CommandData newCommand;
				newCommand.push_back(std::string(argument.begin() + 1, argument.end()));

				m_Commands.push_back(newCommand);
			}
			break;
		case '"': //quoted string command argument
			{
				argument.erase(0, 1);
				std::string remainingArgs = argumentString.str().substr(static_cast<U32>(argumentString.tellg()));
				
				std::size_t nextQuote = remainingArgs.find_first_of('"');
				argument.append(remainingArgs, 0, nextQuote);
				remainingArgs.erase(0, nextQuote + 1);

				argumentString.str(remainingArgs);
			}
		default: //command argument handling
			if(m_Commands.empty())
				throw std::exception("this sucks");

			m_Commands.back().push_back(argument);
			break;
		}

		return true;
	}

	CommandLineParser::CommandIterator CommandLineParser::Begin()
	{
		return m_Commands.begin();
	}

	CommandLineParser::CommandIterator CommandLineParser::End()
	{
		return m_Commands.end();
	}
}