#pragma once

#include "Graphics/Color.h"

#include "Math/Vector3.h"

#include <algorithm>
#include <vector>

namespace Kezia
{
	inline U32 GenerateCubeVertices(std::vector< Vector3<F32> > & outVertices, std::vector< Vector3<F32> > & outNormals = std::vector< Vector3<F32> >(), std::vector<U32> & outIndices = std::vector<U32>(), std::vector<Color> & outColors = std::vector<Color>())
	{
		static std::vector< Vector3<F32> > unitBoxVertices;
		static std::vector< Vector3<F32> > unitBoxNormals;
		static std::vector<U32> unitBoxIndices;
		static std::vector<Color> unitBoxColors;
		
		if(unitBoxVertices.empty())
		{
			const F32 min = -0.5f;
			const F32 max = 0.5f;

			for(U32 normalAxis = 0; normalAxis < 3; normalAxis++)
			{
				U32 faceAxis1 = (normalAxis + 1) % 3;
				U32 faceAxis2 = (faceAxis1 + 1) % 3;

				for(F32 normalAxisValue = min; normalAxisValue <= max; normalAxisValue += (max - min))
				{
					Vector3<F32> cursor;
					cursor[normalAxis] = normalAxisValue;

					cursor[faceAxis1] = min;
					cursor[faceAxis2] = min;
					Vector3<F32> bl(cursor);

					cursor[faceAxis2] = max;
					Vector3<F32> ul(cursor);

					cursor[faceAxis1] = max;
					Vector3<F32> ur(cursor);

					cursor[faceAxis2] = min;
					Vector3<F32> br(cursor);

					unitBoxVertices.push_back(bl);
					unitBoxVertices.push_back(ul);
					unitBoxVertices.push_back(br);
					unitBoxVertices.push_back(ur);

					Vector3<F32> normal;
					normal[normalAxis] = normalAxisValue;
					unitBoxNormals.push_back(normal);
					unitBoxNormals.push_back(normal);
					unitBoxNormals.push_back(normal);
					unitBoxNormals.push_back(normal);

					U32 indexStart = unitBoxVertices.size() - 4;
					unitBoxIndices.push_back(indexStart);
					unitBoxIndices.push_back(indexStart + 2);
					unitBoxIndices.push_back(indexStart + 1);
					unitBoxIndices.push_back(indexStart + 1);
					unitBoxIndices.push_back(indexStart + 2);
					unitBoxIndices.push_back(indexStart + 3);
				}	
			}
		

			unitBoxColors.resize(24, Color());
			
			/*
			unitBoxIndices.push_back(0);
			unitBoxIndices.push_back(2);
			unitBoxIndices.push_back(1);
			unitBoxIndices.push_back(1);
			unitBoxIndices.push_back(2);
			unitBoxIndices.push_back(3);

			unitBoxIndices.push_back(2);
			unitBoxIndices.push_back(6);
			unitBoxIndices.push_back(3);
			unitBoxIndices.push_back(3);
			unitBoxIndices.push_back(6);
			unitBoxIndices.push_back(7);		

			unitBoxIndices.push_back(4);
			unitBoxIndices.push_back(0);
			unitBoxIndices.push_back(1);
			unitBoxIndices.push_back(4);
			unitBoxIndices.push_back(1);
			unitBoxIndices.push_back(5);

			unitBoxIndices.push_back(4);
			unitBoxIndices.push_back(6);
			unitBoxIndices.push_back(2);
			unitBoxIndices.push_back(2);
			unitBoxIndices.push_back(4);
			unitBoxIndices.push_back(0);

			unitBoxIndices.push_back(1);
			unitBoxIndices.push_back(3);
			unitBoxIndices.push_back(5);
			unitBoxIndices.push_back(5);
			unitBoxIndices.push_back(3);
			unitBoxIndices.push_back(7);				

			unitBoxIndices.push_back(6);
			unitBoxIndices.push_back(4);
			unitBoxIndices.push_back(7);
			unitBoxIndices.push_back(7);
			unitBoxIndices.push_back(4);
			unitBoxIndices.push_back(5);
			*/
		}

		outVertices = unitBoxVertices;
		outIndices = unitBoxIndices;
		outColors = unitBoxColors;
		outNormals = unitBoxNormals;

		return outVertices.size();
	}

