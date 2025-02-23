#include "Keyboard.h"

//press a key on the virtual keyboard
void Keyboard::PressKey(KeyboardKeys keyboardKey)
{
		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = keyInfo[keyboardKey].hexCode;
		input.ki.wScan = 0;
		input.ki.dwFlags = 0; //0 for key down
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;

		SendInput(1, &input, sizeof(INPUT));
		keyInfo[keyboardKey].isKeyDown = true;
}

//release key on virtual keyboard
void Keyboard::ReleaseKey(KeyboardKeys keyboardKey)
{
	if (keyInfo[keyboardKey].isKeyDown)
	{
		INPUT input;
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = keyInfo[keyboardKey].hexCode;
		input.ki.wScan = 0;
		input.ki.dwFlags = KEYEVENTF_KEYUP; //0x02 for key up
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;

		SendInput(1, &input, sizeof(INPUT));
		keyInfo[keyboardKey].isKeyDown = false;
	}
}
	
