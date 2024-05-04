#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <cmath>
#include<thread>
#define ScreenWidth 600
#define ScreenHeight 150
#define FrameTime 16 //单位为毫秒
#define GrayScale 13//灰度阶阶数，直接影响细腻程度

class ScreenManager
{
public:
    std::vector<char>& getRefScreenBuffer();
    void ShowPixel();//修正的屏幕参数：因为横向单位输出两个字符，平衡高和宽
    void SetEmptyBuffer();
    void ScreenClear();
private:
    std::vector<char> ScreenBuffer;
};
#endif // SCREENMANAGER_H
