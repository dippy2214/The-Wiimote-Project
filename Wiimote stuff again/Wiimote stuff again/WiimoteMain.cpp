
#include <Windows.h>
#include <bluetoothapis.h>
#include <hidsdi.h>
#include <setupapi.h>
#include <Cfgmgr32.h>
#include <initguid.h>
#include <devpkey.h>

#include<vector>
#include <iostream>

//#include "Helpers.h"

//DEFINE_DEVPROPKEY(DEVPKEY_Device_DriverProvider, 0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 9);

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

bool CheckForToshibaStack(const DEVINST& hid_interface_device_instance)
{
	HDEVINFO parent_device_info = nullptr;
	SP_DEVINFO_DATA parent_device_data = {};
	parent_device_data.cbSize = sizeof(SP_DEVINFO_DATA);

	if (GetParentDevice(hid_interface_device_instance, &parent_device_info, &parent_device_data))
	{
		std::wstring class_driver_provider = GetDeviceProperty(parent_device_info, &parent_device_data, &DEVPKEY_Device_DriverProvider);

		SetupDiDestroyDeviceInfoList(parent_device_info);

		return (class_driver_provider == L"TOSHIBA");
	}

	std::cout << "Unable to detect class driver provider!\n";

	return false;
}

int main()
{

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

	BLUETOOTH_FIND_RADIO_PARAMS radioParams;
	radioParams.dwSize = sizeof(radioParams);

	HANDLE hRadio;
	

	HBLUETOOTH_RADIO_FIND hFindRadio = BluetoothFindFirstRadio(&radioParams, &hRadio);
	//int count = 0;
	while (hFindRadio)
	{
		BLUETOOTH_RADIO_INFO radioInfo;
		radioInfo.dwSize = sizeof(radioInfo);

		auto const rInfo_result = BluetoothGetRadioInfo(hRadio, &radioInfo);

		if (rInfo_result == ERROR_SUCCESS)
		{
			search.hRadio = hRadio;

			BLUETOOTH_DEVICE_INFO btdi;
			btdi.dwSize = sizeof(btdi);

			HBLUETOOTH_DEVICE_FIND hFindDevice = BluetoothFindFirstDevice(&search, &btdi);

			while (hFindDevice)
			{
				std::cout << "Device, Connected: " << btdi.fConnected << " Authenticated: " << btdi.fAuthenticated << " Remembered: " << btdi.fRemembered << "\n";

				if (IsValidDeviceName(WStringToString(btdi.szName)))
				{
					std::cout << "valid device\n";
					ForgetWiimote(btdi);
					AttachWiimote(hRadio, radioInfo, btdi);
				}
				if (BluetoothFindNextDevice(hFindDevice, &btdi) == false)
				{
					BluetoothFindDeviceClose(hFindDevice);
					hFindDevice = nullptr;
				}
			}
		}
		if (!BluetoothFindNextRadio(hFindRadio, &hRadio))
		{
			CloseHandle(hRadio);
			BluetoothFindRadioClose(hFindRadio);
			hFindRadio = nullptr;
		}
	}

	GUID device_id;
	HidD_GetHidGuid(&device_id);

	
	HDEVINFO const device_info = SetupDiGetClassDevsW(&device_id, nullptr, nullptr, (DIGCF_DEVICEINTERFACE | DIGCF_PRESENT));

	SP_DEVICE_INTERFACE_DATA device_data = {};
	device_data.cbSize = sizeof(device_data);

	for (int index = 0; SetupDiEnumDeviceInterfaces(device_info, nullptr, &device_id, index, &device_data); index++)
	{
		DWORD len;
		SetupDiGetDeviceInterfaceDetail(device_info, &device_data, nullptr, 0, &len, nullptr);
		auto detail_data_buf = std::make_unique<std::uint8_t[]>(len);
		auto detail_data = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(detail_data_buf.get());
		detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		SP_DEVINFO_DATA device_info_data = {};
		device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);

		if (SetupDiGetDeviceInterfaceDetail(device_info, &device_data, detail_data, len, nullptr, &device_info_data))
		{
			std::basic_string<TCHAR> device_path(detail_data->DevicePath);
			bool IsUsingToshibaStack = CheckForToshibaStack(device_info_data.DevInst);

			std::cout << "toshiba stack: " << IsUsingToshibaStack << "\n";
		}
	}

	return 0;
}
