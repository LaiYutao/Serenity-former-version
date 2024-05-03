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
	void PlantIt();//在选定位置，“种下”一个场源
	void CultivateIt();//如果在选定位置已经存在场源，那么改变它的参数; 这个函数放在PlantIt里面

private:
	bool SourceChecking[ScreenWidth * ScreenHeight];
	std::vector <Field*> CompoundField;//由所有场叠加而成的复合场
	std::vector<Medium[ScreenWidth * ScreenHeight]> CompoundMedium;//创建由所有场对应介质层叠加而成的复合介质
	Point PlantingPoint;
	bool IfMovingKeyPressed[4];//四个键，上下左右，用来控制种植点移动
	bool IfPlantingKeyPressed[4];//四个键，WSAD，用来控制种下的场的品种，以及调节参数
};

#endif // GARDENER_H