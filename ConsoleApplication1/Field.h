#ifndef FIELD_H
#define FIELD_H
#include"Point.h"
#include"ScreenManager.h"
#include"Medium.h"
#include"RayTip.h"
#include<chrono>
#include<cmath>

class Field 
{
public:
	Field(const double& creationTime, const Point& s_Position, const double& s_Amplitude, const double& s_Frequency, const double& s_InitialPhase, const double& s_Speed);
	virtual ~Field() {}; //���ڸտ�ʼ�񶯵ĵ㣨������Դ�����״α�������Medium�����Ǵ�0��ʼ�𶯵ģ����ֻ��õ�����/����ʱ�䣬���˳���λ���ɣ�
	                     //Field�ĳ���λ������ָ���ǲ�Դ���ĵ㣬��t=0ʱ���£��񶯷��̵ĳ���λ��ֻ��Ϊ��/2����һ�������񶯣�������-��/2����һ�������񶯣���
	
	virtual void ActivateMedium(const double& timeOfNow, const double& frameTime) = 0;//������˷����͡��������ߡ�
	void ClearIfActivated();

	double getCreationTime()const;
	std::vector<Medium>* getPtrMediumLayer();
	Point getSourcePosition()const;
	double getSourceAmplitude() const;
	double getSourceFrequency() const;
	double getSourceInitialPhase() const;
	double getSourceSpeed() const;
	void setSourcePosition(const Point& sourcePosition);
	void setSourceAmplitude(const double& s_Amplitude);
	void setSourceFrequency(const double &s_Frequency);
	void setSourceInitialPhase(const double& s_InitialPhase);
	void setSourceSpeed(const int& s_Speed);

protected:
	bool StopActivation;
	static const int NumberOfRay = ((ScreenWidth + ScreenHeight) * 2 - 4)*16;//����Ȧ��ĸ����ı�������Ϊ�������ߵ�������4��ʱ�Ѿ����¿��У�8��ʱ�м�����ֻʣ������12��ʱ���������ֻʣ�߽���һЩë��,16��ʱ�߽��Դ���һЩë�̣�
	bool IfActivated[ScreenWidth * ScreenHeight];
	RayTip BunchOfRayTips[NumberOfRay];
	std::vector<Medium> MediumLayer;
private:
	double CreationTime;
	Point SourcePosition;
	double SourceInitialPhase;
	double SourceFrequency;
	double SourceAmplitude;
	double SourceSpeed;
};
#endif // FIELD_H
