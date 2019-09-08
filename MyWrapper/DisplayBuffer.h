#pragma once

namespace MySmallRadioApp
{

	class DisplayBuffer
	{

	public:
		~DisplayBuffer();
		static DisplayBuffer* GetInstance()
		{
			if (classInstance == nullptr)
			{
				classInstance = new DisplayBuffer();
			}

			return classInstance;
		};

		void HideCursor();
		void ShowCursor();
		void SetCursorTo(SHORT, SHORT);
		void printBuffer(bool);
		void writeBuffer(const std::wstring&);
		void clearBuffer();
	private:
		std::wstring buffer;
		HANDLE bufferHandle;
		CONSOLE_CURSOR_INFO cursorInfo;
		CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
		DWORD dwConsoleSize;
		COORD zeroCoords = { 0, 0 };

		void clearScreen();

		COORD coord;
		DWORD Written;

		static DisplayBuffer* classInstance;
		DisplayBuffer();



	};

}