#ifndef MEDIUM_H
#define MEDIUM_H
#include"Point.h"
#include<chrono>
class Medium : public Point {//到时将整个地图的每个像素都创建为Medium，用数组迭代；
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