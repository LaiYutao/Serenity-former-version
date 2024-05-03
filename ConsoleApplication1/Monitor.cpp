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
	int GrayScale = 9;//�ҶȽ׽�����ֱ��Ӱ��ϸ��̶�
	int Offset = (GrayScale - 1) / 2; //�ҶȽ��ַ������������ƫ����
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		buffer.push_back(" .:-+o*#%$@"[(int)heightLayer[i] + Offset]);
		buffer.push_back(" .:-+o*#%$@"[(int)heightLayer[i] + Offset]); // push back������ͬԪ�أ��Ե���Ԫ�ظ߶ȿ�ȵĲ�ͬ���µ�Բ��ʾ����Բ������
	}
}