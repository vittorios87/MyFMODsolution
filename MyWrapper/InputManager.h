#pragma once

namespace MySmallRadioApp
{

	class InputManager
	{


	public:
		~InputManager();
		static InputManager* GetInstance()
		{
			if (classInstance == nullptr)
			{
				classInstance = new InputManager();
			}

			return classInstance;
		};

		bool ButtonPressed();
		unsigned char GetButtonPressed();
		void Input();

	private:
		unsigned char mButtonPressed;

		static InputManager* classInstance;
		InputManager();

	};
}


