#ifndef FIELD_H
#define FIELD_H
#include"Point.h"
#include"ScreenManager.h"
#include"Medium.h"
#include<chrono>
class Field {
public:
	Field(Point sourcePosition ,double s_Amplitude, double s_Frequency, double s_InitialPhase,int s_Speed);
	virtual void ActivateMedium(float ElapsedTime, Medium[ScreenWidth * ScreenHeight]) = 0;//向四面八方发送“激活射线”
	virtual ~Field() {};
	Point getSourcePosition()const;
	double getSourceAmplitude() const;
	double getSourceFrequency() const;
	double getSourceInitialPhase() const;
	int getSourceSpeed() const;

	void setSourcePosition(const Point& sourcePosition);
	void setSourceAmplitude(const double& s_Amplitude);
	void setSourceFrequency(const double &s_Frequency);
	void setSourceInitialPhase(const double& s_InitialPhase);
	void setSourceSpeed(const int& s_Speed);
private:
	Point SourcePosition;
	double SourceInitialPhase;
	double SourceFrequency;
	double SourceAmplitude;
	int SourceSpeed;
};
#endif // FIELD_H
