#ifndef CIRCULARFIELD_H
#define CIRCULARFIELD_H
#include"Field.h"
class CircularField : public Field {
public:
	CircularField(Point Source, double s_Amplitude, double s_Frequency, double s_InitialPhase, int s_Speed);
	virtual ~CircularField() {};
	virtual void ActivateMedium(float ElapsedTime,Medium[ScreenWidth * ScreenHeight]);
	
};
#endif //CIRCULARFIELD_H
