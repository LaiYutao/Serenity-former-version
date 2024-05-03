#include "ScreenManager.h"

void ScreenManger::ShowPixel()
{
	int corScreenWidth = 2 * ScreenWidth; //ĞŞÕıÆÁÄ»¿í¶È
	for (int i = 0;i < ScreenBuffer.size();++i) {
		std::cout << ScreenBuffer[i];
		if ((i+1) % corScreenWidth == 0)
		{
			std::cout << std::endl;
		}
	}
}

void ScreenManger::SetEmptyBuffer()
{
	ScreenBuffer.clear();
}

void ScreenManger::ScreenClear()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 };
	SetConsoleCursorPosition(hConsole, coordScreen);
}
