#include "Core/GenerateFilesCommandlet.h"

#include "Core/WindowsInclude.h"

#include "Math/KeziaRandom.h"

#include <fstream>
#include <sstream>

namespace Kezia
{
	void GenerateFilesCommandlet(const U32 numFiles, const U32 fileSizeKb)
	{
		char tmp[255];
		GetCurrentDirectoryA(255, tmp);
		std::string currentDirectory(tmp);

		U8 min = std::numeric_limits<U8>::min();
		U8 max = std::numeric_limits<U8>::max();

		U32 fileSizeBytes = fileSizeKb * 1024;
		char * buffer = new char[fileSizeBytes];

		for(U32 i = 0; i < numFiles; i++)
		{
			std::stringstream ss;
			ss << currentDirectory << "\\file" << i << ".dat";
			std::cout << ss.str() << std::endl;

			for(U32 byte = 0; byte < fileSizeBytes; byte++)
			{
				buffer[byte] = RandomInRange(min, max);
			}

			std::ofstream file(ss.str());
			file.write(buffer, fileSizeBytes);
			file.close();
		}

		exit(1);
	}
}