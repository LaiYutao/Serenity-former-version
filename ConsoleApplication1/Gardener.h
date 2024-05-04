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
	std::vector<Field*>& getRefCompoundField();
	std::vector<std::vector<Medium>> getCompoundMedium()const;
	void SelectPosition();//�ٿع�꣨����һ���Ǻţ�����ϣ����λ��
	void PlantIt(const double& timeOfNow);//��ѡ��λ�ã������¡�һ����Դ
	void AdjustAmplitude();//�����ѡ��λ���Ѿ����ڳ�Դ����ô�ı����Ĳ���; ��������������PlantIt����
	void AdjustFrequency();
private:
	int SourceChecking[ScreenWidth * ScreenHeight]; //��¼�õ㴦�ĳ���ӦCompoundField�е���ţ���1��ʼ)����0����Ϊ�õ�û������һ������
	std::vector <Field*> CompoundField;//�����г����Ӷ��ɵĸ��ϳ�
	std::vector<std::vector<Medium>> CompoundMedium;//���������г���Ӧ���ʲ���Ӷ��ɵĸ��Ͻ���
	Point PlantingPoint;
};

#endif // GARDENER_H