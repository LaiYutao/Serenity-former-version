#include"Monitor.h"

Monitor::Monitor()
{
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		CompoundHeight[i] = 0;
	}
}

void Monitor::UpdateCompoundHeight(std::vector<Medium[ScreenWidth * ScreenHeight]> compoundMedium)
{
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		for (auto mediumLayer : compoundMedium)
		{
			//把各Medium层的Height叠加起来
			CompoundHeight[i] += mediumLayer[i].getHeight();
		}
	}
}

void Monitor::ChangeIntoPixel(std::vector<char>& buffer, double compoundHeight[ScreenWidth * ScreenHeight])
{
	int GrayScale = 11;//灰度阶阶数，直接影响细腻程度
	int Offset = (GrayScale - 1) / 2; //灰度阶字符串索引所需的偏移量
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		if((int)compoundHeight[i] + Offset>GrayScale-1)
		{
			buffer.push_back('@');
			buffer.push_back('@'); // push back两个相同元素，以抵消元素高度宽度的不同导致的圆显示成椭圆的问题
		}
		else if((int)compoundHeight[i] + Offset < 0)
		{
			buffer.push_back(' ');
			buffer.push_back(' ');
		}
		else
		{
			buffer.push_back(" .,:-+o*#%&$@"[(int)compoundHeight[i] + Offset]);
			buffer.push_back(" .,:-+o*#%&$@"[(int)compoundHeight[i] + Offset]);
		}
	}
}

void Monitor::AddPlantingPoint(std::vector<char>& buffer, const Point& plantingPoint)
{
	//用“<>”来显示PlantingPoint的位置
	buffer[2 * (plantingPoint.getYPos() * ScreenHeight + plantingPoint.getXPos())] = '<';
	buffer[2 * (plantingPoint.getYPos() * ScreenHeight + plantingPoint.getXPos()) + 1] = '>';
}
