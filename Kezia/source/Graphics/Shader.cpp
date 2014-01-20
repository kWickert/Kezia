#include "Graphics/Shader.h"

#include "Core/WindowsInclude.h"

#include <gl/glew.h>
#include <gl/GL.h>

#include <fstream>
#include <sstream>
#include <string>

#include <regex>

namespace Kezia
{
	Shader::Shader(const U32 shaderId)
		: m_ShaderId(shaderId)
	{}

	Shader::~Shader()						
	{ 
		glDeleteShader(m_ShaderId);
	}

	const U32 Shader::GetShaderId() const
	{
		return m_ShaderId;
	}

	Shader::VariableIterator Shader::UniformBegin() const
	{
		return m_Uniforms.begin();
	}

	Shader::VariableIterator Shader::UniformEnd() const
	{
		return m_Uniforms.end();
	}

	Shader::VariableIterator Shader::AttributeBegin() const
	{
		return m_Attributes.begin();
	}

	Shader::VariableIterator Shader::AttributeEnd() const
	{
		return m_Attributes.end();
	}

	Shader::VariableIterator Shader::OutBegin() const
	{
		return m_OutVariables.begin();
	}

	Shader::VariableIterator Shader::OutEnd() const
	{
		return m_OutVariables.end();
	}

	const U32 Shader::CreateShader(const char * shaderSource, const U32 shaderType)
	{
		GLuint shaderId = glCreateShader(shaderType);

		glShaderSource(shaderId, 1, &shaderSource, NULL);
		glCompileShader(shaderId);

		GLint compileStatus;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

		if(!compileStatus) //compile failure
		{
			int logLength;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

			char * log = new char[logLength];
			glGetShaderInfoLog(shaderId, logLength, &logLength, log);
			std::stringstream logStream(log);

			char path[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, path);

			std::string line;
			while(getline(logStream, line))
			{
				std::size_t mark = line.find_first_of('(');

				if(mark != std::string::npos)
				{
					std::size_t end = line.find_first_of(')', mark);

					int lineNumber = 0;

					std::stringstream extractLine(line.substr(mark + 1, end - mark - 1));
					extractLine >> lineNumber;

					std::stringstream errorStream;
					errorStream << path << "\\" << "rawr" << '(' << lineNumber << ')' <<  line.substr(end + 1) << std::endl;
					OutputDebugString(errorStream.str().c_str());
				}
			}

			MessageBoxA(NULL, log, "Compile failure", MB_OK);

			delete [] log;

			exit(-1);
		}

		return shaderId;
	}

	void Shader::PopulateVariables(const char * shaderSource)
	{
		std::string source(shaderSource);

		std::smatch match;
		std::regex expression("uniform\\s+(\\w+)\\s+(\\w+)\\s*;");

		while(std::regex_search(source, match, expression))
		{
			m_Uniforms.push_back(VariableInfo(match[1].str(), match[2].str()));
			source = match.suffix().str();
		}

		source = std::string(shaderSource);
		expression = std::regex("in\\s+(\\w+)\\s+(\\w+)\\s*;");

		while(std::regex_search(source, match, expression))
		{
			m_Attributes.push_back(VariableInfo(match[1].str(), match[2].str()));
			source = match.suffix().str();
		}

		source = std::string(shaderSource);
		expression = std::regex("out\\s+(\\w+)\\s+(\\w+)\\s*;");

		while(std::regex_search(source, match, expression))
		{
			m_OutVariables.push_back(VariableInfo(match[1].str(), match[2].str()));
			source = match.suffix().str();
		}
	}

	FragmentShader::FragmentShader(const char * shaderSource)
		:	Shader(CreateShader(shaderSource, GL_FRAGMENT_SHADER))
	{
		PopulateVariables(shaderSource);
	}

	VertexShader::VertexShader(const char * shaderSource)
		:	Shader(CreateShader(shaderSource, GL_VERTEX_SHADER))
	{
		PopulateVariables(shaderSource);
	}
}