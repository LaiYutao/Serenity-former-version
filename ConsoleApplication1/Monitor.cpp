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
			//�Ѹ�Medium���Height��������
			CompoundHeight[i] += mediumLayer[i].getHeight();
		}
	}
}

void Monitor::ChangeIntoPixel(std::vector<char>& buffer, double compoundHeight[ScreenWidth * ScreenHeight])
{
	int GrayScale = 11;//�ҶȽ׽�����ֱ��Ӱ��ϸ��̶�
	int Offset = (GrayScale - 1) / 2; //�ҶȽ��ַ������������ƫ����
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		if((int)compoundHeight[i] + Offset>GrayScale-1)
		{
			buffer.push_back('@');
			buffer.push_back('@'); // push back������ͬԪ�أ��Ե���Ԫ�ظ߶ȿ�ȵĲ�ͬ���µ�Բ��ʾ����Բ������
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
	//�á�<>������ʾPlantingPoint��λ��
	buffer[2 * (plantingPoint.getYPos() * ScreenHeight + plantingPoint.getXPos())] = '<';
	buffer[2 * (plantingPoint.getYPos() * ScreenHeight + plantingPoint.getXPos()) + 1] = '>';
}
