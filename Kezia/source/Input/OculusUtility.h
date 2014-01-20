#pragma once

#include <OVR.h>
#include <OVRVersion.h>

namespace Kezia
{
	static inline Quaternion<F32> FromOculusQuaternion(OVR::Quatf q)
	{
		return Quaternion<F32>(q.w, q.x, q.y, q.z);
	}

	struct OculusImplementation
	{
		OculusImplementation()
		{
			manager = OVR::DeviceManager::Create();

			OVR::DeviceEnumerator<OVR::HMDDevice> dEnum = manager->EnumerateDevices<OVR::HMDDevice>();
			std::cout << "devices:\n";
			while(dEnum)
			{
				OVR::DeviceInfo dInfo;
				dEnum.GetDeviceInfo(&dInfo);

				std::cout << "name: " << dInfo.ProductName << std::endl;

				dEnum.Next();
			}

			device = manager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();

			if(device)
			{
				device->GetDeviceInfo(&info);

				sensor = device->GetSensor();

				if(sensor)
				{
					bool success = fusion.AttachToSensor(sensor);

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

		~OculusImplementation()
		{}

		OVR::HMDInfo info;

		OVR::DeviceManager * manager;
		OVR::SensorDevice * sensor;
		OVR::HMDDevice * device;
		OVR::SensorFusion fusion;
	};
}