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
	double MovingSpeed = 2;//移动速度为2 pixel per sec
	
	//向上: 按下“W”，且不超界，且此时没有按着“J”和“K”；
	if ((GetAsyncKeyState((unsigned short)'W') & 0x8000)&&(PlantingPoint.getYPos()>0)&& !(GetAsyncKeyState((unsigned short)'J') & 0x8000)&& !(GetAsyncKeyState((unsigned short)'K') & 0x8000))
	{
		PlantingPoint.setYPos(PlantingPoint.getYPos()- MovingSpeed * FrameTime/1000);
	}

	//向下: 按下“S”，且不超界，且此时没有按着“J”和“K”；
	if ((GetAsyncKeyState((unsigned short)'S') & 0x8000) && (PlantingPoint.getYPos() < ScreenHeight) && !(GetAsyncKeyState((unsigned short)'J') & 0x8000) && !(GetAsyncKeyState((unsigned short)'K') & 0x8000))
	{
		PlantingPoint.setYPos(PlantingPoint.getYPos() + MovingSpeed * FrameTime / 1000);
	}

	//向左：按下“A”，且不超界，且此时没有按着“J”和“K”；
	if ((GetAsyncKeyState((unsigned short)'A') & 0x8000) && (PlantingPoint.getXPos() > 0) && !(GetAsyncKeyState((unsigned short)'J') & 0x8000) && !(GetAsyncKeyState((unsigned short)'K') & 0x8000))
	{
		PlantingPoint.setXPos(PlantingPoint.getXPos() - MovingSpeed * FrameTime / 1000);
	}

	//向右：按下“D”，且不超界，且此时没有按着“J”和“K”；
	if ((GetAsyncKeyState((unsigned short)'D') & 0x8000) && (PlantingPoint.getXPos() < ScreenWidth) && !(GetAsyncKeyState((unsigned short)'J') & 0x8000) && !(GetAsyncKeyState((unsigned short)'K') & 0x8000))
	{
		PlantingPoint.setXPos(PlantingPoint.getXPos() + MovingSpeed * FrameTime / 1000);
	}
}

void Gardener::PlantIt(const double& timeOfNow)
{
	//直接创建时的默认参数
	static double DefaultAmplitude = 1;
	static double DefaultFrequency = 0.25;//对应角频率为π/2；对应周期为T=4s
	static const double DefaultInitialPhase = PI / 2;//Source处振动起初是向下振动
	static const double DefaultSpeed = 1;//让初相位和波速不变；避免冗杂；
	
	static bool KeyJPressed = false;
	static bool KeyKPressed = false;

	//如果此处没有种下过Source
	if (!SourceChecking[(int(PlantingPoint.getYPos()) * ScreenHeight + int(PlantingPoint.getXPos()))])
	{
		if ((GetAsyncKeyState((unsigned short)'J') & 0x8000)&& !KeyJPressed)
		{
			CompoundField.push_back(new CircularField(timeOfNow, PlantingPoint, DefaultAmplitude, DefaultFrequency, DefaultInitialPhase, DefaultSpeed));
			SourceChecking[(int(PlantingPoint.getYPos()) * ScreenHeight + int(PlantingPoint.getXPos()))] = CompoundField.size();//记录此时创建的场的序号，从“1”开始
			KeyJPressed = true;//防止刚创建就开始调节参数；而应该先放开一下，再考虑是否开始调节
		}
		else if ((GetAsyncKeyState((unsigned short)'K') & 0x8000)&& !KeyKPressed)
		{
			//SpiralField 有待实现
			//CompoundField.push_back(new SpiralField(timeOfNow, PlantingPoint, DefaultAmplitude, DefaultFrequency, DefaultInitialPhase, DefaultSpeed));
			//SourceChecking[(int(PlantingPoint.getYPos()) * ScreenHeight + int(PlantingPoint.getXPos()))] = true;
			KeyKPressed = true;
		}
		else return;
	}
	else //如果此处已经种下过Source
	{
		if ((GetAsyncKeyState((unsigned short)'J') & 0x8000)&&!KeyJPressed)
		{
			AdjustAmplitude();//按紧“J”键，就进入对Amplitude的调节；松开就结束调节
		}
		else if ((GetAsyncKeyState((unsigned short)'K') & 0x8000)&&!KeyKPressed)
		{
			AdjustFrequency();//按紧“K”键，就进入对Amplitude的调节；松开就结束调节；
		}
		else
		{
			KeyJPressed = false;
			KeyKPressed = false; //配合上面“防止刚创建就调整参数”
		}
	}
}

void Gardener::AdjustAmplitude()
{
	static bool KeyWPressed = false;
	static bool KeySPressed = false;
	static double AmplitudeUpperLimit = (GrayScale - 1) / 2;//不能太高，否则效果不好
	
	//对应CompoundField中的编号，从0开始；
	int NumField = SourceChecking[(int(PlantingPoint.getYPos()) * ScreenHeight + int(PlantingPoint.getXPos()))]-1;
	
	//按下“W”，把振幅调高；但不能超过AmplitudeUpperLimit
	if ((GetAsyncKeyState((unsigned short)'W') & 0x8000)&&(CompoundField[NumField]->getSourceAmplitude()< AmplitudeUpperLimit))
	{
		if (!KeyWPressed) //防止按键连续
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

	//按下“S”，把振幅调高；但不能低于0
	if ((GetAsyncKeyState((unsigned short)'S') & 0x8000) && (CompoundField[NumField]->getSourceAmplitude() >0))
	{
		if (!KeySPressed) //防止按键连续
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
	static double FrequencyUpperLimit = 1;//太快的话，显示不清楚

	//对应CompoundField中的编号，从0开始；
	int NumField = SourceChecking[(int(PlantingPoint.getYPos()) * ScreenHeight + int(PlantingPoint.getXPos()))] - 1;

	//按下“W”，把频率调高；但不能超过FrequencyUpperLimit
	if ((GetAsyncKeyState((unsigned short)'W') & 0x8000) && (CompoundField[NumField]->getSourceFrequency() < FrequencyUpperLimit))
	{
		if (!KeyWPressed) //防止按键连续
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

	//按下“S”，把频率调高；但不能低于0
	if ((GetAsyncKeyState((unsigned short)'S') & 0x8000) && (CompoundField[NumField]->getSourceFrequency() > 0))
	{
		if (!KeySPressed) //防止按键连续
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


