#pragma once

#include <fstream>

#define LOG(text) \
	{	\
		g_Logger->PrintTimeStamp(); \
		(*g_Logger << ": " << text); \
		(*g_Logger << '\n'); \
		g_Logger->Flush();	\
	}

namespace Kezia
{
	class Logger
	{
	public:
		Logger(const std::string & fileName = "log");
		~Logger();

		void PrintTimeStamp();

		void Flush();

		template<typename Value_T>
		friend Logger & operator<<(Logger & stream, const Value_T & value)
		{
			stream.m_File << value;

			return stream;
		}
	private:
		std::fstream m_File;
	};

	extern Logger * g_Logger;
}