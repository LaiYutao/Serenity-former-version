#ifndef MONITOR_H
#define MONITOR_H //用于管理每个Medium的显示字符，传到一个字符串中储存，后续再到buffer？
#include"Medium.h"                 
#include"ScreenManager.h"
#include<vector>

class Monitor 
{
public:
    Monitor();

    void UpdateCompoundHeight(const std::vector<std::vector<Medium>>& compoundMedium);
    void ChangeIntoPixel(std::vector<char>& buffer,std::vector<double> compoundHeight);//接受每个Medium的Height,然后映射到对应字符。
    void AddPlantingPoint(std::vector<char>& buffer,const Point& plantingPoint);
private:
    //double CompoundHeight[ScreenWidth * ScreenHeight];
    std::vector<double> CompoundHeight;
};
#endif // MONITOR_H