#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <cmath>
#include <thread>
#define ScreenWidth 100 //ʵ����ʾ���ΪScreenWidth*2��Ϊ��ƽ�ⳤ�����⣬������Ϸ��ÿһ����λ�����������ַ���ʾ��������Բ��ʾ������Բ��
#define ScreenHeight 75
#define FrameTime 25 //��λΪ����
#define GrayScale 13//�ҶȽ׽�����ֱ��Ӱ��ϸ��̶�

class ScreenManager
{
public:
    ScreenManager();
    void Screeninit();
    std::vector<char>& getRefScreenBuffer();
    std::vector<char> getScreenBuffer();
    void ShowPixel();//��������Ļ��������Ϊ����λ��������ַ���ƽ��ߺͿ�
    void SetEmptyBuffer();
    void ScreenClear();
private:
    //HANDLE ConsoleOutput;
    std::vector<char> ScreenBuffer;
};
#endif // SCREENMANAGER_H
