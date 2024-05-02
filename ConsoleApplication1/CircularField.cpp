#include"CircularField.h"
#include"ScreenManager.h"
#include"Medium.h"
#include"RayTip.h"

CircularField::CircularField(const Point& s_Position, const double& s_Amplitude, const double& s_Frequency, const double& s_InitialPhase, const int& s_Speed)
	:Field(s_Position, s_Amplitude, s_Frequency, s_InitialPhase, s_Speed)
{
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		MediumLayer[i] = Medium();//显式初始化为0
	}
}

void CircularField::ActivateMedium(const double& timeOfNow, const double& frameTime)
{
	static const int NumberOfRay = (ScreenWidth + ScreenHeight) * 2 - 4;//屏幕最外圈点的个数，作为发出光线的数量
	
	static bool IfActivated[ScreenWidth*ScreenHeight];
	static bool IfActivated_initialized = false;//避免多次初始化数组
	if (!IfActivated_initialized) 
	{
		for (int i = 0;i < ScreenWidth * ScreenHeight;++i) 
		{
			IfActivated[i] = false;
		}
		IfActivated_initialized = true;
	}
	
	//所有的射线尖端构成一个数组,并初始化
	static RayTip BunchOfRayTips[NumberOfRay];
	static bool BunchOfRayTips_initialized = false;
	if (!BunchOfRayTips_initialized) 
	{
		for (int i = 0;i < NumberOfRay;++i) 
		{
			double DirectionAngle = i * (2 * PI / NumberOfRay);
			BunchOfRayTips[i] = RayTip(getSourcePosition().getXPos(), getSourcePosition().getYPos(), DirectionAngle,0.0f);
		}
	}

	//遍历每一条射线
	for (int i = 0;i < NumberOfRay;++i) 
	{
		BunchOfRayTips[i].setDistanceToSource(BunchOfRayTips[i].getDistanceToSource() + this->getSourceSpeed() * frameTime);
		double newXPos = getSourcePosition().getXPos() + BunchOfRayTips[i].getDistanceToSource() * cos(BunchOfRayTips[i].getDirectionAngle());
		BunchOfRayTips[i].setXPos(newXPos);
		double newYPos = getSourcePosition().getYPos() + BunchOfRayTips[i].getDistanceToSource() * sin(BunchOfRayTips[i].getDirectionAngle());
		BunchOfRayTips[i].setYPos(newYPos);
		
		//判断Medium是否被激活过(将RayTip的坐标取整，就是所击中的MediumPixel的坐标)，如果没有，就将其激活
		int MediumPixelIndex = ScreenWidth * (int)BunchOfRayTips[i].getYPos() + (int)BunchOfRayTips[i].getXPos();
		if (!IfActivated[MediumPixelIndex])
		{
			MediumLayer[MediumPixelIndex].GetActivated(timeOfNow,this->getSourceAmplitude(),this->getSourceFrequency(),this->getSourceInitialPhase());
			IfActivated[MediumPixelIndex] = true;
		}
	}
}