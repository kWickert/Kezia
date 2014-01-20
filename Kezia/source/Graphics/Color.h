#pragma once

#include "Core/Types.h"

#include "Math/Vector4.h"

#include <algorithm>

namespace Kezia
{
	class Color;

	void swap(Color & a, Color & b);

	class Color
	{
	public:
		Color(const F32 greyScale = 0)
			:	r(greyScale),
				g(greyScale),
				b(greyScale),
				a(1)
		{}

		Color(const F32 r, const F32 g, const F32 b, const F32 a = 1)
			:	r(r),
				g(g),
				b(b),
				a(a)
		{}

		Color(const Color & other)
			:	r(other.r),
				g(other.g),
				b(other.b),
				a(other.a)
		{}

		~Color()
		{}

		Color & operator=(const Color & rhs)
		{
			Color tmp(rhs);
			swap(tmp, *this);
			return *this;
		}

		operator Vector4<F32>() const
		{
			return Vector4<F32>(r, g, b, a);
		}

		bool operator==(const Color & rhs) const
		{
			return (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a);
		}

		bool operator!=(const Color & rhs) const
		{
			return !operator==(rhs);
		}

		friend void swap(Color & a, Color & b)
		{
			std::swap(a.r, b.r);
			std::swap(a.g, b.g);
			std::swap(a.b, b.b);
			std::swap(a.a, b.a);
		}

		F32 r;
		F32 g;
		F32 b;
		F32 a;

		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Cyan;
		static const Color Orange;
		static const Color Black;
		static const Color White;
	};

	inline Color CreateColorFromRGB(const U8 r, const U8 g, const U8 b)
	{
		return Color(r / 255.0f, g / 255.0f, b / 255.0f);
	}
}