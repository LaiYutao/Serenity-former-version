#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H
#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <cmath>
#include <thread>
#define ScreenWidth 120 //ʵ����ʾ���ΪScreenWidth*2��Ϊ��ƽ�ⳤ�����⣬������Ϸ��ÿһ����λ�����������ַ���ʾ��������Բ��ʾ������Բ��
#define ScreenHeight 75
#define FrameTime 100 //��λΪ����
#define GrayScale 13//�ҶȽ׽�����ֱ��Ӱ��ϸ��̶�

class ScreenManager
{
public:
    ScreenManager();
    void ChangeBufferForm();
    void ShowActImage();
    void ShowPageImage(); // ÿ����ʾ�����ScreenBuffer
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
