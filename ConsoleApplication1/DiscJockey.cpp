#include "DiscJockey.h"

DiscJockey::DiscJockey()
{
	MusicTypeFlag = true; //一开始默认微分白噪音模式；
	
	int Offset = (GrayScale - 1) / 2; //灰度阶字符串索引所需的偏移量
	
	//初始化：所有的Medium的Height均为0
	for (int i = 0;i < GrayScale;++i)
	{
		HeightDistribution[i] = 0;
	}

	HeightDistribution[0 + Offset] = ScreenWidth * ScreenHeight;

	CalculatedHertz = 0;

	// 配置波形格式
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 1;                  // 单声道
	wfx.nSamplesPerSec = 44100;         // 采样率
	wfx.wBitsPerSample = 16;            // 位深度
	wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.cbSize = 0;

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
	if ((CalculatedHertz <= 37)||(CalculatedHertz> 493.88))//赫兹小于37,或者大于B4，就不发声，Beep也有规定的参数范围
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(kDuration));
	}
	else
	{
		Beep(CalculatedHertz,kDuration);
	}
}

void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	if (uMsg == WOM_DONE) {
	}
}

void DiscJockey::MakeClusters(const int& kDuration)
{
	
	result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, (DWORD_PTR)waveOutProc, 0, CALLBACK_FUNCTION);
	
	// 创建并填充音频数据
	const int numSamples = int(44100 * kDuration/1000); //除一千用于转换单位
	wfx.nSamplesPerSec = int(numSamples / (double(kDuration) / 1000))+1;//抵消一些取整带来的误差
	short* audioData = new short[numSamples];
	for (int i = 0; i < numSamples; ++i) {
		double t = (double)i / wfx.nSamplesPerSec; // 取细分的时间
		double Intensity = 0;
		//十二音叠上去；由于用的是正弦函数，Height==0对应的0赫兹，对应的振幅就直接是零，很方便
		for (int j = 0;j < 13;j++)
		{
			Intensity += HeightDistribution[j]*sin(TwelveToneSeries[j] * t);
		}
		
		Intensity /= ScreenWidth * ScreenHeight; //振幅取均值
		Intensity *= 5000;// 适当放大，使得播放正常,音量和白噪音模式比较像即可
		
		audioData[i] = static_cast<short>(Intensity); 
	}

	header.lpData = (LPSTR)audioData;
	header.dwBufferLength = numSamples * sizeof(short);
	header.dwFlags = 0;
	header.dwLoops = 0;

	result = waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	result = waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
}


