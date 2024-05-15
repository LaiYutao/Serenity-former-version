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
	//��vector<char>���͵�ScreenBufferһһ��ֵ��char*���͵�ScreenShow
	ChangeBufferForm();

	//ʹ��Windows API�⺯���������
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacterA(hConsole,ScreenShow, 2 * ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
	
	//�ÿ�ScreenBuffer
	SetEmptyBuffer();
}

void ScreenManager::ShowStaticImage() //����ÿ�ζ��ÿ�ScreenBuffer
{
	//��vector<char>���͵�ScreenBufferһһ��ֵ��char*���͵�ScreenShow
	ChangeBufferForm();

	//ʹ��Windows API�⺯���������
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
    std::vector<char>().swap(ScreenBuffer);//�ÿղ��ͷ��ڴ�
}