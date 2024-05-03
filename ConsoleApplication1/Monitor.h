#ifndef MONITOR_H
#define MONITOR_H //���ڹ���ÿ��Medium����ʾ�ַ�������һ���ַ����д��棬�����ٵ�buffer��
#include"Medium.h"                 //Ŀǰ��ScreenManager�Ĺ�ϵ������ȷ
#include"ScreenManager.h"
#include<vector>

class Monitor 
{
public:
    Monitor();
    void UpdateCompoundHeight(std::vector<Medium[ScreenWidth * ScreenHeight]> compoundMedium);
    void ChangeIntoPixel(std::vector<char>& buffer,double compoundHeight[ScreenWidth * ScreenHeight]);//����ÿ��Medium��Height,Ȼ��ӳ�䵽��Ӧ�ַ���
    void AddPlantingPoint(std::vector<char>& buffer,const Point& plantingPoint);
private:
    double CompoundHeight[ScreenWidth * ScreenHeight];
};
#endif // MONITOR_H