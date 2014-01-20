#pragma once

#include "Core/Pawn.h"

#include "Math/Vector2.h"

namespace Kezia
{
	class PlayerController
	{
	public:
		PlayerController();
		virtual ~PlayerController();

		virtual void Update(const F64 deltaTime);

		virtual void ProcessLookInput(const Vector2<F32> & lookInput);
		virtual void ProcessMoveInput(const Vector2<F32> & moveInput);

		virtual void Posses(Pawn * pawn);

	protected:
		Pawn * m_Pawn;

		F32 m_LookSensitivity;
	};
}