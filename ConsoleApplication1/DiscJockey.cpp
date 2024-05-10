#include "DiscJockey.h"

DiscJockey::DiscJockey()
{
	MusicTypeFlag = true; //一开始默认微分白噪音模式；
	int Offset = (GrayScale - 1) / 2; //灰度阶字符串索引所需的偏移量
	for (int i = 0;i < GrayScale;++i)
	{
		HeightDistribution[i] = 0;
	}

	//初始化：所有的Medium的Height均为0
	HeightDistribution[0 + Offset] = ScreenWidth * ScreenHeight;

	CalculatedHertz = 0;
}

void DiscJockey::CalculateHeightDistribution(const std::vector<double>& compoundHeight)
{
	int Offset = (GrayScale - 1) / 2; //灰度阶字符串索引所需的偏移量
	//计算分布，全部置为0
	for (int i = 0;i < GrayScale;++i)
	{
		HeightDistribution[i] = 0;
	}
	
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		//采取和Monitor一样的整数化方式以保持统一
		int ShownHeight = (compoundHeight[i] >= 0) ? int(compoundHeight[i] + 0.5) : int(compoundHeight[i] - 0.5);
		
		//规范化，防止越界
		if (ShownHeight > (GrayScale - 1) / 2) ShownHeight = (GrayScale - 1) / 2;
		if ((-ShownHeight) > (GrayScale - 1) / 2) ShownHeight = -(GrayScale - 1) / 2;
		
		//计数
		HeightDistribution[ShownHeight + Offset]++;
	}
}

void DiscJockey::CalculateHertz()
{
	//初始化十二音音列，从中央C开始的十二个音，中间项为0；
	double TwelveToneSeries[13] =
	{ 261.63,277.18,293.66,311.13,329.63,349.23,0,369.99,392.00,415.30,440.00,466.16,493.88 };
	
	CalculatedHertz = 0;

	//简单的尝试1：直接算加权平均的频率；
	for (int i = 0;i < 13;++i)
	{
		CalculatedHertz += HeightDistribution[i] * TwelveToneSeries[i];
	}

	CalculatedHertz /= ScreenWidth * ScreenHeight;

}

double DiscJockey::getCalculatedHertz() const
{
	return CalculatedHertz;
}

bool DiscJockey::getMusicType() const
{
	return MusicTypeFlag;
}

void DiscJockey::DetectMusicTypeChange()
{
	static bool KeyJPressed = false;
	static bool KeyKPressed = false;
	if ((GetAsyncKeyState((unsigned short)'J') & 0x8000) && (GetAsyncKeyState((unsigned short)'K') & 0x8000))
	{
		if (!KeyJPressed && !KeyKPressed) //仍然是：防止按紧变来变去的情况
		{
			MusicTypeFlag = !MusicTypeFlag;
			KeyJPressed = true;
			KeyKPressed = true;
		}
	}
	else
	{
		KeyJPressed = false;
		KeyKPressed = false;
	}
}

void DiscJockey::MakeWhiteNoise(const int& kDuration)
{
	if (MusicTypeFlag == true)
	{

		if ((CalculatedHertz <= 37)||(CalculatedHertz> 493.88))//赫兹小于37,或者大于B4，就不发声，Beep也有规定的参数范围
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(kDuration));
		}
		else
		{
			Beep(CalculatedHertz,kDuration);
		}
	}
	else
	{
		return ;
	}
}


