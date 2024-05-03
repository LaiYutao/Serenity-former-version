#ifndef GARDENER_H
#define GARDENER_H
#include"ScreenManager.h"
#include"CircularField.h"
#include"SpiralField.h"

class Gardener
{
public:
	Gardener();
	Point getPlantingPoint()const;
	void SelectPosition();//�ٿع�꣨����һ���Ǻţ�����ϣ����λ��
	void PlantIt();//��ѡ��λ�ã������¡�һ����Դ
	void CultivateIt();//�����ѡ��λ���Ѿ����ڳ�Դ����ô�ı����Ĳ���; �����������PlantIt����

private:
	bool SourceChecking[ScreenWidth * ScreenHeight];
	std::vector <Field*> CompoundField;//�����г����Ӷ��ɵĸ��ϳ�
	std::vector<Medium[ScreenWidth * ScreenHeight]> CompoundMedium;//���������г���Ӧ���ʲ���Ӷ��ɵĸ��Ͻ���
	Point PlantingPoint;
	bool IfMovingKeyPressed[4];//�ĸ������������ң�����������ֲ���ƶ�
	bool IfPlantingKeyPressed[4];//�ĸ�����WSAD�������������µĳ���Ʒ�֣��Լ����ڲ���
};

#endif // GARDENER_H