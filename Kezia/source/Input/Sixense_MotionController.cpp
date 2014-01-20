#include "Sixense_MotionController.h"

#include "Core/Assert.h"

#include "Math/Vector3.h"
#include "Math/Matrix3.h"

namespace Kezia
{
	Sixense_MotionController::Sixense_MotionController(const U32 baseIndex, const U32 controllerIndex)
		:	m_BaseIndex(baseIndex),
			m_ControllerIndex(controllerIndex)
	{}

	Sixense_MotionController::~Sixense_MotionController()
	{}

	Vector3<F32> Sixense_MotionController::GetControllerPosition() const
	{
		return Vector3<F32>(m_ControllerData.pos[0], m_ControllerData.pos[1], m_ControllerData.pos[2]);
	}

	Matrix3<F32> Sixense_MotionController::GetControllerOrientation() const
	{
		return Matrix3<F32>(&m_ControllerData.rot_mat[0][0]);
	}

	const bool Sixense_MotionController::IsConnected() const
	{
		return sixenseIsBaseConnected(m_BaseIndex) != 0;
	}

	Kezia::ButtonState Sixense_MotionController::GetButtonState(const ButtonIndex button) const
	{
		const U32 & buttons = m_ControllerData.buttons;

		bool state;

		switch(button)
		{
		case Sixense_MotionController::Button1:
			state = buttons && SIXENSE_BUTTON_1;
			break;
		case Sixense_MotionController::Button2:
			state = buttons && SIXENSE_BUTTON_2;
			break;
		case Sixense_MotionController::Button3:
			state = buttons && SIXENSE_BUTTON_3;
			break;
		case Sixense_MotionController::Button4:
			state = buttons && SIXENSE_BUTTON_4;
			break;
		case Sixense_MotionController::StartButton:
			state = buttons && SIXENSE_BUTTON_START;
			break;
		case Sixense_MotionController::BumperButton:
			state = buttons && SIXENSE_BUTTON_BUMPER;
			break;
		case Sixense_MotionController::JoystickButton:
			state = buttons && SIXENSE_BUTTON_JOYSTICK;
			break;
		}

		return state ? BUTTON_DOWN : BUTTON_UP;
	}

	const U8 Sixense_MotionController::GetNumberOfJoysticks() const
	{
		return k_NumJoysticks;
	}

	const JoyStickState Sixense_MotionController::GetJoystickState(const JoystickIndex joystick) const
	{
		KeziaAssert(joystick < k_NumJoysticks);

		return JoyStickState(m_ControllerData.joystick_x, m_ControllerData.joystick_y);
	}

	void Sixense_MotionController::Rumble(F64 durationSeconds)
	{
		sixenseTriggerVibration(m_ControllerIndex, static_cast<S32>(durationSeconds / 10), 0);
	}

	const U32 Sixense_MotionController::GetControllerIndex() const
	{
		return m_ControllerIndex;
	}

	void Sixense_MotionController::Update()
	{
		sixenseGetNewestData(m_ControllerIndex, &m_ControllerData);
	}

	void Sixense_MotionController::RegisterButtonEventCallback(ButtonEventCallback callback)
	{
		throw std::exception("sixense controller event callbacks are not supported yet");
	}

	void Sixense_MotionController::RegisterJoystickEventCallback(JoystickEventCallback callback)
	{
		throw std::exception("sixense controller event callbacks are not supported yet");
	}

	void Sixense_MotionController::RegisterMoveEventCallback(MoveEventCallback callback)
	{
		throw std::exception("sixense controller event callbacks are not supported yet");
	}

	void Sixense_MotionController::RegisterRotateEventCallback(RotateEventCallback callback)
	{
		throw std::exception("sixense controller event callbacks are not supported yet");
	}
}