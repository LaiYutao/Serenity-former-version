#ifndef GARDENER_H
#define GARDENER_H
#include"ScreenManager.h"
#include"CircularField.h"
#include"SpiralField.h"

class Gardener
{
public:
	Gardener();
	Point getPlantingPoint()const;
	void SelectPosition();//操控光标（或者一个记号）到所希望的位置
	void PlantIt(const double& timeOfNow);//在选定位置，“种下”一个场源
	void AdjustAmplitude();//如果在选定位置已经存在场源，那么改变它的参数; 这两个函数放在PlantIt里面
	void AdjustFrequency();
private:
	int SourceChecking[ScreenWidth * ScreenHeight]; //记录该点处的场对应CompoundField中的序号（从1开始)；‘0’即为该点没有种下一个场；
	std::vector <Field*> CompoundField;//由所有场叠加而成的复合场
	std::vector<Medium[ScreenWidth * ScreenHeight]> CompoundMedium;//创建由所有场对应介质层叠加而成的复合介质
	Point PlantingPoint;
	bool IfKeyPressed[6];//共六个键WSADJK：WSAD，用来控制种植点移动；JK，在没有种植的点上种下，两种可选择的场；JKSW，在种植过的点上调节参数；
};

#endif // GARDENER_H