#ifndef MONITOR_H
#define MONITOR_H //���ڹ���ÿ��Medium����ʾ�ַ�������һ���ַ����д��棬�����ٵ�buffer��
#include"Medium.h"                 //Ŀǰ��ScreenManager�Ĺ�ϵ������ȷ
#include"ScreenManager.h"
#include<vector>

class Monitor 
{
public:
    Monitor();
    void ChangeIntoPixel(std::vector<char> buffer,double mediumHeight);//����ÿ��Medium��Height,Ȼ��ӳ�䵽��Ӧ�ַ���
    void UpdateHeightLayer(std::vector<Medium[ScreenWidth * ScreenHeight]> compoundField);
private:
    double HeightLayer[ScreenWidth * ScreenHeight];
};
#endif // MONITOR_H