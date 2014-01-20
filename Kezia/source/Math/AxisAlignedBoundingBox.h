#pragma once

#include <Math\Vector3.h>

namespace Kezia
{
	template<typename T>
	class AxisAlignedBoundingBox
	{
	public:
		AxisAlignedBoundingBox(const Vector3<T> & location, const T width, const T length, const T height)
			:	m_Location(location),
				m_Width(width),
				m_Length(length),
				m_Height(height)
		{

		}

		AxisAlignedBoundingBox(const AxisAlignedBoundingBox & other)
			:	m_Location(other.m_Location),
				m_Width(other.m_Width),
				m_Length(other.m_Length),
				m_Height(other.m_Height)
		{

		}

		~AxisAlignedBoundingBox()
		{

		}

		AxisAlignedBoundingBox operator=(const AxisAlignedBoundingBox & rhs)
		{
			AxisAlignedBoundingBox tmp(rhs);
			swap(tmp, *this);
			return *this;
		}

		friend void swap(AxisAlignedBoundingBox & a, AxisAlignedBoundingBox & b)
		{
			using std::swap;

			swap(a.m_Location, b.m_Location);
			swap(a.m_Width, b.m_Width);
			swap(a.m_Length, b.m_Length);
			swap(a.m_Height, b.m_Height);
		}

		bool IsInside(const Vector3<T> & point) const
		{
			static Vector3<T> dimension(m_Width, m_Length, m_Height);
			static Vector3<T> max = m_Location + (dimension / 2);
			static Vector3<T> min = m_Location - (dimension / 2);

			bool inside = true;
			for(U32 i = 0; i < 3; i++)
			{
				inside = inside && (m_Location[i] <= max[i] && m_Location[i] >= min[i]);

				if(!inside)
					break;
			}

			return inside;
		}

		void SetLocation(const Vector3<T> & location)
		{
			m_Location = location;
		}

		const Vector3<T> & GetLocation() const
		{
			return m_Location;
		}

		const T GetWidth() const
		{
			return m_Width;
		}

		void SetWidth(const T width)
		{
			m_Width = width;
		}

		const T GetLength() const
		{
			return m_Length;
		}

		void SetLength(const T length)
		{
			m_Length = length;
		}

		const T GetHeight() const
		{
			return m_Height;
		}
		
		void SetHeight(const T height)
		{
			m_Height = height;
		}

	private:
		Vector3<T> m_Location;

		T m_Width;
		T m_Height;
		T m_Length;
	};
}