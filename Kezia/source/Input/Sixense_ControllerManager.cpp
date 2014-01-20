#include "Sixense_ControllerManager.h"

#include "Sixense_MotionController.h"

#include <sixense.h>

namespace Kezia
{
	Sixense_ControllerManager::Sixense_ControllerManager()
	{
		m_Controllers.resize(sixenseGetMaxControllers(), nullptr);

		if(sixenseInit() == SIXENSE_FAILURE)
			throw std::exception("Could not initialize sixense");

		sixenseSetActiveBase(0);
	}

	Sixense_ControllerManager::~Sixense_ControllerManager()
	{
		sixenseExit();
	}

	void Sixense_ControllerManager::Update()
	{
		for(auto controller = m_Controllers.begin(); controller != m_Controllers.end(); ++controller)
		{
			Sixense_MotionController * c = *controller;

			if(c != nullptr)
				c->Update();
		}
	}

	IGameController * Sixense_ControllerManager::GetController(const U32 index)
	{
		if(index > m_Controllers.size())
			return nullptr;
		else if(m_Controllers[index] == nullptr)
		{
			m_Controllers[index] = new Sixense_MotionController(0, index);
			m_Controllers[index]->Update();
		}

		return m_Controllers[index];
	}

	const U32 Sixense_ControllerManager::GetActiveMotionControllerCount() const
	{
		return m_Controllers.size();
	}
}