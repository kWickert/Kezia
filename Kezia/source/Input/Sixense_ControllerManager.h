#pragma once

#include "IControllerManager.h"

#include <vector>

namespace Kezia
{
	class Sixense_MotionController;

	class Sixense_ControllerManager : public IControllerManager
	{
	public:
		Sixense_ControllerManager();
		virtual ~Sixense_ControllerManager();

		void Update();

		virtual IGameController * GetController(const U32 index);
		virtual const U32 GetActiveMotionControllerCount() const;
	private:
		std::vector<Sixense_MotionController *> m_Controllers;
	};

	extern Sixense_ControllerManager * g_MotionControllerManager;
}