#ifndef DISCJOCKEY_H
#define DISCJOCKEY_H //由Medium的Height控制音符的映射。作为管理。
#include<vector>
#include"ScreenManager.h"
class DiscJockey
{
public:
	DiscJockey();
	void CalculateHeightDistribution(const std::vector<double>& compoundHeight);
	void CalculateHertz();
	double getCalculatedHertz()const;

private:
	int HeightDistribution[GrayScale];
	double CalculatedHertz;//计算结果储存于内部，使得计算和发声分离
};
#endif // DISCJOCKEY_H
