#include "Gardener.h"
#include"windows.h"

Gardener::Gardener():PlantingPoint(Point(0,0))
{
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		SourceChecking[i] =0;
	}

}

Point Gardener::getPlantingPoint() const
{
	return PlantingPoint;
}

std::vector<Field*> Gardener::getCompoundField() const
{
	return CompoundField;
}

void Gardener::SelectPosition()
{
	double MovingSpeed = 2;//�ƶ��ٶ�Ϊ2 pixel per sec
	
	//����: ���¡�W�����Ҳ����磬�Ҵ�ʱû�а��š�J���͡�K����
	if ((GetAsyncKeyState((unsigned short)'W') & 0x8000)&&(PlantingPoint.getYPos()>0)&& !(GetAsyncKeyState((unsigned short)'J') & 0x8000)&& !(GetAsyncKeyState((unsigned short)'K') & 0x8000))
	{
		PlantingPoint.setYPos(PlantingPoint.getYPos()- MovingSpeed * FrameTime/1000);
	}

	//����: ���¡�S�����Ҳ����磬�Ҵ�ʱû�а��š�J���͡�K����
	if ((GetAsyncKeyState((unsigned short)'S') & 0x8000) && (PlantingPoint.getYPos() < ScreenHeight) && !(GetAsyncKeyState((unsigned short)'J') & 0x8000) && !(GetAsyncKeyState((unsigned short)'K') & 0x8000))
	{
		PlantingPoint.setYPos(PlantingPoint.getYPos() + MovingSpeed * FrameTime / 1000);
	}

	//���󣺰��¡�A�����Ҳ����磬�Ҵ�ʱû�а��š�J���͡�K����
	if ((GetAsyncKeyState((unsigned short)'A') & 0x8000) && (PlantingPoint.getXPos() > 0) && !(GetAsyncKeyState((unsigned short)'J') & 0x8000) && !(GetAsyncKeyState((unsigned short)'K') & 0x8000))
	{
		PlantingPoint.setXPos(PlantingPoint.getXPos() - MovingSpeed * FrameTime / 1000);
	}

	//���ң����¡�D�����Ҳ����磬�Ҵ�ʱû�а��š�J���͡�K����
	if ((GetAsyncKeyState((unsigned short)'D') & 0x8000) && (PlantingPoint.getXPos() < ScreenWidth) && !(GetAsyncKeyState((unsigned short)'J') & 0x8000) && !(GetAsyncKeyState((unsigned short)'K') & 0x8000))
	{
		PlantingPoint.setXPos(PlantingPoint.getXPos() + MovingSpeed * FrameTime / 1000);
	}
}

void Gardener::PlantIt(const double& timeOfNow)
{
	//ֱ�Ӵ���ʱ��Ĭ�ϲ���
	static double DefaultAmplitude = 1;
	static double DefaultFrequency = 0.25;//��Ӧ��Ƶ��Ϊ��/2����Ӧ����ΪT=4s
	static const double DefaultInitialPhase = PI / 2;//Source���������������
	static const double DefaultSpeed = 1;//�ó���λ�Ͳ��ٲ��䣻�������ӣ�
	
	static bool KeyJPressed = false;
	static bool KeyKPressed = false;

	//����˴�û�����¹�Source
	if (!SourceChecking[(int(PlantingPoint.getYPos()) * ScreenHeight + int(PlantingPoint.getXPos()))])
	{
		if ((GetAsyncKeyState((unsigned short)'J') & 0x8000)&& !KeyJPressed)
		{
			CompoundField.push_back(new CircularField(timeOfNow, PlantingPoint, DefaultAmplitude, DefaultFrequency, DefaultInitialPhase, DefaultSpeed));
			SourceChecking[(int(PlantingPoint.getYPos()) * ScreenHeight + int(PlantingPoint.getXPos()))] = CompoundField.size();//��¼��ʱ�����ĳ�����ţ��ӡ�1����ʼ
			KeyJPressed = true;//��ֹ�մ����Ϳ�ʼ���ڲ�������Ӧ���ȷſ�һ�£��ٿ����Ƿ�ʼ����
		}
		else if ((GetAsyncKeyState((unsigned short)'K') & 0x8000)&& !KeyKPressed)
		{
			//SpiralField �д�ʵ��
			//CompoundField.push_back(new SpiralField(timeOfNow, PlantingPoint, DefaultAmplitude, DefaultFrequency, DefaultInitialPhase, DefaultSpeed));
			//SourceChecking[(int(PlantingPoint.getYPos()) * ScreenHeight + int(PlantingPoint.getXPos()))] = true;
			KeyKPressed = true;
		}
		else return;
	}
	else //����˴��Ѿ����¹�Source
	{
		if ((GetAsyncKeyState((unsigned short)'J') & 0x8000)&&!KeyJPressed)
		{
			AdjustAmplitude();//������J�������ͽ����Amplitude�ĵ��ڣ��ɿ��ͽ�������
		}
		else if ((GetAsyncKeyState((unsigned short)'K') & 0x8000)&&!KeyKPressed)
		{
			AdjustFrequency();//������K�������ͽ����Amplitude�ĵ��ڣ��ɿ��ͽ������ڣ�
		}
		else
		{
			KeyJPressed = false;
			KeyKPressed = false; //������桰��ֹ�մ����͵���������
		}
	}
}

