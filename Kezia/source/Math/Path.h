#pragma once

#include "Spline.h"
#include <Math/Vector2.h>

#include <vector>

namespace Kezia
{

	template<typename T>
	class Path
	{
	public:
		Path(T beginPoint, T beginDirection)
			:	m_BeginPoint(beginPoint),
				m_BeginTangent(beginDirection),
				m_Length(0.0f)
		{}

		Path(const Path & other)
			:	m_Path(other.m_Path)
		{}

		Path & operator=(const Path & rhs)
		{
			Path tmp(rhs);
			swap(tmp, *this);
			return *this;
		}

		friend void swap(Path & a, Path & b)
		{
			swap(a.m_Path, b.m_Path);
			std::swap(a.m_Length, b.m_Length);
		}

		~Path()
		{}

		T Interpolate(F16 t) const
		{
			F16 realT = t * m_Length;

			U32 i = static_cast<U32>(realT);

			T point = m_Path[i].Interpolate(realT - i);
			return point;
		}

		void PushControlPoint(const T & point, const T & direction)
		{
			if(m_Path.size() == 0)
			{
				m_Path.push_back(Spline<T>(m_BeginPoint, m_BeginTangent, point, direction));
			}
			else
			{
				Spline<T> & s = m_Path.back();
				m_Path.push_back(Spline<T>(s.GetEndPoint(), s.GetEndTangent(), point, direction));
			}
			
			m_Length += 1.0f;
		}


	private:
		T m_BeginPoint;
		T m_BeginTangent;

		F16 m_Length;

		std::vector<Spline<T> > m_Path;
	};
}