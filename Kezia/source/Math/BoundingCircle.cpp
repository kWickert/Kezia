#include "BoundingCircle.h"

namespace Kezia
{
	BoundingCircle::BoundingCircle(const Vector2<F32> & position, const F32 radius)
		:	m_Position(position),
			m_Radius(radius)
	{}

	BoundingCircle::~BoundingCircle()
	{}

	const Vector2<F32> & BoundingCircle::GetPosition() const
	{
		return m_Position;
	}

	const F32 BoundingCircle::GetRadius() const
	{
		return m_Radius;
	}

	void BoundingCircle::SetPosition(const Vector2<F32> & position)
	{
		m_Position = position;
	}

	void BoundingCircle::SetRadius(const F32 radius)
	{
		m_Radius = radius;
	}

	bool BoundingCircle::HitTest(const Vector2<F32> & position) const
	{
		return (position - m_Position).GetLengthSquared() < m_Radius * m_Radius;
	}
	
	bool BoundingCircle::CollisionTest(const BoundingCircle & other) const
	{
		return (other.m_Position - m_Position).GetLengthSquared() < (m_Radius + other.m_Radius) * (m_Radius + other.m_Radius);
	}
}