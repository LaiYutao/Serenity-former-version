#include<iostream>
#include<chrono>
#include<cmath>
#include"CircularField.h"
#include"ScreenManager.h"
#include"Medium.h"
#include"RayTip.h"
void CircularField::ActivateMedium(float ElapsedTime,Medium GlobalMedium[ScreenWidth * ScreenHeight]) {
	static const int NumberOfRay = (ScreenWidth + ScreenHeight) * 2 - 4;//��Ļ����Ȧ��ĸ�������Ϊ�������ߵ�����
	
	static bool IfActivated[ScreenWidth*ScreenHeight];
	static bool IfActivated_initialized = false;//�����γ�ʼ������
	if (!IfActivated_initialized) {
		for (int i = 0;i < ScreenWidth * ScreenHeight;++i) {
			IfActivated[i] = false;
		}
		IfActivated_initialized = true;
	}
	
	//���е����߼�˹���һ������,����ʼ��
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