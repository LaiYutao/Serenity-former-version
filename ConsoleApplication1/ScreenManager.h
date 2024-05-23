#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H
#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <cmath>
#include <thread>
//���º곣�����������Ϸ������ģ���������ҳ������ҳ��
#define ScreenWidth 240 //��Ϸ������Ŀ�
#define ScreenHeight 150 //��Ϸ������ĸ�
#define FontWidth 6 //��Ϸ��������ֿ�
#define FontHeight 6 //��Ϸ��������ָ�
#define FrameTime 100 //��λΪ����
#define GrayScale 13//�ҶȽ׽�����ֱ��Ӱ��ϸ��̶�

class ScreenManager
{
public:
    //Ĭ�ϲ���Ϊ��Ϸ������Ĳ���
    ScreenManager(int screenWidth= ScreenWidth,int screenHeight= ScreenHeight,int fontWidth= FontWidth,int fontHeight= FontHeight);
    void ChangeBufferForm();
    void ShowActImage();
    void ShowPageImage(); // ÿ����ʾ�����ScreenBuffer

    void setScreenShow(char* screen);
    void SetEmptyBuffer();
    std::vector<char>& getRefScreenBuffer();
    std::vector<char> getScreenBuffer();
    char*& getRefScreenShow();
    int getScreenWidth()const;
    int getScreenHeight()const;

private:
    HANDLE hConsole;
    SMALL_RECT rectWindow;
    char* ScreenShow;
    std::vector<char> ScreenBuffer;
    //������Ϸ������֮�⻭��Ŀ���̨��ߡ�������������˽�г�Ա����m��ʾ���֣�
    int m_ScreenWidth;
    int m_ScreenHeight;
    int m_FontWidth;
    int m_FontHeight;
};
#endif // SCREENMANAGER_H
