#ifndef MONITOR_H
#define MONITOR_H //���ڹ���ÿ��Medium����ʾ�ַ�������һ���ַ����д��棬�����ٵ�buffer��
#include"Medium.h"                 
#include"ScreenManager.h"
#include<vector>

class Monitor 
{
public:
    Monitor();

    void UpdateCompoundHeight(const std::vector<std::vector<Medium>>& compoundMedium);
    void ChangeIntoPixel(std::vector<char>& buffer,std::vector<double> compoundHeight);//����ÿ��Medium��Height,Ȼ��ӳ�䵽��Ӧ�ַ���
    void AddPlantingPoint(std::vector<char>& buffer,const Point& plantingPoint);
private:
    //double CompoundHeight[ScreenWidth * ScreenHeight];
    std::vector<double> CompoundHeight;
};
#endif // MONITOR_H