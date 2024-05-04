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
#define FrameTime 16 //��λΪ����
#define GrayScale 13//�ҶȽ׽�����ֱ��Ӱ��ϸ��̶�

class ScreenManager
{
public:
    std::vector<char>& getRefScreenBuffer();
    void ShowPixel();//��������Ļ��������Ϊ����λ��������ַ���ƽ��ߺͿ�
    void SetEmptyBuffer();
    void ScreenClear();
private:
    std::vector<char> ScreenBuffer;
};
#endif // SCREENMANAGER_H
