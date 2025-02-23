#pragma once
#include <Windows.h>

namespace WiimoteButtons
{
	//enum for all wii remote buttons
	enum class WiimoteButtons
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
}

namespace ButtonBitMasks
{
	//bitmasks for wii report buttons
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
}

//104 keys based off the keyboard i happen to have right now
enum KeyboardKeys
{
	LEFT_CLICK,
	RIGHT_CLICK,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	ZERO,
	ESC,
	TAB,
	CAPSLOCK,
	LSHIFT,
	LCTRL,
	WINDOWS,
	LALT,
	SPACE,
	RALT,
	RCTRL,
	RSHIFT,
	ENTER,
	BACKSPACE,
	MINUS,
	EQUALS,
	OPEN_SQR_BRACKET,
	CLOSE_SQR_BRACKET,
	SEMICOLON,
	BACKSLASH,
	FORWARDSLASH,
	COMMA,
	FULLSTOP,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	PRINT_SCREEN,
	SCROLL_LOCK,
	PAUSE,
	INSERT,
	HOME,
	PAGE_UP,
	DEL,
	END,
	PAGE_DOWN,
	UP_ARROW,
	LEFT_ARROW,
	DOWN_ARROW,
	RIGHT_ARROW,
	NUM_LOCK,
	NUMPAD_SLASH,
	NUMPAD_ASTERISK,
	NUMPAD_MINUS,
	NUMPAD_PLUS,
	NUMPAD_ENTER,
	NUMPAD_FULLSTOP,
	NUMPAD_0,
	NUMPAD_1,
	NUMPAD_2,
	NUMPAD_3,
	NUMPAD_4,
	NUMPAD_5,
	NUMPAD_6,
	NUMPAD_7,
	NUMPAD_8,
	NUMPAD_9
};

