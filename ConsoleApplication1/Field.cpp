#include "Field.h"

Field::Field(const Point& s_Position,const double& s_Amplitude, const double& s_Frequency, const double& s_InitialPhase, const int& s_Speed)
    : SourcePosition(s_Position), SourceAmplitude(s_Amplitude), SourceFrequency(s_Frequency), SourceInitialPhase(s_InitialPhase), SourceSpeed(s_Speed) 
{ 
}

Point Field::getSourcePosition() const 
{
    return SourcePosition;
}

double Field::getSourceAmplitude() const 
{
    return SourceAmplitude;
}

double Field::getSourceFrequency() const 
{
    return SourceFrequency;
}

double Field::getSourceInitialPhase() const 
{
    return SourceInitialPhase;
}

int Field::getSourceSpeed() const 
{
    return SourceSpeed;
}

void Field::setSourcePosition(const Point& sourcePosition) 
{
    SourcePosition = sourcePosition;
}

void Field::setSourceAmplitude(const double& s_Amplitude) 
{
    SourceAmplitude = s_Amplitude;
}

void Field::setSourceFrequency(const double& s_Frequency) 
{
    SourceFrequency = s_Frequency;
}

void Field::setSourceInitialPhase(const double& s_InitialPhase) 
{
    SourceInitialPhase = s_InitialPhase;
}

void Field::setSourceSpeed(const int& s_Speed) 
{
    SourceSpeed = s_Speed;
}