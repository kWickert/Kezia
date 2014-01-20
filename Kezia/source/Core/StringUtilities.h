#pragma once

#include "Core/Logger.h"
#include "Core/Types.h"

#include <algorithm>
#include <limits>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

namespace Kezia
{
	template<typename Value_T>
	void ParseList(const std::string & text, std::vector<char> & delimiters, std::vector<Value_T> & outTokens, bool appendTokens = false)
	{
		if(!appendTokens)
			outTokens.clear();

		U32 marker = 0;

		std::stringstream ss;

		while(true)
		{
			U32 first = std::numeric_limits<U32>::max();

			for(auto it = delimiters.begin(); it != delimiters.end(); ++it)
			{
				U32 next = text.find(*it, marker);

				if(next < first)
				{
					first = next;
				}
			}

			std::string token = text.substr(marker, first - marker);
			token.erase(std::remove_if(token.begin(), token.end(), isspace), token.end());

			std::stringstream ss(token);

			Value_T val;
			ss >> val;

			outTokens.push_back(val);

			marker = first + 1;

			if(first == std::string::npos)
				break;
		}
	}

	template<typename Value_T>
	void ParseList(const std::string & text, char delimiter, std::vector<Value_T> & outTokens, bool appendTokens = false)
	{
		if(!appendTokens)
			outTokens.clear();

		U32 marker = 0;

		while(!text.empty())
		{
			U32 first = text.find(delimiter, marker);

			std::string token = text.substr(marker, first - marker);
			token.erase(std::remove_if(token.begin(), token.end(), isspace), token.end());

			std::stringstream ss(token);

			Value_T val;
			ss >> val;

			outTokens.push_back(val);

			marker = first + 1;

			if(first == std::string::npos)
				break;
		}
	}


	inline void ToLower(const std::string & text, std::string & outText)
	{
		outText.clear();

		for(auto it = text.begin(); it != text.end(); ++it)
		{
			outText.push_back(tolower(*it));
		}
	}

	inline std::string ExtractFileName(const std::string & path)
	{
		std::vector<std::string> tokens;

		ParseList(path.substr(path.find_last_of('\\') + 1), '.', tokens);

		return tokens[0];
	}
}
