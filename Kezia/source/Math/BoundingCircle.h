#pragma once

#include "Core/Types.h"
#include "Math/Vector2.h"

namespace Kezia
{
	class BoundingCircle
	{
	public:
		BoundingCircle(const Vector2<F32> & position, const F32 radius);
		~BoundingCircle();

		const Vector2<F32> & GetPosition() const;
		const F32 GetRadius() const;

		void SetPosition(const Vector2<F32> & position);
		void SetRadius(const F32 radius);
	
		bool HitTest(const Vector2<F32> & position) const;
		bool CollisionTest(const BoundingCircle & other) const;
		
	private:
		Vector2<F32> m_Position;
		F32 m_Radius;
	};
}