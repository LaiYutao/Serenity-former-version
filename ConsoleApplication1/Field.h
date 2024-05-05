#ifndef FIELD_H
#define FIELD_H
#include"Point.h"
#include"ScreenManager.h"
#include"Medium.h"
#include"RayTip.h"
#include<chrono>
#include<cmath>


class Field 
{
public:
	Field(const double& creationTime, const Point& s_Position, const double& s_Amplitude, const double& s_Frequency, const double& s_InitialPhase, const double& s_Speed);
	virtual ~Field() {}; //对于刚开始振动的点（包括波源，和首次被激发的Medium），初相位常数φ似乎顶多是用于判断是下一时刻先向下还是先向上的，并作为cos和sin之间的转化（±π/2），因为必然是从0开始震动的
	                     //Field的初相位参量，指的是波源处振动方程的初相位，只能为π/2（下一刻向下振动），或者-π/2（下一刻向上振动）；
	
	virtual void ActivateMedium(const double& timeOfNow, const double& frameTime) = 0;//向四面八方发送“激活射线”
	void ClearIfActivated();

	double getCreationTime()const;
	std::vector<Medium>& getRefMediumLayer();
	Point getSourcePosition()const;
	double getSourceAmplitude() const;
	double getSourceFrequency() const;
	double getSourceInitialPhase() const;
	double getSourceSpeed() const;
	void setSourcePosition(const Point& sourcePosition);
	void setSourceAmplitude(const double& s_Amplitude);
	void setSourceFrequency(const double &s_Frequency);
	void setSourceInitialPhase(const double& s_InitialPhase);
	void setSourceSpeed(const int& s_Speed);

protected:
	static const int NumberOfRay = (ScreenWidth + ScreenHeight) * 2 - 4;//屏幕最外圈点的个数，作为发出光线的数量
	bool IfActivated[ScreenWidth * ScreenHeight];
	RayTip BunchOfRayTips[NumberOfRay];
	std::vector<Medium> MediumLayer;
private:
	double CreationTime;
	Point SourcePosition;
	double SourceInitialPhase;
	double SourceFrequency;
	double SourceAmplitude;
	double SourceSpeed;
};
#endif // FIELD_H
