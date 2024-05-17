#include "ScreenManager.h"

ScreenManager::ScreenManager()
{
	ScreenShow = new char[ScreenWidth * ScreenHeight];
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	CONSOLE_SCREEN_BUFFER_INFOEX csbi;
	rectWindow = { 0, 0, 1, 1 };
	csbi.srWindow = rectWindow;
	csbi.dwSize = { 1,1 };
	//SetConsoleScreenBufferInfoEx(hConsole, &csbi);
	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);
	COORD BufferSize = { (short)ScreenWidth, (short)ScreenHeight };
	SetConsoleScreenBufferSize(hConsole, BufferSize);
	SetConsoleActiveScreenBuffer(hConsole);
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = FontWidth;
	cfi.dwFontSize.Y = FontHeight;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(hConsole, false, &cfi);
	rectWindow = { 0, 0, (short)ScreenWidth - 1, (short)ScreenHeight - 1 };
	csbi.srWindow = rectWindow;
	csbi.dwSize = BufferSize;
	csbi.dwMaximumWindowSize = { ScreenWidth,ScreenHeight };
	//SetConsoleScreenBufferInfoEx(hConsole, &csbi);
	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);

}

void ScreenManager::ChangeBufferForm()
{
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		ScreenShow[i] = ScreenBuffer[i];
	}
}

void ScreenManager::ShowActImage()
{
	//ʹ��Windows API�⺯���������
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacterA(hConsole,ScreenShow, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
	
	//�ÿ�ScreenBuffer
	SetEmptyBuffer();
}

void ScreenManager::ShowPageImage() //����ÿ�ζ��ÿ�ScreenBuffer
{
	//��vector<char>���͵�ScreenBufferһһ��ֵ��char*���͵�ScreenShow
	ChangeBufferForm();

	//ʹ��Windows API�⺯���������
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacterA(hConsole, ScreenShow, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
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
    std::vector<char>().swap(ScreenBuffer);//�ÿղ��ͷ��ڴ�
}