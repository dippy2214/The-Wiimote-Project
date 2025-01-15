
#include <Windows.h>
#include <bluetoothapis.h>
#include <hidsdi.h>
#include <setupapi.h>
#include <Cfgmgr32.h>
#include <initguid.h>
#include <devpkey.h>

#include<vector>
#include <iostream>
#include <thread>

#include "Wiimote.h"
#include "Keyboard.h"

static const float tickRate = 60.0f;

bool IsValidDeviceName(const std::string& name)
{
	return (name == "Nintendo RVL-CNT-01" || name == "Nintendo RVL-CNT-01-TR");
}

std::string WStringToString(std::wstring wstring)
{

	std::string output;
	for (wchar_t c : wstring)
	{
		output += char(c);
	}
	std::cout << output << "\n";
	return output;
}

bool ForgetWiimote(BLUETOOTH_DEVICE_INFO_STRUCT& btdi)
{
	//only forget devices that arent connected and are remembered
	if (!btdi.fConnected && btdi.fRemembered)
	{
		BluetoothRemoveDevice(&btdi.Address);
		return true;
	}
	return false;
}

bool AttachWiimote(HANDLE hRadio, const BLUETOOTH_RADIO_INFO& rinfo, BLUETOOTH_DEVICE_INFO_STRUCT& btdi)
{
	if (!btdi.fConnected)
	{
		auto const& wm_addr = btdi.Address.rgBytes;
		std::cout << "Found Wiimote (" << std::hex << static_cast<int>(wm_addr[0]) << ":"
			<< std::hex << static_cast<int>(wm_addr[1]) << ":" 
			<< std::hex << static_cast<int>(wm_addr[2]) << ":"
			<< std::hex << static_cast<int>(wm_addr[3]) << ":"
			<< std::hex << static_cast<int>(wm_addr[4]) << ":"
			<< std::hex << static_cast<int>(wm_addr[5]) << ")\n";

		const DWORD hr = BluetoothSetServiceState(hRadio, &btdi, &HumanInterfaceDeviceServiceClass_UUID, BLUETOOTH_SERVICE_ENABLE);

		if (FAILED(hr))
		{
			std::cout << "Wiimote BluetoothSetServiceState failed: " << hr << "\n";
		}
		else
		{
			return true;
		}
		
	}
	return false;
}

bool GetParentDevice(const DEVINST& child_device_instance, HDEVINFO* parent_device_info, PSP_DEVINFO_DATA parent_device_data)
{
	ULONG status;
	ULONG problem_number;
	CONFIGRET result;

	result = CM_Get_DevNode_Status(&status, &problem_number, child_device_instance, 0);
	if (result != CR_SUCCESS)
	{
		return false;
	}

	DEVINST parent_device;

	result = CM_Get_Parent(&parent_device, child_device_instance, 0);
	if (result != CR_SUCCESS)
	{
		return false;
	}

	std::vector<WCHAR> parent_device_id(MAX_DEVICE_ID_LEN);

	result = CM_Get_Device_ID(parent_device, parent_device_id.data(), (ULONG)parent_device_id.size(), 0);
	if (result != CR_SUCCESS)
	{
		return false;
	}

	(*parent_device_info) = SetupDiCreateDeviceInfoList(nullptr, nullptr);

	if (!SetupDiOpenDeviceInfo((*parent_device_info), parent_device_id.data(), nullptr, 0, parent_device_data))
	{
		SetupDiDestroyDeviceInfoList(parent_device_info);
		return false;
	}

	return true;
}

std::wstring GetDeviceProperty(const HDEVINFO& device_info, const PSP_DEVINFO_DATA device_data, const DEVPROPKEY* requested_property)
{
	DWORD required_size = 0;
	DEVPROPTYPE device_property_type;

	SetupDiGetDeviceProperty(device_info, device_data, requested_property, &device_property_type, nullptr, 0, &required_size, 0);

	std::vector<BYTE> unicode_buffer(required_size, 0);

	BOOL result = SetupDiGetDeviceProperty(device_info, device_data, requested_property, &device_property_type, unicode_buffer.data(), required_size, nullptr, 0);
	if (!result)
	{
		return std::wstring();
	}
	return std::wstring((PWCHAR)unicode_buffer.data());
}

