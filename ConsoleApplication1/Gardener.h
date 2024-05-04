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
	void PlantIt(const double& timeOfNow);//��ѡ��λ�ã������¡�һ����Դ
	void AdjustAmplitude();//�����ѡ��λ���Ѿ����ڳ�Դ����ô�ı����Ĳ���; ��������������PlantIt����
	void AdjustFrequency();
private:
	int SourceChecking[ScreenWidth * ScreenHeight]; //��¼�õ㴦�ĳ���ӦCompoundField�е���ţ���1��ʼ)����0����Ϊ�õ�û������һ������
	std::vector <Field*> CompoundField;//�����г����Ӷ��ɵĸ��ϳ�
	std::vector<Medium[ScreenWidth * ScreenHeight]> CompoundMedium;//���������г���Ӧ���ʲ���Ӷ��ɵĸ��Ͻ���
	Point PlantingPoint;
	bool IfKeyPressed[6];//��������WSADJK��WSAD������������ֲ���ƶ���JK����û����ֲ�ĵ������£����ֿ�ѡ��ĳ���JKSW������ֲ���ĵ��ϵ��ڲ�����
};

#endif // GARDENER_H