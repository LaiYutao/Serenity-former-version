#include "Medium.h"

Medium::Medium(const double& xPos, const double& yPos)
    : Point(xPos, yPos), CurrentFrequency(0), CurrentInitialPhase(0), CurrentAmplitude(0), Height(0) 
{
}

//���ʱ�������ܡ������Ĳ���
void Medium::GetActivated(const double& timeOfNow, const double& newAmplitude, const double& newFrequency, const double& newInitialPhase)
{
    //���㲢�����񶯷��̲�����t=timeOfNowʱ�����������Ҫ���Ǻ�����������Ӱ�죩
    if (CurrentAmplitude == 0)
    {
        //�����Ƶ��ֱ�Ӵ��뼴��
        CurrentAmplitude = newAmplitude;
        CurrentFrequency = newFrequency;

        //t=timeOfNowʱ��HeightΪ0������񶯷������ϻ������£���ʱ��λ����Source������λ�����Ƴ���t=0Ϊ���ĳ���λ
        CurrentInitialPhase = newInitialPhase - 2*PI*CurrentFrequency * timeOfNow; // t=timeOfNowʱ��cos�ڲ���Ӧ����newInitialPhase; ��Ƶ�ʦ�=2��f��frequency��

    }
    else//���ӵ����
    {
        //������ĺ����壺�𵽲��ĵ�������
    }
}

//�񶯣����Ǹı�Height��ֵ��������Monitor����ת��Ϊ�ַ���ʾ
void Medium::Vibrate(const double& timeOfNow)
{
    if (CurrentAmplitude == 0)
    {
        return;//���CurrentAmplitudeΪ�㣬��Ϊ����
    }
    else
    {
        Height = CurrentAmplitude * cos(2 * PI * CurrentFrequency * timeOfNow + CurrentInitialPhase);
    }
}

double Medium::getCurrentFrequency() const 
{
    return CurrentFrequency;
}

double Medium::getCurrentInitialPhase() const 
{
    return CurrentInitialPhase;
}

double Medium::getCurrentAmplitude() const 
{
    return CurrentAmplitude;
}