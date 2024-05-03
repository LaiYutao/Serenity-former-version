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
    void ShowPixel();//修正的屏幕参数：因为横向单位输出两个字符，平衡高和宽
    void SetEmptyBuffer();
    void ScreenClear();
private:
    std::vector<char> ScreenBuffer;
};
#endif // SCREENMANAGER_H
