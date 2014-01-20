#pragma once

#include "IMotionController.h"

#include "Core/Types.h"

#include <sixense.h>

namespace Kezia
{
	class Sixense_MotionController : public IMotionController
	{
	public:
		Sixense_MotionController(const U32 baseIndex, const U32 controllerIndex);
		virtual ~Sixense_MotionController();

		//IController interface
		virtual void Update();

		virtual ButtonState GetButtonState(const ButtonIndex button) const;

		virtual const U8 GetNumberOfJoysticks() const;
		virtual const JoyStickState GetJoystickState(const JoystickIndex joystick) const;

		virtual void Rumble(const F64 durationSeconds);

		virtual void RegisterButtonEventCallback(ButtonEventCallback callback);
		virtual void RegisterJoystickEventCallback(JoystickEventCallback callback);

		//IMotionController interface
		virtual Vector3<F32> GetControllerPosition() const;
		virtual Matrix3<F32> GetControllerOrientation() const;

		virtual const bool IsConnected() const;

		virtual void RegisterMoveEventCallback(MoveEventCallback callback);
		virtual void RegisterRotateEventCallback(RotateEventCallback callback);

		const U32 GetControllerIndex() const;

		static const U8 Button1 = 0;
		static const U8 Button2 = 1;
		static const U8 Button3 = 2;
		static const U8 Button4 = 3;
		static const U8 StartButton = 4;
		static const U8 BumperButton = 5;
		static const U8 JoystickButton = 6;
	private:
		static const U8 k_NumJoysticks = 1;

		const U32 m_ControllerIndex;
		const U32 m_BaseIndex;
		
		sixenseControllerData m_ControllerData;
	};
}