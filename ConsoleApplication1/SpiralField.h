#ifndef SPIRALFIELD_H
#define SPIRALFIELD_H
#include"Field.h"
#include"Medium.h"
class SpiralField : public Field{
public:
	SpiralField(Point Source, double s_Amplitude, double s_Frequency, double s_InitialPhase, int s_Speed);
	virtual ~SpiralField() {};
	virtual void ActivateMedium(float ElapsedTime, Medium GlobalMedium[ScreenWidth * ScreenHeight]);
};
#endif //SPIRALFIELD_H

