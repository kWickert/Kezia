#pragma once

#include "Math/Quaternion.h"

#include "OVR.h"
#include "OVRVersion.h"

namespace Kezia
{
	class OculusController
	{
	public:
		OculusController();
		~OculusController();

		void Update();
		const Quaternion<F32> GetOrientation() const;
	private:
		friend class OculusCamera;
		friend class OculusPostProcessPass;

		static bool InitOVR();

		OVR::HMDInfo m_Info;

		OVR::DeviceManager * m_Manager;
		OVR::SensorDevice * m_Sensor;
		OVR::HMDDevice * m_Device;
		OVR::SensorFusion m_Fusion;

		Quaternion<F32> m_Orientation;
	};
}