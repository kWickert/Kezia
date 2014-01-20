#pragma once

#include "Core/Types.h"
#include "Core/TypeList.h"
#include "Core/SuperFastHash.h"

#include "Graphics/Color.h"
#include "Graphics/glInclude.h"
#include "Graphics/Texture.h"

#include "Math/Matrix4.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

#include "Core/WindowsInclude.h"

namespace Kezia
{
	struct glslTypeHashTable
	{
		static const U32 i;
		static const U32 f;

		static const U32 sampler2D;

		static const U32 vec2; 
		static const U32 vec3; 
		static const U32 vec4;

		static const U32 mat4;
	};

	struct glTextureUnitManager
	{
	public:
		static void ResetTextureBindings()
		{
			k_NextAvailableUnit = 0;
		}

		static U32 GetNextAvailableTextureUnit()
		{
			return k_NextAvailableUnit++;
		}

	private:
		static U32 k_NextAvailableUnit;
	};

	template<typename Data_T>
	struct glTranslator
	{
		static const U32 DatumCount;
		static const U32 EnumValue;
		static inline void BindUniform(const GLuint location, const Data_T & value);
	};

	template<>
	struct glTranslator<U32>
	{
		static const U32 DatumCount = 1;
		static const U32 EnumValue = GL_UNSIGNED_INT;
		static inline void BindUniform(const GLuint location, const U32 & value);
	};

	template<>
	struct glTranslator<F32>
	{
		static const U32 DatumCount = 1;
		static const U32 EnumValue = GL_FLOAT;
		static inline void BindUniform(const GLuint location, const F32 & value);
	};

	template<>
	struct glTranslator<Texture>
	{
		static const U32 DatumCount = 1;
		static const U32 EnumValue = GL_UNSIGNED_INT; //not sure about this
		static inline void BindUniform(const GLuint location, const Texture & value);
	};

	template<typename Data_T>
	struct glTranslator< Vector2<Data_T> >
	{
		static const U32 DatumCount = 2;
		static const U32 EnumValue = glTranslator<Data_T>::EnumValue;
		static inline void BindUniform(const GLuint location, const Vector2<Data_T> & value);
	};

	template<typename Data_T>
	struct glTranslator< Vector3<Data_T> >
	{
		static const U32 DatumCount = 3;
		static const U32 EnumValue = glTranslator<Data_T>::EnumValue;
		static inline void BindUniform(const GLuint location, const Vector3<Data_T> & value);
	};

	template<typename Data_T>
	struct glTranslator< Vector4<Data_T> >
	{
		static const U32 DatumCount = 4;
		static const U32 EnumValue = glTranslator<Data_T>::EnumValue;
		static inline void BindUniform(const GLuint location, const Vector4<Data_T> & value);
	};

	template<typename Data_T>
	struct glTranslator< Matrix4<Data_T> >
	{
		static const U32 DatumCount = 16;
		static const U32 EnumValue = glTranslator<Data_T>::EnumValue;
		static inline void BindUniform(const GLuint location, const Matrix4<Data_T> & value);
	};

	//uniform bind
	inline void glTranslator<U32>::BindUniform(const GLuint location, const U32 & value)
	{
		glUniform1i(location, value);
	}

	inline void glTranslator<F32>::BindUniform(const GLuint location, const F32 & value)
	{
		glUniform1f(location, value);
	}

	inline void glTranslator<Texture>::BindUniform(const GLuint location, const Texture & value)
	{
		GLint textureUnit = glTextureUnitManager::GetNextAvailableTextureUnit();
		glActiveTexture(GL_TEXTURE0 + textureUnit);

		glUniform1i(location, textureUnit);
		value.Bind(textureUnit);
	}

	template<>
	inline void glTranslator< Vector2<F32> >::BindUniform(GLuint location, const Vector2<F32> & value)
	{
		glUniform2f(location, value[0], value[1]);
	}

	template<>
	inline void glTranslator< Vector3<F32> >::BindUniform(GLuint location, const Vector3<F32> & value)
	{
		glUniform3f(location, value[0], value[1], value[2]);
	}

	template<>
	inline void glTranslator< Vector4<F32> >::BindUniform(GLuint location, const Vector4<F32> & value)
	{
		glUniform4f(location, value[0], value[1], value[2], value[3]);
	}

	template<>
	inline void glTranslator<Color>::BindUniform(GLuint location, const Color & value)
	{
		glUniform4f(location, value.r, value.g, value.b, value.a);
	}

	template<>
	inline void glTranslator< Matrix4<F32> >::BindUniform(const GLuint location, const Matrix4<F32> & value)
	{
		glUniformMatrix4fv(location, 1, false, static_cast<const F32 *>(value));
	}
}