void BluetoothSetup()
{
	//initialising bluetooth search filter parameters
	BLUETOOTH_DEVICE_SEARCH_PARAMS search;
	search.dwSize = sizeof(search);
	//search filters
	search.fReturnAuthenticated = true;
	search.fReturnRemembered = true;
	search.fReturnConnected = true;
	search.fReturnUnknown = true;
	search.fIssueInquiry = true;
	//timeout is a multiple of 1.28 seconds
	search.cTimeoutMultiplier = 2;

	//initialising radio search parameters
	BLUETOOTH_FIND_RADIO_PARAMS radioParams;
	radioParams.dwSize = sizeof(radioParams);

	//initialise handle for radio
	HANDLE hRadio;

	//this function will find the first radio (our commputer probably) and set the handle based on our parameters
	HBLUETOOTH_RADIO_FIND hFindRadio = BluetoothFindFirstRadio(&radioParams, &hRadio);

	//while we are using a valid radio
	while (hFindRadio)
	{
		//initialise radio info
		BLUETOOTH_RADIO_INFO radioInfo;
		radioInfo.dwSize = sizeof(radioInfo);

		//set radio info based on the handle we pass representing our radio
		auto const rInfo_result = BluetoothGetRadioInfo(hRadio, &radioInfo);

		//if valid radio info we continue running
		if (rInfo_result == ERROR_SUCCESS)
		{
			//set the radio handle in bluetooth search parameters
			search.hRadio = hRadio;

			//initialise bluetooth device info
			BLUETOOTH_DEVICE_INFO btdi;
			btdi.dwSize = sizeof(btdi);

			//set bluetooth device info and output result to hFindDevice
			HBLUETOOTH_DEVICE_FIND hFindDevice = BluetoothFindFirstDevice(&search, &btdi);

			//while we have valid device info
			while (hFindDevice)
			{
				std::cout << "Device, Connected: " << btdi.fConnected << " Authenticated: " << btdi.fAuthenticated << " Remembered: " << btdi.fRemembered << "\n";

				//check if this device is a wiimote
				if (IsValidDeviceName(WStringToString(btdi.szName)))
				{
					std::cout << "valid device\n";
					//forget this wii remote to avoid errors then form connection again
					ForgetWiimote(btdi);
					AttachWiimote(hRadio, radioInfo, btdi);

				}

				//close handles if no valid next device, else set btdi to next device's device info
				if (BluetoothFindNextDevice(hFindDevice, &btdi) == false)
				{
					BluetoothFindDeviceClose(hFindDevice);
					hFindDevice = nullptr;
				}
			}
		}

		//if no valid next radio close handles else move on to next radio
		if (!BluetoothFindNextRadio(hFindRadio, &hRadio))
		{
			CloseHandle(hRadio);
			BluetoothFindRadioClose(hFindRadio);
			hFindRadio = nullptr;
		}
	}
}

void ConstantRead(Wiimote* wiimote, bool* isRunning)
{
	std::vector<BYTE> inBuf(22);
	while (*isRunning == true)
	{
		wiimote->Read(inBuf);
	}
}

void HandleInputs(Wiimote* wiimote, Keyboard* keyboard)
{
	//core buttons
	if (wiimote->IsButtonDown(WiimoteButtons::WiimoteButtons::A))
	{
		keyboard->PressKey(KeyboardKeys::A);
	}
	else
	{
		keyboard->ReleaseKey(KeyboardKeys::A);
	}
	if (wiimote->IsButtonDown(WiimoteButtons::WiimoteButtons::B))
	{
		keyboard->PressKey(KeyboardKeys::BACKSPACE);
	}
	else
	{
		keyboard->ReleaseKey(KeyboardKeys::BACKSPACE);
	}
	if (wiimote->IsButtonDown(WiimoteButtons::WiimoteButtons::One))
	{
		keyboard->PressKey(KeyboardKeys::S);
	}
	else
	{
		keyboard->ReleaseKey(KeyboardKeys::S);
	}
	if (wiimote->IsButtonDown(WiimoteButtons::WiimoteButtons::Two))
	{
		keyboard->PressKey(KeyboardKeys::W);
	}
	else
	{
		keyboard->ReleaseKey(KeyboardKeys::W);
	}

	//Dpad
	if (wiimote->IsButtonDown(WiimoteButtons::WiimoteButtons::DpadDown))
	{
		keyboard->PressKey(KeyboardKeys::DOWN_ARROW);
	}
	else
	{
		keyboard->ReleaseKey(KeyboardKeys::DOWN_ARROW);
	}
	if (wiimote->IsButtonDown(WiimoteButtons::WiimoteButtons::DpadUp))
	{
		keyboard->PressKey(KeyboardKeys::UP_ARROW);
	}
	else
	{
		keyboard->ReleaseKey(KeyboardKeys::UP_ARROW);
	}
	if (wiimote->IsButtonDown(WiimoteButtons::WiimoteButtons::DpadLeft))
	{
		keyboard->PressKey(KeyboardKeys::LEFT_ARROW);
	}
	else
	{
		keyboard->ReleaseKey(KeyboardKeys::LEFT_ARROW);
	}
	if (wiimote->IsButtonDown(WiimoteButtons::WiimoteButtons::DpadRight))
	{
		keyboard->PressKey(KeyboardKeys::RIGHT_ARROW);
	}
	else
	{
		keyboard->ReleaseKey(KeyboardKeys::RIGHT_ARROW);
	}
}

int main()
{
	BluetoothSetup();
	
	Wiimote wiimote;
	Keyboard keyboard;

	if (!wiimote.CreateHIDConnection())
	{
		return 0;
	}
	wiimote.GetCaps();

	Sleep(500);

	std::vector<BYTE> input_buffer(22);

	wiimote.SetLEDs();
	wiimote.SetReportMode();
	//wiimote.EnableIRSensor();

	std::cout << "\n";

	int count = 0;
	
	bool isRunning = true;

	std::thread InputThread(ConstantRead, &wiimote, &isRunning);

	//give time to receive first inputs
	Sleep(100);

	//timers for deltaTime
	using clock = std::chrono::high_resolution_clock;
	auto previousTime = clock::now();


	float timePerTick = 1000.0f / tickRate;

	//now we run the main program loop
	while (isRunning)
	{
		//deltaTime calculations
		auto currentTime = clock::now();
		float deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(currentTime - previousTime).count();
		previousTime = currentTime;

		HandleInputs(&wiimote, &keyboard);

		//std::cout << std::dec << wiimote.getAccelX() << ", " << wiimote.getAccelY() << ", " << wiimote.getAccelZ() << "\n";

		//moderate framerate (spamming sendInput severely effects the performance of my PC)
		if (deltaTime < timePerTick)
		{
			Sleep((timePerTick - deltaTime));
		}
	}

	InputThread.join();

	return 0;
}
