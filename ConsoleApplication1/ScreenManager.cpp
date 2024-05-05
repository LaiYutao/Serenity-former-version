#include "ScreenManager.h"

ScreenManager::ScreenManager()
{
	// 获取控制台输出句柄
	//ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	//// 设置控制台窗口大小
	//SMALL_RECT windowSize = { 0, 0, ScreenWidth*2 - 1, ScreenHeight - 1 };
	//SetConsoleWindowInfo(ConsoleOutput, true, &windowSize);

	//// 设置控制台缓冲区大小
	//COORD bufferSize = { ScreenWidth*2, ScreenHeight };
	//SetConsoleScreenBufferSize(ConsoleOutput, bufferSize);

	/*Screeninit();*/
}

void ScreenManager::Screeninit()
{
    // 获取标准输出句柄
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    // 获取控制台窗口大小
    HWND consoleWindow = GetConsoleWindow();
    RECT windowRect;
    GetWindowRect(consoleWindow, &windowRect);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    // 设置控制台屏幕缓冲区大小
    COORD bufferSize = { ScreenWidth * 2, ScreenHeight };
    SetConsoleScreenBufferSize(hConsoleOutput, bufferSize);

    // 设置控制台窗口大小和位置
    SMALL_RECT windowSize = { 0, 0, 99, 39 };
    SetConsoleWindowInfo(hConsoleOutput, TRUE, &windowSize);
    SetConsoleOutputCP(CP_UTF8);
    
    // 禁用编辑功能
    DWORD consoleMode;
    GetConsoleMode(hConsoleOutput, &consoleMode);
    consoleMode &= ~ENABLE_QUICK_EDIT_MODE; // 移除快速编辑模式
    SetConsoleMode(hConsoleOutput, consoleMode);
    consoleMode &= ~ENABLE_INSERT_MODE;     // 移除插入模式
    SetConsoleMode(hConsoleOutput, consoleMode);
    consoleMode &= ~ENABLE_MOUSE_INPUT;     // 移除鼠标输入
    SetConsoleMode(hConsoleOutput, consoleMode);

    // 隐藏滚动条
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);
    csbi.dwSize.Y = 40;
    csbi.dwSize.X = 100;
    SetConsoleScreenBufferSize(hConsoleOutput, csbi.dwSize);
    SetConsoleMode(hConsoleOutput, consoleMode | CONSOLE_FULLSCREEN_MODE);

    // 重置窗口大小和位置
    SetWindowPos(consoleWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

    // 获取标准输出句柄
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    // 获取当前字体信息
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hConsoleOutput, FALSE, &fontInfo);

    // 设置字体名称和大小
    wcscpy_s(fontInfo.FaceName, L"Consolas");
    fontInfo.dwFontSize.X = 3;
    fontInfo.dwFontSize.Y = 6;

    // 设置字体
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
	int corScreenWidth = 2 * ScreenWidth; //修正屏幕宽度
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
    std::vector<char>().swap(ScreenBuffer);//置空并释放内存
}

void ScreenManager::ScreenClear()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 };
	SetConsoleCursorPosition(hConsole, coordScreen);
	SetEmptyBuffer();
}
