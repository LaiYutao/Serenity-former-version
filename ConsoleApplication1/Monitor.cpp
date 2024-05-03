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
			CompoundHeight[i] += mediumLayer[i].getHeight();
		}
	}
}

void Monitor::ChangeIntoPixel(std::vector<char> buffer, double heightLayer[ScreenWidth * ScreenHeight])
{
	int GrayScale = 9;//灰度阶阶数，直接影响细腻程度
	int Offset = (GrayScale - 1) / 2; //灰度阶字符串索引所需的偏移量
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		buffer.push_back(" .:-+o*#%$@"[(int)heightLayer[i] + Offset]);
		buffer.push_back(" .:-+o*#%$@"[(int)heightLayer[i] + Offset]); // push back两个相同元素，以抵消元素高度宽度的不同导致的圆显示成椭圆的问题
	}
}