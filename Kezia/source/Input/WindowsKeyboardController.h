#pragma once

#include "Input/IGameController.h"

namespace Kezia
{
	class WindowsKeyboardController : public IGameController
	{
	public:
		WindowsKeyboardController();
		~WindowsKeyboardController();

		virtual void Update();

		virtual ButtonState GetButtonState(const ButtonIndex button) const;

		virtual const U8 GetNumberOfJoysticks() const;
		virtual const JoyStickState GetJoystickState(const JoystickIndex joystick) const;

		virtual void Rumble(const F64 durationSeconds);

		virtual void RegisterButtonEventCallback(ButtonEventCallback);
		virtual void RegisterJoystickEventCallback(JoystickEventCallback);
	private:
		const ButtonState GetKeyState(const U32 key, const U8 * keyboardState) const;

		void PopulateKeyboardState(U8 * outKeyboardState);

		U8 m_OldKeyboardState[256];
		U8 m_CurrentKeyboardState[256];
	};
}