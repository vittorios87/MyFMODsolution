#include "pch.h"
#include "DisplayBuffer.h"


namespace MySmallRadioApp
{
	DisplayBuffer*  DisplayBuffer::classInstance;

	DisplayBuffer::DisplayBuffer()
	{
		bufferHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleCursorInfo(bufferHandle, &cursorInfo);
		HideCursor();
		SetCursorTo(0, 0);

	}


	void DisplayBuffer::HideCursor() {
		cursorInfo.bVisible = false; // set the cursor visibility
		SetConsoleCursorInfo(bufferHandle, &cursorInfo);
	}
	void DisplayBuffer::ShowCursor() {
		cursorInfo.bVisible = true; // set the cursor visibility
		SetConsoleCursorInfo(bufferHandle, &cursorInfo);
	}
	void DisplayBuffer::SetCursorTo(SHORT Xcoord, SHORT Ycoord) {
		coord.X = Xcoord;
		coord.Y = Ycoord;
		SetConsoleCursorPosition(bufferHandle, coord);
	}

	void DisplayBuffer::printBuffer(bool hideCursor) {
		SetCursorTo(0, 0);
		clearScreen();
		SetCursorTo(0, 0);
		WriteConsole(bufferHandle, buffer.c_str(), buffer.size(), &Written, NULL);

		if (!hideCursor)
		{
			HideCursor();
		}
		else
		{
			ShowCursor();
		}
	}

	void DisplayBuffer::writeBuffer(const std::wstring& input) {
		buffer.assign(input);
	}

	void DisplayBuffer::clearBuffer() {
		buffer.clear();
	}


	void DisplayBuffer::clearScreen()
	{

		GetConsoleScreenBufferInfo(bufferHandle, &screenBufferInfo);
		dwConsoleSize = screenBufferInfo.dwSize.X * screenBufferInfo.dwSize.Y;
		FillConsoleOutputCharacter(bufferHandle, (TCHAR) ' ', dwConsoleSize, zeroCoords, &Written);
		FillConsoleOutputAttribute(bufferHandle, screenBufferInfo.wAttributes, dwConsoleSize, zeroCoords, &Written);

	}

	DisplayBuffer::~DisplayBuffer()
	{
	}
}