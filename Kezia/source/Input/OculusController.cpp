#include "OculusController.h"

#include "Math/Quaternion.h"

namespace Kezia
{
	OculusController::OculusController()
	{
		//OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));

		m_Manager = OVR::DeviceManager::Create();

		OVR::DeviceEnumerator<OVR::HMDDevice> device = m_Manager->EnumerateDevices<OVR::HMDDevice>();
		std::cout << "devices:\n";
		while(device)
		{
			OVR::DeviceInfo info;
			device.GetDeviceInfo(&info);

			std::cout << "name: " << info.ProductName << std::endl;

			device.Next();
		}

		m_Device = m_Manager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();

		if(m_Device)
		{
			m_Device->GetDeviceInfo(&m_Info);

			m_Sensor = m_Device->GetSensor();

			if(m_Sensor)
			{
				bool success = m_Fusion.AttachToSensor(m_Sensor);

				std::cout << (success ? "success!" : "failure!") << std::endl;
			}
			else
				std::cout << "couldn't get sensor\n";
		}
		else
		{
			std::cerr << "failed to get device\n";
		}
	}

	OculusController::~OculusController()
	{

	}

	void OculusController::Update()
	{
		OVR::Quatf orient = m_Fusion.GetOrientation();
		m_Orientation = Quaternion<F32>(orient.w, orient.x, orient.y, orient.z);
	}

	const Quaternion<F32> OculusController::GetOrientation() const
	{
		return m_Orientation;
	}
}