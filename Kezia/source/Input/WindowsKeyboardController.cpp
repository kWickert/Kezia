#include "WindowsKeyboardController.h"

#include "Core/Assert.h"
#include "Core/Keys.h"
#include "Core/WindowsInclude.h"

#include "Event/EventManager.h"
#include "Event/NamedProperties.h"

#include <algorithm>

namespace Kezia
{
	WindowsKeyboardController::WindowsKeyboardController()
	{
		PopulateKeyboardState(m_OldKeyboardState);
	}

	WindowsKeyboardController::~WindowsKeyboardController()
	{}

	void WindowsKeyboardController::Update()
	{
		PopulateKeyboardState(m_OldKeyboardState);
		std::swap(m_CurrentKeyboardState, m_OldKeyboardState);

		for(U32 i = 0; i < 256; i++)
		{
			std::string eventName;

			if(GetKeyState(i, m_CurrentKeyboardState) == BUTTON_DOWN && GetKeyState(i, m_OldKeyboardState) == BUTTON_UP)
			{
				eventName = "keyPress";
			}	
			else if(GetKeyState(i, m_CurrentKeyboardState) == BUTTON_UP && GetKeyState(i, m_OldKeyboardState) == BUTTON_DOWN)
			{
				eventName = "keyRelease";
			}

			if(!eventName.empty())
			{
				NamedProperties args;

				U16 translation;
				S32 translateResult = ToAscii(i, MapVirtualKey(i, MAPVK_VK_TO_VSC), m_CurrentKeyboardState, &translation, 0);
				U8 key = static_cast<U8>(translation);

				if(translateResult == 0)
				{
					if(i >= VK_END && i <= VK_HELP)
					{
						key = char(i - VK_END + SpecialKeyBegin);
					}
				}

				args.SetProperty("key", key);

				g_EventManager->FireEvent(eventName, args);
			}
		}	
	}

	Kezia::ButtonState WindowsKeyboardController::GetButtonState(const ButtonIndex button) const
	{
		return GetKeyState(button, m_CurrentKeyboardState);
	}

	const U8 WindowsKeyboardController::GetNumberOfJoysticks() const
	{
		return 0;
	}

	const JoyStickState WindowsKeyboardController::GetJoystickState(const JoystickIndex joystick) const
	{
		return JoyStickState();
	}

	void WindowsKeyboardController::Rumble(const F64 durationSeconds)
	{
		//no rumble on le keyboard
	}

	void WindowsKeyboardController::RegisterButtonEventCallback(ButtonEventCallback)
	{

	}

	void WindowsKeyboardController::RegisterJoystickEventCallback(JoystickEventCallback)
	{

	}

	const ButtonState WindowsKeyboardController::GetKeyState(const U32 key, const U8 * keyboardState) const
	{
		KeziaAssert(key < 256);
		return (keyboardState[key] & 0x80) != 0 ? BUTTON_DOWN : BUTTON_UP;
	}

	void WindowsKeyboardController::PopulateKeyboardState(U8 * outKeyboardState)
	{
		GetKeyboardState(outKeyboardState);
	}
}