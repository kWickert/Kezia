#include "EntityTypes.h"

namespace Kezia
{
	std::vector<EntityTypeInfo *> & EntityTypeInfo::GetTypes()
	{
		static std::vector<EntityTypeInfo *> types;
		return types;
	}
}