#pragma once
#include <Windows.h>
#include <hidsdi.h>
#include <setupapi.h>
#include <Cfgmgr32.h>

#include <iostream>
#include <vector>

enum WiimoteButtons
{
	A,
	B,
	One,
	Two,
	DpadUp,
	DpadDown,
	DpadLeft,
	DpadRight,
	Plus,
	Minus,
	Home

};


class Wiimote
{

public:
	bool CreateHIDConnection();
	void GetCaps();

	void SetLEDs();
	void SetReportMode();


	bool IsButtonDown(WiimoteButtons button);

private:
	HANDLE wiimoteHandle;
	USHORT outputReportSize;

	bool Write(std::vector<BYTE> outputBuffer);

public:
	bool Read(std::vector<BYTE> inputBuffer);

private:
	std::vector<BYTE> lastInputs;
};

