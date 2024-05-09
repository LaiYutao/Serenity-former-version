#include"Monitor.h"

Monitor::Monitor()
{
	CompoundHeight.resize(ScreenWidth * ScreenHeight, 0);
}

std::vector<double> Monitor::getCompoundHeight() const
{
	return CompoundHeight;
}

void Monitor::UpdateCompoundHeight(const std::vector<std::vector<Medium>*>& compoundMedium)
{
	//先进行重置
	CompoundHeight.clear();
	CompoundHeight.resize(ScreenWidth * ScreenHeight, 0);

	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		for (std::vector<Medium>* mediumLayerPtr : compoundMedium)
		{
			//把各Medium层的Height叠加起来
			CompoundHeight[i] += (*mediumLayerPtr)[i].getHeight();
		}
	}
}

void Monitor::ChangeIntoPixel(std::vector<char>& buffer)
{
	int Offset = (GrayScale - 1) / 2; //灰度阶字符串索引所需的偏移量
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		//粗略的四舍五入，避免对于一些波，振幅最高的情形取不到
		int ShownHeight = (CompoundHeight[i] >= 0) ? int(CompoundHeight[i] + 0.5) :int(CompoundHeight[i] - 0.5);
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
			buffer.push_back(" .,:;*o%&#H$@"[ShownHeight + Offset]); //经过实验得出来的灰度阶序列，基本准确；
			buffer.push_back(" .,:;*o%&#H$@"[ShownHeight + Offset]); //同时十三个灰度阶，抛出Height==0对应的情况，刚好能对应十二个音，很方便进行音乐的映射
		}
	}
}

void Monitor::AddPlantingPoint(std::vector<char>& buffer, const Point& plantingPoint)
{
	//定位器：取离得最近的点作为考虑位置（PlantingPoint在SelectPosition函数那里已经设置为不会越界的了）
	double PickRadius = 0.5;
	double X = plantingPoint.getXPos();
	double Y = plantingPoint.getYPos();
	int LessX = int(X);//向下取X，作为判断基准点X坐标
	int LessY = int(Y);//向下取Y，作为判断基准点Y坐标
	int PlantX = int(X);
	int PlantY = int(Y);//先随便初始化
	for (int x = LessX;x < LessX + 2;++x)
	{
		for (int y = LessY;y < LessY + 2;++y)
		{
			//位于捕捉半径内
			if ((X - x) * (X - x) + (Y - y) * (Y - y) < PickRadius * PickRadius)
			{
				PlantX = x;
				PlantY = y;
				break;
			}
		}
	}
	
	//用“<>”来显示PlantingPoint的位置
	buffer[2 * (PlantY * ScreenWidth + PlantX)] = '<';
	buffer[2 * (PlantY * ScreenWidth + PlantX) + 1] = '>';
}
