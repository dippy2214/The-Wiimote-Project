#include "Wiimote.h"



bool Wiimote::CreateHIDConnection()
{
	//initialise GUID of device
	GUID device_id;
	HidD_GetHidGuid(&device_id);

	//this gets a handle to the device information set for all the HID devices, basically a list of them
	HDEVINFO const device_info = SetupDiGetClassDevs(&device_id, nullptr, nullptr, (DIGCF_DEVICEINTERFACE | DIGCF_PRESENT));

	//initialise device interface information structure
	SP_DEVICE_INTERFACE_DATA device_data = {};
	device_data.cbSize = sizeof(device_data);

	bool isWiimote = false;
	wiimoteHandle = INVALID_HANDLE_VALUE;

	//loop through all device interfaces in the device information set defined earlier (puts inforation about given device in device_info)
	for (int index = 0; SetupDiEnumDeviceInterfaces(device_info, nullptr, &device_id, index, &device_data); index++)
	{
		//get buffer size required for device detail and intialise buffers
		DWORD len;
		SetupDiGetDeviceInterfaceDetail(device_info, &device_data, nullptr, 0, &len, nullptr);
		auto detail_data_buf = std::make_unique<std::uint8_t[]>(len);
		auto detail_data = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(detail_data_buf.get());
		detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		//device info data buffer intialised
		SP_DEVINFO_DATA device_info_data = {};
		device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);

		//retrive device info details
		if (SetupDiGetDeviceInterfaceDetail(device_info, &device_data, detail_data, len, nullptr, &device_info_data))
		{
			//get the device path
			std::basic_string<TCHAR> device_path(detail_data->DevicePath);
			std::cout << device_path.c_str() << "\n";

			//initialise hardware ID buffer and get the hardware ID from the device
			TCHAR hardware_id[256];
			if (SetupDiGetDeviceRegistryProperty(device_info, &device_info_data, SPDRP_HARDWAREID, nullptr, (BYTE*)hardware_id, sizeof(hardware_id), nullptr))
			{
				//get the wstring from hardwareID
				std::wstring hw_id_str(hardware_id);

				//filter to get the vendor ID and product ID of device
				//search structured to work for bluetooth HID format of hardware ID, not USB (which uses _ instead of & and has less characters to skip on the vendor ID)
				size_t vid_pos = hw_id_str.find(L"VID&");
				size_t pid_pos = hw_id_str.find(L"PID&");

				//if we find the vendor and product IDs, output them and check if they fit the wiimote vendor and product IDs
				if (vid_pos != std::wstring::npos && pid_pos != std::wstring::npos)
				{
					std::wstring vid = hw_id_str.substr(vid_pos + 8, 4);
					std::wstring pid = hw_id_str.substr(pid_pos + 4, 4);
					std::wcout << L"Vendor ID (VID): " << vid << L"\n";
					std::wcout << L"Product ID (PID): " << pid << L"\n";

					if (vid == L"057e" && (pid == L"0306" || pid == L"0330"))
					{
						isWiimote = true;
					}
					else
					{
						isWiimote = false;
					}
				}
				else
				{
					std::wcout << hw_id_str << L"\n";
					std::cout << "hardware IDs not found\n";
					isWiimote = false;
				}


			}

			//if a wii remote has been found
			if (isWiimote)
			{
				//open a connection to the HID device path using the createfile function
				wiimoteHandle = CreateFile(device_path.c_str(),			//specify device path
					GENERIC_READ | GENERIC_WRITE,		//read write access
					FILE_SHARE_READ | FILE_SHARE_WRITE, //read write shared access
					nullptr,							//no security lmao
					OPEN_EXISTING,						//open existing file is one already exists
					FILE_FLAG_OVERLAPPED,				//async I/O
					nullptr								//no template file
				);
			}

			//check for invalid handle
			if (wiimoteHandle == INVALID_HANDLE_VALUE)
			{
				std::cout << "error connecting to device\n";
			}
			else
			{
				std::cout << "device connection successfully opened\n";
			}
		}
	}

	//close device info list
	SetupDiDestroyDeviceInfoList(device_info);

	//check for invalid device handle
	if (wiimoteHandle == INVALID_HANDLE_VALUE)
	{
		//close handle without matching device found and end the program
		CloseHandle(wiimoteHandle);
		std::cout << "no matching device found\n";
		return false;
	}

	std::cout << "wiimote found\n\n";
	return true;
}

