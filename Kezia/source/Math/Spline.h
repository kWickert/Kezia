#pragma once

#include <Math/Vector2.h>

namespace Kezia
{
	//templated on vector type
	template<typename T>
	class Spline
	{
	public:
		Spline(const T & beginPoint, const T & beginTangent, const T & endPoint, const T & endTangent)
			:	m_BeginPoint(beginPoint),
				m_BeginTangent(beginTangent),
				m_EndPoint(endPoint),
				m_EndTangent(endTangent)
		{}

		Spline(const Spline & other)
			:	m_BeginPoint(other.m_BeginPoint),
				m_BeginTangent(other.m_BeginTangent),
				m_EndPoint(other.m_EndPoint),
				m_EndTangent(other.m_EndTangent)
		{}

		Spline & operator=(const Spline & rhs)
		{
			Spline tmp(rhs);
			swap(tmp, *this);
			return *this;
		}

		friend void swap(Spline & a, Spline & b)
		{
			swap(a.m_BeginPoint, b.m_BeginPoint);
			swap(a.m_BeginTangent, b.m_BeginTangent);
			swap(a.m_EndPoint, b.m_EndPoint);
			swap(a.m_EndTangent, b.m_EndTangent);
		}
		
		~Spline()
		{}

		T Interpolate(const F16 t) const
		{
			F16 t2 = t * t;
			F16 t3 = t * t * t;

			return (2 * t3 - 3 * t2 + 1) * m_BeginPoint + (t3 - 2 * t2 + t) * m_BeginTangent + (-2 * t3 + 3 * t2) * m_EndPoint + (t3 - t2) * m_EndTangent;
		}

		const T GetBeginPoint() const
		{
			return m_BeginPoint;
		}

		const T GetBeginTangent() const
		{
			return m_BeginTangent;
		}

		const T GetEndPoint() const
		{
			return m_EndPoint;
		}

		const T GetEndTangent() const
		{
			return m_EndTangent;
		}
	private:
		T m_BeginPoint;
		T m_BeginTangent;

		T m_EndPoint;
		T m_EndTangent;
	};
}