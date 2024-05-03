#ifndef MEDIUM_H
#define MEDIUM_H
#include"Point.h"
#include<chrono>
#include<cmath>

class Medium  
{//到时将整个地图的每个像素都创建为Medium，用数组迭代；
public:
	Medium(const double& sourceCreationTime=0);
	virtual ~Medium() {};
	void GetActivated(const double& timeOfNow, const double& newAmplitude, const double& newFrequency, const double& newInitialPhase);
	void Vibrate(const double& timeOfNow);
	double getCurrentAmplitude() const;
	double getCurrentFrequency() const;
	double getCurrentInitialPhase() const;
	double getHeight()const;

private:
	double SourceCreationTime;
	double CurrentAmplitude;
	double CurrentFrequency;
	double CurrentInitialPhase;
	double Height;
};
#endif // MEDIUM_H