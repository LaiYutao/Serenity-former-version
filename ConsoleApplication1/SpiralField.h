#ifndef SPIRALFIELD_H
#define SPIRALFIELD_H
#include"Field.h"
#include"Medium.h"

class SpiralField : public Field
{
public:
	SpiralField(const Point& s_Position, const double& s_Amplitude, const double& s_Frequency, const double& s_InitialPhase, const int& s_Speed);
	virtual ~SpiralField() {};
	
	virtual void ActivateMedium(const double& timeOfNow, const double& frameTime, Medium[ScreenWidth * ScreenHeight]);
};
#endif //SPIRALFIELD_H

