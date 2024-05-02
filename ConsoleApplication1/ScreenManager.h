#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <cmath>
#define ScreenWidth 600
#define ScreenHeight 150

class ScreenManger
{
public:
private:
    HANDLE m_ConsoleOutput;
    std::vector<char> m_ScreenBuffer;
    std::vector<char> m_BackBuffer;
};
#endif // SCREENMANAGER_H
