#include "ScreenManager.h"

ScreenManager::ScreenManager(int screenWidth, int screenHeight, int fontWidth, int fontHeight):m_ScreenWidth(screenWidth),m_ScreenHeight(screenHeight),m_FontWidth(fontWidth), m_FontHeight(fontHeight)
{
	ScreenShow = new char[m_ScreenWidth * m_ScreenHeight];
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
	//��������С������֮�����
	rectWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);
	
	//����Buffer��С
	COORD BufferSize = { (short)m_ScreenWidth, (short)m_ScreenHeight };
	SetConsoleScreenBufferSize(hConsole, BufferSize);
	SetConsoleActiveScreenBuffer(hConsole);
	
	//������������
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = m_FontWidth;
	cfi.dwFontSize.Y = m_FontHeight;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(hConsole, false, &cfi);
	
	//���ÿ���̨���ڵ�������
	rectWindow = { 0, 0, short(m_ScreenWidth - 1), short(m_ScreenHeight - 1) };
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
	//ʹ��Windows API�⺯���������
	DWORD dwBytesWritten = 0;
	WriteConsoleOutputCharacterA(hConsole,ScreenShow, m_ScreenWidth * m_ScreenHeight, { 0,0 }, &dwBytesWritten);
	
	//�ÿ�ScreenBuffer
	SetEmptyBuffer();
}

void ScreenManager::ShowPageImage() //����ÿ�ζ��ÿ�ScreenBuffer
{
	//��vector<char>���͵�ScreenBufferһһ��ֵ��char*���͵�ScreenShow
	ChangeBufferForm();

	//ʹ��Windows API�⺯���������
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

void ScreenManager::setScreenShow(char* screen)
{
	ScreenShow = screen;
}

void ScreenManager::SetEmptyBuffer()
{
    std::vector<char>().swap(ScreenBuffer);//�ÿղ��ͷ��ڴ�
}

int ScreenManager::getScreenWidth() const
{
	return m_ScreenWidth;
}

int ScreenManager::getScreenHeight() const
{
	return m_ScreenHeight;
}
