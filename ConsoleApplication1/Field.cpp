#include "Field.h"

Field::Field(const double& creationTime, const Point& s_Position,const double& s_Amplitude, const double& s_Frequency, const double& s_InitialPhase, const int& s_Speed)
    : CreationTime(creationTime),SourcePosition(s_Position), SourceAmplitude(s_Amplitude), SourceFrequency(s_Frequency), SourceInitialPhase(s_InitialPhase), SourceSpeed(s_Speed)
{
    for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
    {
        MediumLayer[i] = Medium(this->getCreationTime());//显式初始化为0
    }
}

double Field::getCreationTime()const
{
    return CreationTime;
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