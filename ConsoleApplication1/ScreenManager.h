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
    void ShowPixel();//��������Ļ��������Ϊ����λ��������ַ���ƽ��ߺͿ�
    void SetEmptyBuffer();
    void ScreenClear();
private:
    std::vector<char> ScreenBuffer;
};
#endif // SCREENMANAGER_H
