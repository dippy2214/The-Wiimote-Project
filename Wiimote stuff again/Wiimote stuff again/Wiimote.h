#pragma once
#include <Windows.h>
#include <hidsdi.h>
#include <setupapi.h>
#include <Cfgmgr32.h>

#include <iostream>
#include <vector>

#include "Enums.h"

struct IRdata
{
	int x = 0;
	int y = 0;
};

class Wiimote
{
	
public:
	

	bool CreateHIDConnection();
	void GetCaps();

	void SetLEDs();
	void SetReportMode();
	void EnableIRSensor();


	bool IsButtonDown(WiimoteButtons::WiimoteButtons button);
	int getAccelX();
	int getAccelY();
	int getAccelZ();
	IRdata getIRData();

private:
	HANDLE wiimoteHandle;
	USHORT outputReportSize;

	bool Write(std::vector<BYTE> outputBuffer);

public:
	bool Read(std::vector<BYTE> inputBuffer);

private:
	std::vector<BYTE> lastInputs;

	bool inputs[11];
};

