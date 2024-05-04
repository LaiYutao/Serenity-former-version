#include"Monitor.h"

Monitor::Monitor()
{
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		CompoundHeight.push_back(0);
	}
}

std::vector<double> Monitor::getCompoundHeight() const
{
	return CompoundHeight;
}

void Monitor::UpdateCompoundHeight(const std::vector<std::vector<Medium>>& compoundMedium)
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

void Monitor::ChangeIntoPixel(std::vector<char>& buffer, std::vector<double> compoundHeight)
{
	int Offset = (GrayScale - 1) / 2; //灰度阶字符串索引所需的偏移量
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		//粗略的四舍五入，避免对于一些波，振幅最高的情形取不到
		int ShownHeight = (compoundHeight[i] >= 0) ? int(compoundHeight[i] + 0.5) :int(compoundHeight[i] - 0.5);
		if(ShownHeight + Offset>GrayScale-1)
		{
			buffer.push_back('@');
			buffer.push_back('@'); // push back两个相同元素，以抵消元素高度宽度的不同导致的圆显示成椭圆的问题
		}
		else if(ShownHeight + Offset < 0)
		{
			buffer.push_back(' ');
			buffer.push_back(' ');
		}
		else
		{
			buffer.push_back(" .,:-+o*#%&$@"[ShownHeight + Offset]);
			buffer.push_back(" .,:-+o*#%&$@"[ShownHeight + Offset]);
		}
	}
}

void Monitor::AddPlantingPoint(std::vector<char>& buffer, const Point& plantingPoint)
{
	//用“<>”来显示PlantingPoint的位置
	buffer[2 * (int(plantingPoint.getYPos()) * ScreenHeight + int(plantingPoint.getXPos()))] = '<';
	buffer[2 * (int(plantingPoint.getYPos()) * ScreenHeight + int(plantingPoint.getXPos())) + 1] = '>';
}
