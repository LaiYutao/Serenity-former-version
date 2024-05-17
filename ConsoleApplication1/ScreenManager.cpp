#include "ScreenManager.h"

ScreenManager::ScreenManager(int screenWidth, int screenHeight, int fontWidth, int fontHeight):m_ScreenWidth(screenWidth),m_ScreenHeight(screenHeight),m_FontWidth(fontWidth), m_FontHeight(fontHeight)
{
	ScreenShow = new char[m_ScreenWidth * m_ScreenHeight];
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	CONSOLE_SCREEN_BUFFER_INFOEX csbi;
	rectWindow = { 0, 0, 1, 1 };
	csbi.srWindow = rectWindow;
	csbi.dwSize = { 1,1 };
	//SetConsoleScreenBufferInfoEx(hConsole, &csbi);
	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);
	COORD BufferSize = { (short)m_ScreenWidth, (short)m_ScreenHeight };
	SetConsoleScreenBufferSize(hConsole, BufferSize);
	SetConsoleActiveScreenBuffer(hConsole);
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = m_FontWidth;
	cfi.dwFontSize.Y = m_FontHeight;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(hConsole, false, &cfi);
	rectWindow = { 0, 0, short(m_ScreenWidth - 1), short(m_ScreenHeight - 1) };
	csbi.srWindow = rectWindow;
	csbi.dwSize = BufferSize;
	csbi.dwMaximumWindowSize = { (short)m_ScreenWidth,(short)m_ScreenHeight };
	//SetConsoleScreenBufferInfoEx(hConsole, &csbi);
	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);

}

void ScreenManager::ChangeBufferForm()
{
	for (int i = 0;i < m_ScreenWidth * m_ScreenHeight;++i)
	{
		ScreenShow[i] = ScreenBuffer[i];
	}
}

void ScreenManager::ShowActImage()
{
	//使用Windows API库函数进行输出
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacterA(hConsole,ScreenShow, m_ScreenWidth * m_ScreenHeight, { 0,0 }, &dwBytesWritten);
	
	//置空ScreenBuffer
	SetEmptyBuffer();
}

void ScreenManager::ShowPageImage() //不再每次都置空ScreenBuffer
{
	//将vector<char>类型的ScreenBuffer一一赋值给char*类型的ScreenShow
	ChangeBufferForm();

	//使用Windows API库函数进行输出
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacterA(hConsole, ScreenShow, m_ScreenWidth * m_ScreenHeight, { 0,0 }, &dwBytesWritten);
}

std::vector<char>& ScreenManager::getRefScreenBuffer()
{
	return ScreenBuffer;
}

std::vector<char> ScreenManager::getScreenBuffer()
{
    return ScreenBuffer;
}

char*& ScreenManager::getRefScreenShow()
{
	return ScreenShow;
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

int ScreenManager::getScreenWidth() const
{
	return m_ScreenWidth;
}

int ScreenManager::getScreenHeight() const
{
	return m_ScreenHeight;
}
