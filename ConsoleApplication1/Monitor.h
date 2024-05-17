#ifndef MONITOR_H
#define MONITOR_H //�����ۺϴ���ÿ��MediumLayer��ÿһ��Medium��Height
#include"Medium.h"                 
#include"ScreenManager.h"
#include<vector>

class Monitor 
{
public:
    Monitor();
    std::vector<double> getCompoundHeight()const;
    void SetCompoundHeight(const std::vector<double>& compoundHeight);
    void ChangeIntoPixel(std::vector<char>& buffer);//����ÿ��Medium��Height,Ȼ��ӳ�䵽��Ӧ�ַ���
    void AddPlantingPoint(std::vector<char>& buffer,const Point& plantingPoint);
    void AddStatusBar(std::vector<char> buffer,char*& screenShow,const Point& plantingPoint, const int& numOfCircularField, const int& numOfSpiralField);

private:
    std::vector<double> CompoundHeight;
};
#endif // MONITOR_H