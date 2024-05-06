#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <cmath>
#include <thread>
#define ScreenWidth 100 //实际显示宽度为ScreenWidth*2，为了平衡长宽问题，所以游戏里每一个单位，都用两个字符表示。（避免圆显示得像椭圆）
#define ScreenHeight 75
#define FrameTime 25 //单位为毫秒
#define GrayScale 13//灰度阶阶数，直接影响细腻程度

class ScreenManager
{
public:
    ScreenManager();
    void Screeninit();
    std::vector<char>& getRefScreenBuffer();
    std::vector<char> getScreenBuffer();
    void ShowPixel();//修正的屏幕参数：因为横向单位输出两个字符，平衡高和宽
    void SetEmptyBuffer();
    void ScreenClear();
private:
    //HANDLE ConsoleOutput;
    std::vector<char> ScreenBuffer;
};
#endif // SCREENMANAGER_H
