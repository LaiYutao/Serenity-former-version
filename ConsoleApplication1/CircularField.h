#ifndef CIRCULARFIELD_H
#define CIRCULARFIELD_H
#include"Field.h"

class CircularField : public Field 
{
public:
	CircularField(const Point& s_Position, const double& s_Amplitude, const double& s_Frequency, const double& s_InitialPhase, const int& s_Speed);
	virtual ~CircularField() {};
	
	virtual void ActivateMedium(const double& timeOfNow, const double& frameTime);
	
};
#endif //CIRCULARFIELD_H