void Gardener::AdjustAmplitude()
{
	static bool KeyWPressed = false;
	static bool KeySPressed = false;
	static double AmplitudeUpperLimit = (GrayScale - 1) / 2;//����̫�ߣ�����Ч������
	
	//��ӦCompoundField�еı�ţ���0��ʼ��
	int NumField = SourceChecking[(int(PlantingPoint.getYPos()) * ScreenHeight + int(PlantingPoint.getXPos()))]-1;
	
	//���¡�W������������ߣ������ܳ���AmplitudeUpperLimit
	if ((GetAsyncKeyState((unsigned short)'W') & 0x8000)&&(CompoundField[NumField]->getSourceAmplitude()< AmplitudeUpperLimit))
	{
		if (!KeyWPressed) //��ֹ��������
		{
			CompoundField[NumField]->setSourceAmplitude(CompoundField[NumField]->getSourceAmplitude() + 1);
			CompoundField[NumField]->ClearIfActivated();
			KeyWPressed = true;
		}
	}
	else
	{
		KeyWPressed = false;
	}

	//���¡�S������������ߣ������ܵ���0
	if ((GetAsyncKeyState((unsigned short)'S') & 0x8000) && (CompoundField[NumField]->getSourceAmplitude() >0))
	{
		if (!KeySPressed) //��ֹ��������
		{
			CompoundField[NumField]->setSourceAmplitude(CompoundField[NumField]->getSourceAmplitude() - 1);
			CompoundField[NumField]->ClearIfActivated();
			KeySPressed = true;
		}
	}
	else
	{
		KeySPressed = false;
	}
}

void Gardener::AdjustFrequency()
{
	static bool KeyWPressed = false;
	static bool KeySPressed = false;
	static double FrequencyUpperLimit = 1;//̫��Ļ�����ʾ�����

	//��ӦCompoundField�еı�ţ���0��ʼ��
	int NumField = SourceChecking[(int(PlantingPoint.getYPos()) * ScreenHeight + int(PlantingPoint.getXPos()))] - 1;

	//���¡�W������Ƶ�ʵ��ߣ������ܳ���FrequencyUpperLimit
	if ((GetAsyncKeyState((unsigned short)'W') & 0x8000) && (CompoundField[NumField]->getSourceFrequency() < FrequencyUpperLimit))
	{
		if (!KeyWPressed) //��ֹ��������
		{
			CompoundField[NumField]->setSourceFrequency(CompoundField[NumField]->getSourceFrequency() + 0.05);
			CompoundField[NumField]->ClearIfActivated();
			KeyWPressed = true;
		}
	}
	else
	{
		KeyWPressed = false;
	}

	//���¡�S������Ƶ�ʵ��ߣ������ܵ���0
	if ((GetAsyncKeyState((unsigned short)'S') & 0x8000) && (CompoundField[NumField]->getSourceFrequency() > 0))
	{
		if (!KeySPressed) //��ֹ��������
		{
			CompoundField[NumField]->setSourceFrequency(CompoundField[NumField]->getSourceFrequency() - 0.05);
			CompoundField[NumField]->ClearIfActivated();
			KeySPressed = true;
		}
	}
	else
	{
		KeySPressed = false;
	}
}


