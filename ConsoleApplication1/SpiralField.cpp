#include"SpiralField.h"

SpiralField::SpiralField(const double& creationTime, const Point& s_Position, const double& s_Amplitude, const double& s_Frequency, const double& s_InitialPhase, const double& s_Speed)
	:Field(creationTime, s_Position, s_Amplitude, s_Frequency, s_InitialPhase, s_Speed)
{
}

void SpiralField::ActivateMedium(const double& timeOfNow, const double& frameTime)
{

	//���ÿ��Medium�㶼�Ѿ�������Ͳ��ٽ��м���ĳ������
	static bool StopActivation = false;
	if (StopActivation == true)return;//ֻҪ��һ��GameLoop�У�Mediumȫ�����Ѽ����ô��������ѭ���У��������

	bool CheckActivation = true;
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		if (!IfActivated[i])
		{
			CheckActivation = false;
			break;
		}
	}
	if (CheckActivation == true)
	{
		StopActivation = true;
	}

	//�����Ǽ��δ���ʱ���������裺

	//����ÿһ������
	for (int i = 0;i < NumberOfRay;++i)
	{
		BunchOfRayTips[i].setDistanceToSource(BunchOfRayTips[i].getDistanceToSource() + this->getSourceSpeed() * frameTime/1000);
		double newXPos = getSourcePosition().getXPos() + BunchOfRayTips[i].getDistanceToSource() * cos(BunchOfRayTips[i].getDirectionAngle());
		BunchOfRayTips[i].setXPos(newXPos);
		double newYPos = getSourcePosition().getYPos() + BunchOfRayTips[i].getDistanceToSource() * sin(BunchOfRayTips[i].getDirectionAngle());
		BunchOfRayTips[i].setYPos(newYPos);

		//�ж�Medium�Ƿ񱻼����(��RayTip����������ȡ�������������е�MediumPixel������)�����û�У��ͽ��伤��
		int MediumPixelIndex = ScreenWidth * int(BunchOfRayTips[i].getYPos() - 1) + int(BunchOfRayTips[i].getXPos() - 1);
		if (!IfActivated[MediumPixelIndex])
		{
			//�趨ʱ����Ƕ��йأ��γ�����Ч��
			MediumLayer[MediumPixelIndex].GetActivated(timeOfNow+i/NumberOfRay/this->getSourceSpeed(), this->getSourceAmplitude(), this->getSourceFrequency(), this->getSourceInitialPhase());
			IfActivated[MediumPixelIndex] = true;
		}
	}
}