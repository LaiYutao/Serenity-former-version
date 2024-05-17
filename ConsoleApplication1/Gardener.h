#ifndef GARDENER_H
#define GARDENER_H
#include"ScreenManager.h"
#include"CircularField.h"
#include"SpiralField.h"

class Gardener
{
public:
	Gardener();
	void UpdateCompoundHeight();
	void SelectPosition();//�ٿع�꣨����һ���Ǻţ�����ϣ����λ��
	void PlantIt(const double& timeOfNow);//��ѡ��λ�ã������¡�һ����Դ
	void AdjustAmplitude(int plantX,int plantY);//�����ѡ��λ���Ѿ����ڳ�Դ����ô���Ըı����Ĳ���; ��������������PlantIt����
	void AdjustFrequency(int plantX, int plantY);
	
	Point getPlantingPoint()const;
	int getNumOfCircularField()const;
	int getNumOfSpiralField()const;
	std::vector<double> getCompoundHeight()const;
	std::vector<Field*>& getRefCompoundField();

private:
	int SourceChecking[ScreenWidth * ScreenHeight]; //��¼�õ㴦�ĳ���ӦCompoundField�е���ţ���1��ʼ)����0����Ϊ�õ�û������һ������
	std::vector <Field*> CompoundField;//�����г����Ӷ��ɵĸ��ϳ�
	std::vector<std::vector<Medium>*> CompoundMedium;//���������г���Ӧ���ʲ���Ӷ��ɵĸ��Ͻ���
	std::vector<double> CompoundHeight;//��¼ÿ�����ʶ�Ӧ���ۺϸ߶�
	Point PlantingPoint;
	int NumOfCircularField;
	int NumOfSpiralField;
};

#endif // GARDENER_H