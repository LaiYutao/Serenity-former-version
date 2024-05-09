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
		BunchOfRayTips[i].setDistanceToSource(BunchOfRayTips[i].getDistanceToSource() + this->getSourceSpeed() * frameTime);
		double newXPos = getSourcePosition().getXPos() + BunchOfRayTips[i].getDistanceToSource() * cos(BunchOfRayTips[i].getDirectionAngle());
		BunchOfRayTips[i].setXPos(newXPos);
		double newYPos = getSourcePosition().getYPos() + BunchOfRayTips[i].getDistanceToSource() * sin(BunchOfRayTips[i].getDirectionAngle());
		BunchOfRayTips[i].setYPos(newYPos);

		//��λ�����ж��Ƿ��˿��Լ���Medium�����Σ��Ƿ����������㹻���������ܵ��������ϻ�������ȡ��������������������͸����ж�����
		double JudgeRadius = 0.5;
		bool Judger = false;
		double X = BunchOfRayTips[i].getXPos();
		double Y = BunchOfRayTips[i].getYPos();
		if (X<0 || Y<0 || X>ScreenWidth - 1 || Y>ScreenHeight - 1)
		{
			//RayTipԽ����ֱ�ӵ���һ��
			continue;
		}
		int LessX = int(X);//����ȡX����Ϊ�жϻ�׼��X����
		int LessY = int(Y);//����ȡY����Ϊ�жϻ�׼��Y����
		int ActX = int(getSourcePosition().getXPos());
		int ActY = int(getSourcePosition().getYPos());//��ʼ��ΪSource��������ﵽ���������Ļ��������ڴ洢��Ӧ����
		for (int x = LessX;x < LessX + 2;++x)
		{
			for (int y = LessY;y < LessY + 2;++y)
			{
				//λ�ڼ���뾶���Ҳ���Խ��
				if (((X - x) * (X - x) + (Y - y) * (Y - y) < JudgeRadius * JudgeRadius) && ((x >= 0) && (y >= 0) && (x <= ScreenWidth - 1) && (y <= ScreenHeight - 1)))
				{
					ActX = x;
					ActY = y;
					Judger = true;
					break;
				}
			}
		}

		//���û�ﵽ�����������Ͳ���ִ�����沽��
		if (Judger == false)continue;

		//�ж�Medium�Ƿ񱻼����(��RayTip����������ȡ�������������е�MediumPixel������)�����û�У��ͽ��伤��
		int MediumPixelIndex = ScreenWidth * ActY + ActX;
		if (!IfActivated[MediumPixelIndex])
		{
			//�趨ʱ����Ƕ��йأ��γ�����Ч��
			double SpiralParameter = 10.05 * pow(getSourceFrequency(), -1);//��������������������̬�йأ�����ʵ�顢ȡ�㡢��ϡ��ٵ������ó�ϵ����ָ�������������ݺ���Ч����ã�
			MediumLayer[MediumPixelIndex].GetActivated(timeOfNow+ SpiralParameter*(double)i/NumberOfRay/this->getSourceSpeed(), this->getSourceAmplitude(), this->getSourceFrequency(), this->getSourceInitialPhase());
			IfActivated[MediumPixelIndex] = true;
		}
	}
}