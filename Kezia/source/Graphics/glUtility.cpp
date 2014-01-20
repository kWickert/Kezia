#include "glUtility.h"

namespace Kezia
{
	const U32 glslTypeHashTable::i = SuperFastHash("int");
	const U32 glslTypeHashTable::f = SuperFastHash("float");

	const U32 glslTypeHashTable::sampler2D = SuperFastHash("sampler2D");

	const U32 glslTypeHashTable::vec2 = SuperFastHash("vec2");
	const U32 glslTypeHashTable::vec3 = SuperFastHash("vec3");
	const U32 glslTypeHashTable::vec4 = SuperFastHash("vec4");

	const U32 glslTypeHashTable::mat4 = SuperFastHash("mat4");

	U32 glTextureUnitManager::k_NextAvailableUnit = 0;
}