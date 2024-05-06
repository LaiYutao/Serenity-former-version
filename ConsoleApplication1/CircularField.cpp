#include"CircularField.h"

CircularField::CircularField(const double& creationTime,const Point& s_Position, const double& s_Amplitude, const double& s_Frequency, const double& s_InitialPhase, const double& s_Speed)
	:Field(creationTime,s_Position, s_Amplitude, s_Frequency, s_InitialPhase, s_Speed)
{
}

void CircularField::ActivateMedium(const double& timeOfNow, const double& frameTime)
{
	//���ÿ��Medium�㶼�Ѿ�������Ͳ��ٽ��м���ĳ������
	if (StopActivation == true)return;//ֻҪ��һ��GameLoop�У�Mediumȫ�����Ѽ����ôֻҪ����û�иı䣬��������ѭ���У��������ټ���
	
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
	
	////��source��ֱ�Ӽ���
	//if (!IfActivated[int(getSourcePosition().getYPos() * ScreenWidth + getSourcePosition().getXPos())])
	//{
	//	MediumLayer[int(getSourcePosition().getYPos() * ScreenWidth + getSourcePosition().getXPos())].GetActivated(timeOfNow, this->getSourceAmplitude(), this->getSourceFrequency(), this->getSourceInitialPhase());
	//	IfActivated[int(getSourcePosition().getYPos() * ScreenWidth + getSourcePosition().getXPos())] = true;
	//	//Sleep(int(1000 / getSourceSpeed()));//�ȴ�����ʱ�䣨����һ��Pixel��ʱ�䣩��������Χ�벨Դ���һȦ�ĵ�Ҳ����������
	//}

	//����ÿһ������
	for (int i = 0;i < NumberOfRay;++i) 
	{
		BunchOfRayTips[i].setDistanceToSource(BunchOfRayTips[i].getDistanceToSource() + this->getSourceSpeed() * frameTime/1000);
		double newXPos = getSourcePosition().getXPos() + BunchOfRayTips[i].getDistanceToSource() * cos(BunchOfRayTips[i].getDirectionAngle());
		BunchOfRayTips[i].setXPos(newXPos);
		double newYPos = getSourcePosition().getYPos() + BunchOfRayTips[i].getDistanceToSource() * sin(BunchOfRayTips[i].getDirectionAngle());
		BunchOfRayTips[i].setYPos(newYPos);
		
		//�ж�Medium�Ƿ񱻼����(��RayTip������ȡ�������������е�MediumPixel������)�����û�У��ͽ��伤��
		int MediumPixelIndex = ScreenWidth * int(BunchOfRayTips[i].getYPos()) + int(BunchOfRayTips[i].getXPos());
		if (!IfActivated[MediumPixelIndex])
		{
			MediumLayer[MediumPixelIndex].GetActivated(timeOfNow,this->getSourceAmplitude(),this->getSourceFrequency(),this->getSourceInitialPhase());
			IfActivated[MediumPixelIndex] = true;
		}
	}
}