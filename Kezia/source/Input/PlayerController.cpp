#include "PlayerController.h"

#include "Core/Pawn.h"
#include "Core/TransformationNode.h"

#include "Graphics/Camera.h"

#include "Math/KeziaTrig.h"

namespace Kezia
{
	PlayerController::PlayerController()
		:	m_Pawn(nullptr),
			m_LookSensitivity(DegreesToRadians(1.0f))
	{}

	PlayerController::~PlayerController()
	{}

	void PlayerController::Update(const F64 deltaTime)
	{}

	void PlayerController::ProcessLookInput(const Vector2<F32> & lookInput)
	{
		Vector2<F32> normalizedInput = Normalize(lookInput);

		if(m_Pawn != nullptr)
		{
			Camera * camera = m_Pawn->GetAttachedCamera();

			if(camera != nullptr)
			{
				camera->Rotate(0, normalizedInput.y() * m_LookSensitivity);
			}

			m_Pawn->GetTransformationNode()->Rotate(-normalizedInput.x() * m_LookSensitivity);
		}
	}

	void PlayerController::ProcessMoveInput(const Vector2<F32> & moveInput)
	{
		if(m_Pawn != nullptr)
		{
			Vector2<F32> normalizedInput = Normalize(moveInput);
			F32 forwardInput = -normalizedInput.y();
			F32 strafeInput = normalizedInput.x();

			Vector3<F32> localMove;
			localMove.z() = forwardInput;
			localMove.x() = strafeInput;

			Vector3<F32> worldMove = Matrix3<F32>(m_Pawn->GetTransformationNode()->GetOrientation()) * localMove;
			worldMove *= m_Pawn->GetMoveSpeed();

			m_Pawn->SetLinearVelocity(worldMove);
		}
	}

	void PlayerController::Posses(Pawn * pawn)
	{
		m_Pawn = pawn;
	}
}