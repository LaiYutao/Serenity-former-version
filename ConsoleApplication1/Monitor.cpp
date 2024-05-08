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
	//�Ƚ�������
	CompoundHeight.clear();
	CompoundHeight.resize(ScreenWidth * ScreenHeight, 0);

	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		for (std::vector<Medium>* mediumLayerPtr : compoundMedium)
		{
			//�Ѹ�Medium���Height��������
			CompoundHeight[i] += (*mediumLayerPtr)[i].getHeight();
		}
	}
}

void Monitor::ChangeIntoPixel(std::vector<char>& buffer)
{
	int Offset = (GrayScale - 1) / 2; //�ҶȽ��ַ������������ƫ����
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		//���Ե��������룬�������һЩ���������ߵ�����ȡ����
		int ShownHeight = (CompoundHeight[i] >= 0) ? int(CompoundHeight[i] + 0.5) :int(CompoundHeight[i] - 0.5);
		if(ShownHeight + Offset>GrayScale-1)
		{
			buffer.push_back('@');
			buffer.push_back('@'); // push back������ͬԪ�أ��Ե���Ԫ�ظ߶ȿ�ȵĲ�ͬ���µ�Բ��ʾ����Բ������
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
	//�á�<>������ʾPlantingPoint��λ��
	buffer[2 * (int(plantingPoint.getYPos()) * ScreenWidth + int(plantingPoint.getXPos()))] = '<';
	buffer[2 * (int(plantingPoint.getYPos()) * ScreenWidth + int(plantingPoint.getXPos())) + 1] = '>';
}