	inline U32 GenerateSphereVertices(std::vector< Vector3<F32> > & outVertices, std::vector<U32> & outIndices, std::vector< Vector3<F32> > & outNormals)
	{
		typedef Vector3<U32> Triangle;

		static std::vector< Vector3<F32> > unitSphereVertices;
		static std::vector< Vector3<F32> > unitSphereNormals;
		static std::vector<U32> unitSphereIndices;
		static std::vector<Color> unitSphereColors;

		if(unitSphereVertices.empty())
		{
			F32 t = (1.0f + sqrt(5.0f)) / 2.0f;

			unitSphereVertices.push_back(Normalize(Vector3<F32>(	-1.0f,	t,	0.0f)) * 0.5f);
			unitSphereVertices.push_back(Normalize(Vector3<F32>(	1.0f,	t,	0.0f)) * 0.5f);
			unitSphereVertices.push_back(Normalize(Vector3<F32>(	-1.0f,	-t,	0.0f)) * 0.5f);
			unitSphereVertices.push_back(Normalize(Vector3<F32>(	1.0f,	-t,	0.0f)) * 0.5f);
										 
			unitSphereVertices.push_back(Normalize(Vector3<F32>(	0.0f,	-1.0f,	t)) * 0.5f);
			unitSphereVertices.push_back(Normalize(Vector3<F32>(	0.0f,	1.0f,	t)) * 0.5f);
			unitSphereVertices.push_back(Normalize(Vector3<F32>(	0.0f,	-1.0f,	-t)) * 0.5f);
			unitSphereVertices.push_back(Normalize(Vector3<F32>(	0.0f,	1.0f,	-t)) * 0.5f);
										
			unitSphereVertices.push_back(Normalize(Vector3<F32>(	t,	0.0f,	-1.0f)) * 0.5f);
			unitSphereVertices.push_back(Normalize(Vector3<F32>(	t,	0.0f,	1.0f)) * 0.5f);
			unitSphereVertices.push_back(Normalize(Vector3<F32>(	-t,	0.0f,	-1.0f)) * 0.5f);
			unitSphereVertices.push_back(Normalize(Vector3<F32>(	-t,	0.0f,	1.0f)) * 0.5f);

			std::vector<Triangle> faces;

			faces.push_back(Triangle(0,	11,	5));
			faces.push_back(Triangle(0,	5,	1));
			faces.push_back(Triangle(0,	1,	7));
			faces.push_back(Triangle(0,	7,	10));
			faces.push_back(Triangle(0,	10, 11));

			faces.push_back(Triangle(1, 5, 9));
			faces.push_back(Triangle(5, 11, 4));
			faces.push_back(Triangle(11, 10, 2));
			faces.push_back(Triangle(10, 7, 6));
			faces.push_back(Triangle(7, 1, 8));

			faces.push_back(Triangle(3, 9, 4));
			faces.push_back(Triangle(3, 4, 2));
			faces.push_back(Triangle(3, 2, 6));
			faces.push_back(Triangle(3, 6, 8));
			faces.push_back(Triangle(3, 8, 9));

			faces.push_back(Triangle(4, 9, 5));
			faces.push_back(Triangle(2, 4, 11));
			faces.push_back(Triangle(6, 2, 10));
			faces.push_back(Triangle(8, 6, 7));
			faces.push_back(Triangle(9, 8, 1));

			U32 numberOfSubdivisions = 2;
			for(U32 i = 0; i < numberOfSubdivisions; i++)
			{
				std::vector<Triangle> subdividedFaces;

				for(auto it = faces.begin(); it != faces.end(); ++it)
				{
					Triangle & face = *it;

					U32 i0 = face[0];
					U32 i1 = face[1];
					U32 i2 = face[2];

					Vector3<F32> v0 = unitSphereVertices[i0];
					Vector3<F32> v1 = unitSphereVertices[i1];
					Vector3<F32> v2 = unitSphereVertices[i2];					

					//TODO check for duplicates
					Vector3<F32> v01 = Normalize(v0 + v1) * 0.5f;
					U32 i01 = unitSphereVertices.size();
					unitSphereVertices.push_back(v01);
					
					Vector3<F32> v12 = Normalize(v1 + v2) * 0.5f;
					U32 i12 = unitSphereVertices.size();
					unitSphereVertices.push_back(v12);

					Vector3<F32> v20 = Normalize(v2 + v0) * 0.5f;
					U32 i20 = unitSphereVertices.size();
					unitSphereVertices.push_back(v20);

					subdividedFaces.push_back(Triangle(i0, i01, i20));
					subdividedFaces.push_back(Triangle(i1, i12, i01));
					subdividedFaces.push_back(Triangle(i2, i20, i12));
					subdividedFaces.push_back(Triangle(i01, i12, i20));
				}

				faces = subdividedFaces;
			}

			//unroll the indices
			for(auto it = faces.begin(); it != faces.end(); ++it)
			{
				Triangle & face = *it;

				unitSphereIndices.push_back(face[0]);
				unitSphereIndices.push_back(face[1]);
				unitSphereIndices.push_back(face[2]);
			}

			unitSphereColors = std::vector<Color>(unitSphereVertices.size(), Color());

			for(auto it = unitSphereVertices.begin(); it != unitSphereVertices.end(); ++it)
			{
				unitSphereNormals.push_back(Normalize(*it));
			}
		}

		outVertices = unitSphereVertices;
		outIndices = unitSphereIndices;
		outNormals = unitSphereNormals;
		//outColors = unitSphereColors;

		return unitSphereVertices.size();
	}
}