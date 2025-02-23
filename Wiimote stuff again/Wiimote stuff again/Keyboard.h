#pragma once
#include <Windows.h>
#include "Enums.h"


class Keyboard
{
public:
	struct KeyInfo
	{
		KeyboardKeys index;
		WORD hexCode;
		bool isKeyDown = false;
	};

	void PressKey(KeyboardKeys keyboardKey);
	void ReleaseKey(KeyboardKeys keyboardKey);

private:
	bool iskeydown[104];

	//pairs of enum and hex codes for the keyboard keys
	KeyInfo keyInfo[102] = {
		//mouse clicks
		{LEFT_CLICK, 0x01},
		{RIGHT_CLICK, 0x02},
		//letters
		{A, 0x41},
		{B, 0x42},
		{C, 0x43},
		{D, 0x44},
		{E, 0x45},
		{F, 0x46},
		{G, 0x47},
		{H, 0x48},
		{I, 0x49},
		{J, 0x4A},
		{K, 0x4B},
		{L, 0x4C},
		{M, 0x4D},
		{N, 0x4E},
		{O, 0x4F},
		{P, 0x51},
		{Q, 0x52},
		{R, 0x53},
		{S, 0x54},
		{T, 0x55},
		{U, 0x56},
		{V, 0x57},
		{W, 0x58},
		{X, 0x59},
		{Y, 0x5A},
		{Z, 0x5B},
		//numbers
		{ZERO, 0x30},
		{ONE, 0x31},
		{TWO, 0x32},
		{THREE, 0x33},
		{FOUR, 0x34},
		{FIVE, 0x35},
		{SIX, 0x36},
		{SEVEN, 0x37},
		{EIGHT, 0x38},
		{NINE, 0x39},
		//main keyboard functionality
		{ESC, 0x1B},
		{TAB, 0x09},
		{CAPSLOCK, 0x14},
		{LSHIFT, 0xA0},
		{LCTRL, 0xA2},
		{WINDOWS, 0x5B},
		{LALT, 0xA4},
		{SPACE, 0x20},
		{RALT, 0xA5},
		{RCTRL, 0xA3},
		{RSHIFT, 0XA1},
		{ENTER, 0x0D},
		{BACKSPACE, 0x08},
		//math and punctuation
		{MINUS, 0xBD},
		{EQUALS, 0xBB},
		{OPEN_SQR_BRACKET, 0xDB},
		{CLOSE_SQR_BRACKET, 0xDD},
		{SEMICOLON, 0xBA},
		{BACKSLASH, 0xDC},
		{FORWARDSLASH, 0xBF},
		{COMMA, 0xBC},
		{FULLSTOP, 0xBE},
		{F1, 0x71},
		{F2, 0x72},
		{F3, 0x73},
		{F4, 0x74},
		{F5, 0x75},
		{F6, 0x76},
		{F7, 0x77},
		{F8, 0x78},
		{F9, 0x79},
		{F10, 0x7A},
		{F11, 0x7B},
		{F12, 0x7C},
		//arrows and command keys
		{PRINT_SCREEN, 0x2C},
		{SCROLL_LOCK, 0x91},
		{PAUSE, 0x13},
		{INSERT, 0x2D},
		{HOME, 0x24},
		{PAGE_UP, 0x21},
		{DEL, 0x2E}, 
		{END, 0x23},
		{PAGE_DOWN, 0x22},
		{UP_ARROW, 0x26},
		{LEFT_ARROW, 0x25},
		{DOWN_ARROW, 0x28},
		{RIGHT_ARROW, 0x27},
		//num pad
		{NUM_LOCK, 0x90},
		{NUMPAD_SLASH, 0x6F},
		{NUMPAD_ASTERISK, 0x6A},
		{NUMPAD_MINUS, 0x6D},
		{NUMPAD_PLUS, 0x6B},
		{NUMPAD_ENTER, 0x6C},
		{NUMPAD_FULLSTOP, 0x6E},
		{NUMPAD_0, 0x60},
		{NUMPAD_1, 0x61},
		{NUMPAD_2, 0x62},
		{NUMPAD_3, 0x63},
		{NUMPAD_4, 0x64},
		{NUMPAD_5, 0x65},
		{NUMPAD_6, 0x66},
		{NUMPAD_7, 0x67},
		{NUMPAD_8, 0x68},
		{NUMPAD_9, 0x69}
	};
};

