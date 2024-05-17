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
		}
		else if(ShownHeight + Offset < 0)
		{
			buffer.push_back(' ');
		}
		else
		{
			buffer.push_back(" .,:;*o%&#H$@"[ShownHeight + Offset]); //����ʵ��ó����ĻҶȽ����У�����׼ȷ��
                                                                     //ͬʱʮ�����ҶȽף��׳�Height==0��Ӧ��������պ��ܶ�Ӧʮ���������ܷ���������ֵ�ӳ��
		}
	}
}

void Monitor::AddPlantingPoint(std::vector<char>& buffer, const Point& plantingPoint)
{
	//��λ����ȡ�������ĵ���Ϊ����λ�ã�PlantingPoint��SelectPosition���������Ѿ�����Ϊ����Խ����ˣ�
	double PickRadius = 0.5;
	double X = plantingPoint.getXPos();
	double Y = plantingPoint.getYPos();
	int LessX = int(X);//����ȡX����Ϊ�жϻ�׼��X����
	int LessY = int(Y);//����ȡY����Ϊ�жϻ�׼��Y����
	int PlantX = int(X);
	int PlantY = int(Y);//������ʼ��
	for (int x = LessX;x < LessX + 2;++x)
	{
		for (int y = LessY;y < LessY + 2;++y)
		{
			//λ�ڲ�׽�뾶��
			if ((X - x) * (X - x) + (Y - y) * (Y - y) < PickRadius * PickRadius)
			{
				PlantX = x;
				PlantY = y;
				break;
			}
		}
	}
	
	//���ĸ���+��Χ��PlantingPoint
	buffer[(PlantY-1) * ScreenWidth + PlantX] = '+';
	buffer[(PlantY+1) * ScreenWidth + PlantX] = '+';
	buffer[PlantY * ScreenWidth + PlantX] = 'X';
	buffer[PlantY  * ScreenWidth + PlantX-1] = '+';
	buffer[PlantY  * ScreenWidth + PlantX+1] = '+';
}

void Monitor::AddStatusBar(std::vector<char> buffer,char*& screenShow,const Point& plantingPoint, const int& numOfCircularField, const int& numOfSpiralField)
{
	//�������һ�����
	for (int i = (ScreenHeight - 1) * ScreenWidth;i < ScreenHeight * ScreenWidth;++i)
	{
		buffer[i] = ' ';
	}
	
	for (int i = 0;i <  ScreenWidth * ScreenHeight;++i)
	{
		screenShow[i] = buffer[i];
	}

	sprintf_s(&screenShow[(ScreenHeight - 1) * ScreenWidth], ScreenWidth, "PlantPoint: X=%3.2f, Y=%3.2f;  NumOfFields: CircularField=%2.i, SpiralField=%2.i, Total=%2.i  ", plantingPoint.getXPos(), plantingPoint.getYPos(), numOfCircularField, numOfSpiralField, numOfCircularField + numOfSpiralField);
}
