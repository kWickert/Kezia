#pragma once

#include "Math/Vector3.h"

#include <btBulletDynamicsCommon.h>

namespace Kezia
{
	struct BulletComponentImplementation
	{
		BulletComponentImplementation()
			:	rigidBody(nullptr),
				shape(nullptr)
		{}

		~BulletComponentImplementation()
		{
			delete rigidBody->getMotionState();
			delete rigidBody;
			delete shape;
		}

		btRigidBody * rigidBody;
		btCollisionShape * shape;
	};

	struct BulletManagerImplementation
	{
		BulletManagerImplementation()
			:	dynamicsWorld(nullptr),
				solver(nullptr),
				dispatcher(nullptr),
				collisionConfiguration(nullptr),
				broadphase(nullptr)
		{}

		~BulletManagerImplementation()
		{
			delete dynamicsWorld;
			delete solver;
			delete dispatcher;
			delete collisionConfiguration;
			delete broadphase;
		}

		btBroadphaseInterface * broadphase;
		btDefaultCollisionConfiguration * collisionConfiguration;
		btCollisionDispatcher * dispatcher;
		btSequentialImpulseConstraintSolver * solver;
		btDiscreteDynamicsWorld * dynamicsWorld;
	};

	inline btVector3 ToBulletVector(const Vector3<F32> & vector)
	{
		return btVector3(vector[0], vector[1], vector[2]);
	}

	inline Vector3<F32> FromBulletVector(const btVector3 & vector)
	{
		return Vector3<F32>(vector[0], vector[1], vector[2]);
	}

	inline btMatrix3x3 ToBulletMatrix(const Matrix3<F32> & mat)
	{
		return btMatrix3x3(	mat(0, 0), mat(1, 0), mat(2, 0),
							mat(0, 1), mat(1, 1), mat(2, 1),
							mat(0, 2), mat(1, 2), mat(2, 2));
	}

	inline Quaternion<F32> FromBulletQuaternion(const btQuaternion & quat)
	{
		return Quaternion<F32>(quat.w(), quat.x(), quat.y(), quat.z());
	}
}