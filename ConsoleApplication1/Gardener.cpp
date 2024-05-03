#include "Gardener.h"
#include"windows.h"

Gardener::Gardener():PlantingPoint(Point(0,0))
{
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		SourceChecking[i] = false;
	}

	for (int i = 0;i < 4;++i)//只使用四个按键
	{
		IfMovingKeyPressed[i] = false;
		IfPlantingKeyPressed[i] = false;
	}
}

Point Gardener::getPlantingPoint() const
{
	return PlantingPoint;
}

void Gardener::SelectPosition()
{

}


