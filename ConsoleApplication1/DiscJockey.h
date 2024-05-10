#ifndef DISCJOCKEY_H
#define DISCJOCKEY_H //��Medium��Height����������ӳ�䡣��Ϊ����
#include<vector>
#include"ScreenManager.h"

class DiscJockey
{
public:
	DiscJockey();
	void CalculateHeightDistribution(const std::vector<double>& compoundHeight);
	void CalculateHertz();
	double getCalculatedHertz()const;
	void DetectMusicTypeChange();

private:
	int HeightDistribution[GrayScale];
	bool MusicTypeFlag;// true��Ӧ΢�ְ�����ģʽ��false��Ӧʮ��������ģʽ
	double CalculatedHertz;//�������������ڲ���ʹ�ü���ͷ�������
};
#endif // DISCJOCKEY_H
