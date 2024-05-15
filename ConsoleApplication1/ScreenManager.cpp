#include "ScreenManager.h"

ScreenManager::ScreenManager()
{
	ScreenShow = new char[2 * ScreenWidth * ScreenHeight];
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
}

void ScreenManager::ChangeBufferForm()
{
	for (int i = 0;i < 2 * ScreenWidth * ScreenHeight;++i)
	{
		ScreenShow[i] = ScreenBuffer[i];
	}
}

void ScreenManager::ShowImage()
{
	//将vector<char>类型的ScreenBuffer一一赋值给char*类型的ScreenShow
	ChangeBufferForm();

	//使用Windows API库函数进行输出
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacterA(hConsole,ScreenShow, 2 * ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
	
	//置空ScreenBuffer
	SetEmptyBuffer();
}

void ScreenManager::ShowStaticImage() //不再每次都置空ScreenBuffer
{
	//将vector<char>类型的ScreenBuffer一一赋值给char*类型的ScreenShow
	ChangeBufferForm();

	//使用Windows API库函数进行输出
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacterA(hConsole, ScreenShow, 2 * ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
}

std::vector<char>& ScreenManager::getRefScreenBuffer()
{
	return ScreenBuffer;
}

std::vector<char> ScreenManager::getScreenBuffer()
{
    return ScreenBuffer;
}

//char* ScreenManager::getScreenShow() const
//{
//	return ScreenShow;
//}

//HANDLE& ScreenManager::get_hConsole()
//{
//	return hConsole;
//}

void ScreenManager::setScreenShow(char* screen)
{
	ScreenShow = screen;
}

void ScreenManager::SetEmptyBuffer()
{
    std::vector<char>().swap(ScreenBuffer);//置空并释放内存
}