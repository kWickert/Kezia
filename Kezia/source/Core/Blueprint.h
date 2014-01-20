#pragma once

#include "Core/Any.h"

#include <vector>

namespace Kezia
{
	class BlueprintBase
	{
	public:
		virtual ~BlueprintBase()
		{}

		virtual const type_info & GetBlueprintType() const = 0;
		virtual void * CreateUntypedInstance() = 0;
	};

	template<typename Archetype_T>
	class Blueprint : public BlueprintBase
	{
	public:
		typedef void (*SpawnFunction)(Archetype_T *, std::vector<Any> &);

		Blueprint(const Archetype_T * archetype)
			: m_Archetype(archetype)
		{}

		virtual ~Blueprint()
		{
			//bad
			//delete m_Archetype;
		}

		virtual const type_info & GetBlueprintType() const
		{
			return typeid(Archetype_T);
		}

		void * CreateUntypedInstance()
		{
			return m_Archetype->Clone();
		}

		Archetype_T * CreateTypedInstance()
		{
			return m_Archetype->Clone();
		}
	private:
		const Archetype_T * m_Archetype;
	};
}