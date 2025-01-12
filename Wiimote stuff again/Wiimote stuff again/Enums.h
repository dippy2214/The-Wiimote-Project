#pragma once

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

enum ButtonBitMasks
{
	//byte one
	DpadLeft = 0x01,
	DpadRight = 0x02,
	DpadDown = 0x04,
	DpadUp = 0x08,
	Plus = 0x10,

	//byte 2
	A = 0x08,
	B = 0x04,
	One = 0x02,
	Two = 0x01,
	Minus = 0x10,
	Home = 0x80
};