#pragma once

#include "IGameController.h"

#include "Math/Matrix3.h"
#include "Math/Vector3.h"

namespace Kezia
{
	class IMotionController;

	typedef void (*MoveEventCallback)(IMotionController *, const Vector3<F32> &);
	typedef void (*RotateEventCallback)(IMotionController *, const Matrix3<F32> &);

	class IMotionController : public IGameController
	{
	public:
		virtual Vector3<F32> GetControllerPosition() const = 0;
		virtual Matrix3<F32> GetControllerOrientation() const = 0;

		virtual const bool IsConnected() const = 0;

		virtual void RegisterMoveEventCallback(MoveEventCallback callback) = 0;
		virtual void RegisterRotateEventCallback(RotateEventCallback callback) = 0;
	};
}