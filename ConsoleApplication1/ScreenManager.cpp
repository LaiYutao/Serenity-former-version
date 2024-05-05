#include "ScreenManager.h"

ScreenManager::ScreenManager()
{
	// ��ȡ����̨������
	//ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	//// ���ÿ���̨���ڴ�С
	//SMALL_RECT windowSize = { 0, 0, ScreenWidth*2 - 1, ScreenHeight - 1 };
	//SetConsoleWindowInfo(ConsoleOutput, true, &windowSize);

	//// ���ÿ���̨��������С
	//COORD bufferSize = { ScreenWidth*2, ScreenHeight };
	//SetConsoleScreenBufferSize(ConsoleOutput, bufferSize);

	/*Screeninit();*/
}

void ScreenManager::Screeninit()
{
    // ��ȡ��׼������
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    // ��ȡ����̨���ڴ�С
    HWND consoleWindow = GetConsoleWindow();
    RECT windowRect;
    GetWindowRect(consoleWindow, &windowRect);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    // ���ÿ���̨��Ļ��������С
    COORD bufferSize = { ScreenWidth * 2, ScreenHeight };
    SetConsoleScreenBufferSize(hConsoleOutput, bufferSize);

    // ���ÿ���̨���ڴ�С��λ��
    SMALL_RECT windowSize = { 0, 0, 99, 39 };
    SetConsoleWindowInfo(hConsoleOutput, TRUE, &windowSize);
    SetConsoleOutputCP(CP_UTF8);
    
    // ���ñ༭����
    DWORD consoleMode;
    GetConsoleMode(hConsoleOutput, &consoleMode);
    consoleMode &= ~ENABLE_QUICK_EDIT_MODE; // �Ƴ����ٱ༭ģʽ
    SetConsoleMode(hConsoleOutput, consoleMode);
    consoleMode &= ~ENABLE_INSERT_MODE;     // �Ƴ�����ģʽ
    SetConsoleMode(hConsoleOutput, consoleMode);
    consoleMode &= ~ENABLE_MOUSE_INPUT;     // �Ƴ��������
    SetConsoleMode(hConsoleOutput, consoleMode);

    // ���ع�����
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);
    csbi.dwSize.Y = 40;
    csbi.dwSize.X = 100;
    SetConsoleScreenBufferSize(hConsoleOutput, csbi.dwSize);
    SetConsoleMode(hConsoleOutput, consoleMode | CONSOLE_FULLSCREEN_MODE);

    // ���ô��ڴ�С��λ��
    SetWindowPos(consoleWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

    // ��ȡ��׼������
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    // ��ȡ��ǰ������Ϣ
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hConsoleOutput, FALSE, &fontInfo);

    // �����������ƺʹ�С
    wcscpy_s(fontInfo.FaceName, L"Consolas");
    fontInfo.dwFontSize.X = 3;
    fontInfo.dwFontSize.Y = 6;

    // ��������
    SetCurrentConsoleFontEx(hConsoleOutput, FALSE, &fontInfo);
}

std::vector<char>& ScreenManager::getRefScreenBuffer()
{
	return ScreenBuffer;
}

std::vector<char> ScreenManager::getScreenBuffer()
{
    return ScreenBuffer;
}

void ScreenManager::ShowPixel()
{
	int corScreenWidth = 2 * ScreenWidth; //������Ļ���
	for (int i = 0;i < ScreenBuffer.size();++i) {
		std::cout << ScreenBuffer[i];
		if ((i+1) % corScreenWidth == 0)
		{
			std::cout << std::endl;
		}
	}
	ScreenClear();
}

void ScreenManager::SetEmptyBuffer()
{
    std::vector<char>().swap(ScreenBuffer);//�ÿղ��ͷ��ڴ�
}

void ScreenManager::ScreenClear()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 };
	SetConsoleCursorPosition(hConsole, coordScreen);
	SetEmptyBuffer();
}
