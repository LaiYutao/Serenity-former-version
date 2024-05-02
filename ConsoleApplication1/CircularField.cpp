#include"CircularField.h"
#include"ScreenManager.h"
#include"Medium.h"
#include"RayTip.h"

CircularField::CircularField(const Point& s_Position, const double& s_Amplitude, const double& s_Frequency, const double& s_InitialPhase, const int& s_Speed)
	:Field(s_Position, s_Amplitude, s_Frequency, s_InitialPhase, s_Speed)
{
}

void CircularField::ActivateMedium(const double& timeOfNow, const double& frameTime,Medium globalMedium[ScreenWidth * ScreenHeight])
{
	static const int NumberOfRay = (ScreenWidth + ScreenHeight) * 2 - 4;//��Ļ����Ȧ��ĸ�������Ϊ�������ߵ�����
	
	static bool IfActivated[ScreenWidth*ScreenHeight];
	static bool IfActivated_initialized = false;//�����γ�ʼ������
	if (!IfActivated_initialized) 
	{
		for (int i = 0;i < ScreenWidth * ScreenHeight;++i) 
		{
			IfActivated[i] = false;
		}
		IfActivated_initialized = true;
	}
	
	//���е����߼�˹���һ������,����ʼ��
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

	//����ÿһ������
	for (int i = 0;i < NumberOfRay;++i) 
	{
		BunchOfRayTips[i].setDistanceToSource(BunchOfRayTips[i].getDistanceToSource() + this->getSourceSpeed() * frameTime);
		double newXPos = getSourcePosition().getXPos() + BunchOfRayTips[i].getDistanceToSource() * cos(BunchOfRayTips[i].getDirectionAngle());
		BunchOfRayTips[i].setXPos(newXPos);
		double newYPos = getSourcePosition().getYPos() + BunchOfRayTips[i].getDistanceToSource() * sin(BunchOfRayTips[i].getDirectionAngle());
		BunchOfRayTips[i].setYPos(newYPos);
		
		//�ж�Medium�Ƿ񱻼����(��RayTip������ȡ�������������е�MediumPixel������)�����û�У��ͽ��伤��
		int MediumPixelIndex = ScreenWidth * (int)BunchOfRayTips[i].getYPos() + (int)BunchOfRayTips[i].getXPos();
		if (!IfActivated[MediumPixelIndex])
		{
			globalMedium[MediumPixelIndex].GetActivated(timeOfNow,this->getSourceAmplitude(),this->getSourceFrequency(),this->getSourceInitialPhase());
			IfActivated[MediumPixelIndex] = true;
		}
	}
}