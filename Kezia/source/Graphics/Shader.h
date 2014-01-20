#pragma once

#include "Core/Types.h"

#include "Graphics/glInclude.h"

#include <string>
#include <vector>

namespace Kezia
{
	class Shader
	{
	public:
		struct VariableInfo
		{
			VariableInfo(const std::string & type, const std::string & name)
				:	type(type),
					name(name)
			{}

			std::string type;
			std::string name;
		};

		typedef std::vector<VariableInfo>::const_iterator VariableIterator;

		virtual ~Shader();

		const U32 GetShaderId() const;

		VariableIterator UniformBegin() const;
		VariableIterator UniformEnd() const;
		VariableIterator AttributeBegin() const;
		VariableIterator AttributeEnd() const;
		VariableIterator OutBegin() const;
		VariableIterator OutEnd() const;
		
	protected:
		Shader(const U32 shaderId);

		const U32 CreateShader(const char * shaderSource, const U32 shaderType);
		void PopulateVariables(const char * shaderSource);

		std::vector<VariableInfo> m_Uniforms;
		std::vector<VariableInfo> m_Attributes;
		std::vector<VariableInfo> m_OutVariables;

		const U32 m_ShaderId;
	};

	class FragmentShader : public Shader
	{
	public:
		FragmentShader(const char * shaderSource);
	};

	class VertexShader : public Shader
	{
	public:
		VertexShader(const char * shaderSource);
	};
}
