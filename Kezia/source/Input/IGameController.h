#pragma once

#include "Core/Types.h"

#include "Math/Vector2.h"

namespace Kezia
{
	typedef U32 ButtonIndex;
	typedef U32 JoystickIndex;

	enum ButtonState
	{
		BUTTON_UP,
		BUTTON_DOWN
	};

	typedef Vector2<F32> JoyStickState;
	
	class IGameController;

	typedef void (*ButtonEventCallback)(const IGameController *, const ButtonIndex, const ButtonState);
	typedef void (*JoystickEventCallback)(const IGameController *, const JoystickIndex, const JoyStickState);

	class IGameController
	{
	public:
		virtual void Update() = 0;

		virtual ButtonState GetButtonState(const ButtonIndex button) const = 0;

		virtual const U8 GetNumberOfJoysticks() const = 0;
		virtual const JoyStickState GetJoystickState(const JoystickIndex joystick) const = 0;

		virtual void Rumble(const F64 durationSeconds) = 0;

		virtual void RegisterButtonEventCallback(ButtonEventCallback) = 0;
		virtual void RegisterJoystickEventCallback(JoystickEventCallback) = 0;
	};
}