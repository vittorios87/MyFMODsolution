#include "pch.h"
#include "InputManager.h"

namespace MySmallRadioApp
{
	InputManager*  InputManager::classInstance;

	InputManager::InputManager()
	{
	}


	InputManager::~InputManager()
	{
	}

	bool InputManager::ButtonPressed()
	{
		return mButtonPressed;
	}
	unsigned char InputManager::GetButtonPressed()
	{
		return mButtonPressed;
	}
	void InputManager::Input()
	{
		if (_kbhit()) {
			mButtonPressed = _getch();
		}
		else
			mButtonPressed = 0;
	}
}

