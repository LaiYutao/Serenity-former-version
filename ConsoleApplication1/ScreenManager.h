#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H
#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <cmath>
#include <thread>
#define ScreenWidth 120 //实际显示宽度为ScreenWidth*2，为了平衡长宽问题，所以游戏里每一个单位，都用两个字符表示。（避免圆显示得像椭圆）
#define ScreenHeight 75
#define FrameTime 100 //单位为毫秒
#define GrayScale 13//灰度阶阶数，直接影响细腻程度

class ScreenManager
{
public:
    ScreenManager();
    void ChangeBufferForm();
    void ShowActImage();
    void ShowPageImage(); // 每次显示后不清空ScreenBuffer
    std::vector<char>& getRefScreenBuffer();
    std::vector<char> getScreenBuffer();
    char*& getRefScreenShow();
    //HANDLE& get_hConsole();
    void setScreenShow(char* screen);
    void SetEmptyBuffer();

private:
    HANDLE hConsole;
    char* ScreenShow;
    std::vector<char> ScreenBuffer;
};
#endif // SCREENMANAGER_H
