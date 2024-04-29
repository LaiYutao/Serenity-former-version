#include<iostream>
#include<chrono>
#include<cmath>
#include"CircularField.h"
#include"ScreenManager.h"
#include"Medium.h"
#include"RayTip.h"
void CircularField::ActivateMedium(float ElapsedTime,Medium GlobalMedium[ScreenWidth * ScreenHeight]) {
	static const int NumberOfRay = (ScreenWidth + ScreenHeight) * 2 - 4;//屏幕最外圈点的个数，作为发出光线的数量
	
	static bool IfActivated[ScreenWidth*ScreenHeight];
	static bool IfActivated_initialized = false;//避免多次初始化数组
	if (!IfActivated_initialized) {
		for (int i = 0;i < ScreenWidth * ScreenHeight;++i) {
			IfActivated[i] = false;
		}
		IfActivated_initialized = true;
	}
	
	//所有的射线尖端构成一个数组,并初始化
	static RayTip BunchOfRayTips[NumberOfRay];
	static bool BunchOfRayTips_initialized = false;
	if (!BunchOfRayTips_initialized) {
		for (int i = 0;i < NumberOfRay;++i) {
			double DirectionAngle = i * (2 * PI / NumberOfRay);
			BunchOfRayTips[i] = RayTip(getSourcePosition().getXPos(), getSourcePosition().getYPos(), getSourcePosition(), DirectionAngle,0.0f);
		}
	}

	float Stepsize = 0.1f; 
	for (int i = 0;i < NumberOfRay;++i) {
		BunchOfRayTips[i].setDistanceToSource(BunchOfRayTips[i].getDistanceToSource() + this->getSourceSpeed() * ElapsedTime);
		BunchOfRayTips[i].setXPos()
	}
}