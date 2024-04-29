#ifndef MEDIUM_H
#define MEDIUM_H
#include"Point.h"
#include<chrono>
class Medium : public Point {//��ʱ��������ͼ��ÿ�����ض�����ΪMedium�������������
public:
	Medium(double xPos, double yPos);
	virtual ~Medium() {};
	void GetActivated(const double &Amplitude, const double &Frequency, const double &InitialPhase);
	double getCurrentFrequency() const;
	double getCurrentPhase() const;
	double getCurrentAmplitude() const;
private:
	double CurrentFrequency;
	double CurrentPhase;
	double CurrentAmplitude;
	double Height;
};
#endif // MEDIUM_H