void Wiimote::GetCaps()
{
	PHIDP_PREPARSED_DATA preparsed_data = nullptr;
	HIDP_CAPS caps;

	//get unparsed data from wii remote
	if (HidD_GetPreparsedData(wiimoteHandle, &preparsed_data))
	{
		//check unparsed data for the size caps and store them in caps
		if (HidP_GetCaps(preparsed_data, &caps) == HIDP_STATUS_SUCCESS)
		{
			std::wcout << L"Input Report Size: " << caps.InputReportByteLength << L"\n";
			std::wcout << L"Output Report Size: " << caps.OutputReportByteLength << L"\n";
			std::wcout << L"Feature Report Size: " << caps.FeatureReportByteLength << L"\n";
		}
		else
		{
			std::cout << "failed to get report size data\n";
		}
	}
	else
	{
		std::cout << "failed to get preparsed data\n";
	}

	outputReportSize = caps.OutputReportByteLength;
	HidD_FreePreparsedData(preparsed_data);
}

void Wiimote::SetLEDs()
{
	//0x11 = setting LEDs
	//0x10 = LED settings
	std::vector<BYTE> outBuf({ 0x11, 0x10 });
	Write(outBuf);
}

void Wiimote::SetReportMode()
{
	// 0x12 = data report mode
	// 0x00 = non continuous reporting (only reports when change happens)
	// 0x30 = report mode
	std::vector<BYTE> outBuf({ 0x12, 0x00, 0x30 });
	Write(outBuf);
}


bool Wiimote::IsButtonDown(WiimoteButtons button)
{
	bool out = false;

	switch (button)
	{
	case WiimoteButtons::A:
	{
		if (lastInputs[2] & ButtonBitMasks::A)
		{
			std::cout << std::hex << lastInputs[2];
			out = true;
			
		}
		break;
	}
	case WiimoteButtons::B:
	{
		if (lastInputs[2] & ButtonBitMasks::B)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::One:
	{
		if (lastInputs[2] & ButtonBitMasks::One)
		{
			out = true;

		}
		break;
	}
	case WiimoteButtons::Two:
	{
		if (lastInputs[2] & ButtonBitMasks::Two)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::DpadUp:
	{
		if (lastInputs[1] & ButtonBitMasks::DpadUp)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::DpadDown:
	{
		if (lastInputs[1] & ButtonBitMasks::DpadDown)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::DpadRight:
	{
		if (lastInputs[1] & ButtonBitMasks::DpadRight)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::DpadLeft:
	{
		if (lastInputs[1] & ButtonBitMasks::DpadLeft)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::Plus:
	{
		if (lastInputs[1] & ButtonBitMasks::Plus)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::Minus:
	{
		if (lastInputs[2] & ButtonBitMasks::Minus)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::Home:
	{
		if (lastInputs[2] & ButtonBitMasks::Home)
		{
			out = true;
		}
		break;
	}
	}
	return out;
}

bool Wiimote::Write(std::vector<BYTE> outputBuffer)
{
	DWORD bytes_written = 0;
	OVERLAPPED overlapped;
	ZeroMemory(&overlapped, sizeof(overlapped)); 

	outputBuffer.resize(outputReportSize);
	
	if (!WriteFile(wiimoteHandle, outputBuffer.data(), outputBuffer.size(), &bytes_written, &overlapped))
	{
		DWORD error = GetLastError();
		if (error != ERROR_IO_PENDING)
		{
			std::cout << "failed to send output report\n";
			std::cout << GetLastError() << "\n" << outputBuffer.size() << ", " << bytes_written << "\n";
			return false;

		}
	}
	if (GetOverlappedResult(wiimoteHandle, &overlapped, &bytes_written, true))
	{
		//std::wcout << L"sent bytes to the device\n";
		return true;
	}
}

bool Wiimote::Read(std::vector<BYTE> inputBuffer)
{
	DWORD bytes_read = 0;
	OVERLAPPED overlapped;
	ZeroMemory(&overlapped, sizeof(overlapped));

	inputBuffer.resize(22);

	if (!ReadFile(wiimoteHandle, inputBuffer.data(), inputBuffer.size(), &bytes_read, &overlapped))
	{
		DWORD error = GetLastError();
		if (error != ERROR_IO_PENDING)
		{
			std::cout << "failed to read input\n";
			return false;
		}

	}
	if (GetOverlappedResult(wiimoteHandle, &overlapped, &bytes_read, true))
	{
		std::wcout << L"recieved " << bytes_read << L" bytes: ";
		for (BYTE byte : inputBuffer)
		{
			std::wcout << std::hex << static_cast<int>(byte) << L" ";
		}
		std::cout << "\n";
		lastInputs = inputBuffer;
		return true;
	}
	
}
