#include "Wiimote.h"


//create HID connection to a wii remote with an existing bluetooth connection to this computer
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

//get the report sizes of the wiimote
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

//set LEDs on wii remote
void Wiimote::SetLEDs()
{
	//0x11 = setting LEDs
	//0x10 = LED settings
	std::vector<BYTE> outBuf({ 0x11, 0x10 });
	Write(outBuf);
}

//set report mode of wii remote 
void Wiimote::SetReportMode()
{
	// 0x12 = data report mode
	// 0x00 = non continuous reporting (only reports when change happens)
	// 0x30 = report mode
	std::vector<BYTE> outBuf({ 0x12, 0x00, 0x33 });
	Write(outBuf);
}

//this code enables wii level 3 sensitivity for the IR sensor
void Wiimote::EnableIRSensor()
{	
	//enable IR camera toggles
	std::vector<BYTE> outBuf({ 0x13, 0x04 });
	Write(outBuf);
	//sleep for 50 between transmissions to avoid random state weirdness
	Sleep(50);
	outBuf = { 0x1a, 0x04 };
	Write(outBuf);
	
	//write to buffers
	//inform start
	Sleep(50);
	outBuf = { 0x16, /*set to control registers*/ 0x04,
		/*address space to write to*/ 0xb0, 0x00, 0x30, 
		/*Size to write in bytes*/ 0x01,
		/*data being written*/ 0x08 };
	Write(outBuf);

	//data block 1
	Sleep(50);
	outBuf = { 0x16, /*set to control registers*///0x04,
		/*address space to write to*/ 0xb0, 0x00, 0x00, 
		/*Size to write in bytes*/ 0x09,
		/*data being written*/ 0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0xaa, 0x00, 0x64};
	Write(outBuf);

	//data block 2
	Sleep(50);
	outBuf = { 0x16, /*set to control registers*///0x04,
		/*address space to write to*/ 0xb0, 0x00, 0x1a,
		/*Size to write in bytes*/ 0x02,
		/*data being written*/ 0x63, 0x03 };
	Write(outBuf);

	//IR sensor mode
	Sleep(50);
	outBuf = { 0x16, /*set to control registers*/0x04,
		/*address space to write to*/ 0xb0, 0x00, 0x33,
		/*Size to write in bytes*/ 0x01,
		/*data being written*/ 0x03 };
	Write(outBuf);

	////inform end
	Sleep(50);
	outBuf = { 0x16, /*set to control registers*/0x04,
		/*address space to write to*/ 0xb0, 0x00, 0x30,
		/*Size to write in bytes*/ 0x01,
		/*data being written*/ 0x08 };
	Write(outBuf);

}

//check if a given button is down on the last report in the wii remote
bool Wiimote::IsButtonDown(WiimoteButtons::WiimoteButtons button)
{
	bool out = false;

	switch (button)
	{
	case WiimoteButtons::WiimoteButtons::A:
	{
		if (lastInputs[2] & ButtonBitMasks::ButtonBitMasks::A)
		{
			out = true;			
		}
		break;
	}
	case WiimoteButtons::WiimoteButtons::B:
	{
		if (lastInputs[2] & ButtonBitMasks::ButtonBitMasks::B)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::WiimoteButtons::One:
	{
		if (lastInputs[2] & ButtonBitMasks::ButtonBitMasks::One)
		{
			out = true;

		}
		break;
	}
	case WiimoteButtons::WiimoteButtons::Two:
	{
		if (lastInputs[2] & ButtonBitMasks::ButtonBitMasks::Two)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::WiimoteButtons::DpadUp:
	{
		if (lastInputs[1] & ButtonBitMasks::ButtonBitMasks::DpadUp)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::WiimoteButtons::DpadDown:
	{
		if (lastInputs[1] & ButtonBitMasks::ButtonBitMasks::DpadDown)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::WiimoteButtons::DpadRight:
	{
		if (lastInputs[1] & ButtonBitMasks::ButtonBitMasks::DpadRight)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::WiimoteButtons::DpadLeft:
	{
		if (lastInputs[1] & ButtonBitMasks::ButtonBitMasks::DpadLeft)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::WiimoteButtons::Plus:
	{
		if (lastInputs[1] & ButtonBitMasks::ButtonBitMasks::Plus)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::WiimoteButtons::Minus:
	{
		if (lastInputs[2] & ButtonBitMasks::ButtonBitMasks::Minus)
		{
			out = true;
		}
		break;
	}
	case WiimoteButtons::WiimoteButtons::Home:
	{
		if (lastInputs[2] & ButtonBitMasks::ButtonBitMasks::Home)
		{
			out = true;
		}
		break;
	}
	}
	return out;
}

//get x accel data
int Wiimote::getAccelX()
{
	return lastInputs[3];
}

//get y accel data
int Wiimote::getAccelY()
{
	return lastInputs[4];
}

//get z accel data
int Wiimote::getAccelZ()
{
	return lastInputs[5];
}

//get IR sensor data
IRdata Wiimote::getIRData()
{
	IRdata output;
	
	std::vector<BYTE> package1 = { lastInputs[6], lastInputs[7], lastInputs[8] };

	int16_t xValue = package1[0];
	xValue += (int16_t)(package1[2] & 0b00110000) << 4;

	int16_t yValue = package1[1];
	yValue += (int16_t)(package1[2] & 0b11000000) << 2;

	output.x = xValue;
	output.y = yValue;

	if (output.x == 1023 && output.y == 1023)
	{
		output.x = -1;
		output.y = -1;
	}

	return output;
}

bool Wiimote::IsConnected()
{
	//ask HID connection for its attributes as a way to determine if it is still connected
	HIDD_ATTRIBUTES Attributes;
	Attributes.Size = sizeof(Attributes);
	if (HidD_GetAttributes(wiimoteHandle, &Attributes) && wiimoteHandle != INVALID_HANDLE_VALUE)
	{
		return true;
	}
	return false;
}

//send packets to wiimote
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

//read from wiimote input report
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
		/*std::wcout << L"recieved " << bytes_read << L" bytes: ";
		for (BYTE byte : inputBuffer)
		{
			std::wcout << std::hex << static_cast<int>(byte) << L" ";
		}
		std::cout << "\n";*/
		lastInputs = inputBuffer;
		return true;
	}
	
}
