
#include <Windows.h>
#include <bluetoothapis.h>
#include <hidsdi.h>
#include <setupapi.h>
#include <Cfgmgr32.h>
#include <initguid.h>
#include <devpkey.h>

#include<vector>
#include <iostream>


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
	if (!btdi.fConnected && !btdi.fRemembered)
	{
		auto const& wm_addr = btdi.Address.rgBytes;
		std::cout << "Found Wiimote (" << wm_addr[0] << ":" << wm_addr[1] << ":" << wm_addr[2] << ":" << wm_addr[3] << ":" << wm_addr[4] << ":" << wm_addr[5] << "\n";

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

int main()
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

	//initialise GUID of device
	GUID device_id;
	HidD_GetHidGuid(&device_id);

	//this gets a handle to the device information set for all the HID devices, basically a list of them
	HDEVINFO const device_info = SetupDiGetClassDevs(&device_id, nullptr, nullptr, (DIGCF_DEVICEINTERFACE | DIGCF_PRESENT));

	//initialise device interface information structure
	SP_DEVICE_INTERFACE_DATA device_data = {};
	device_data.cbSize = sizeof(device_data);

	bool isWiimote = false;
	HANDLE wiimote_handle = INVALID_HANDLE_VALUE;

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
			std::cout << device_path.c_str()<< "\n";

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
				wiimote_handle = CreateFile(device_path.c_str(),			//specify device path
					GENERIC_READ | GENERIC_WRITE,		//read write access
					FILE_SHARE_READ | FILE_SHARE_WRITE, //read write shared access
					nullptr,							//no security lmao
					OPEN_EXISTING,						//open existing file is one already exists
					FILE_FLAG_OVERLAPPED,				//async I/O
					nullptr								//no template file
				);
			}

			//check for invalid handle
			if (wiimote_handle == INVALID_HANDLE_VALUE)
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
	if (wiimote_handle == INVALID_HANDLE_VALUE)
	{
		//close handle without matching device found and end the program
		CloseHandle(wiimote_handle);
		std::cout << "no matching device found\n";
		return 0;
	}

	std::cout << "wiimote found\n";

	//////////////////////////
	//
	// TODO - write the code to handle talking to the wii remote here
	//
	//////////////////////////


	CloseHandle(wiimote_handle);
	return 0;
}
