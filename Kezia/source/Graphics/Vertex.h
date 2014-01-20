#pragma once

#include "Graphics/Color.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace Kezia
{
	struct Vertex
	{
		Vertex(const Vector3<F32> & position = Vector3<F32>(), const Vector2<F32> & texureCoordinates = Vector2<F32>(), const Vector3<F32> & normal = Vector3<F32>(), const Color & color = Color())
			:	position(position),
				textureCoordinates(texureCoordinates),
				normal(normal),
				color(color)
		{}

		~Vertex()
		{}

		Vector3<F32> position;
		Vector2<F32> textureCoordinates;
		Vector3<F32> normal;
		Color color;
	};
}