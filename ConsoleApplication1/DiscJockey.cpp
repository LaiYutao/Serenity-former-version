#include "DiscJockey.h"

DiscJockey::DiscJockey()
{
	MusicTypeFlag = true; //һ��ʼĬ��΢�ְ�����ģʽ��
	
	int Offset = (GrayScale - 1) / 2; //�ҶȽ��ַ������������ƫ����
	
	//��ʼ�������е�Medium��Height��Ϊ0
	for (int i = 0;i < GrayScale;++i)
	{
		HeightDistribution[i] = 0;
	}

	HeightDistribution[0 + Offset] = ScreenWidth * ScreenHeight;

	CalculatedHertz = 0;

	// ���ò��θ�ʽ
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 1;                  // ������
	wfx.nSamplesPerSec = 44100;         // ������
	wfx.wBitsPerSample = 16;            // λ���
	wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.cbSize = 0;

}

void DiscJockey::CalculateHeightDistribution(const std::vector<double>& compoundHeight)
{
	int Offset = (GrayScale - 1) / 2; //�ҶȽ��ַ������������ƫ����
	//����ֲ���ȫ����Ϊ0
	for (int i = 0;i < GrayScale;++i)
	{
		HeightDistribution[i] = 0;
	}
	
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		//��ȡ��Monitorһ������������ʽ�Ա���ͳһ
		int ShownHeight = (compoundHeight[i] >= 0) ? int(compoundHeight[i] + 0.5) : int(compoundHeight[i] - 0.5);
		
		//�淶������ֹԽ��
		if (ShownHeight > (GrayScale - 1) / 2) ShownHeight = (GrayScale - 1) / 2;
		if ((-ShownHeight) > (GrayScale - 1) / 2) ShownHeight = -(GrayScale - 1) / 2;
		
		//����
		HeightDistribution[ShownHeight + Offset]++;
	}
}

void DiscJockey::CalculateHertz()
{
	CalculatedHertz = 0;

	//�򵥵ĳ���1��ֱ�����Ȩƽ����Ƶ�ʣ�
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
		if (!KeyJPressed && !KeyKPressed) //��Ȼ�ǣ���ֹ����������ȥ�����
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
	if ((CalculatedHertz <= 37)||(CalculatedHertz> 493.88))//����С��37,���ߴ���B4���Ͳ�������BeepҲ�й涨�Ĳ�����Χ
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
	
	// �����������Ƶ����
	const int numSamples = int(44100 * kDuration/1000); //��һǧ����ת����λ
	wfx.nSamplesPerSec = int(numSamples / (double(kDuration) / 1000))+1;//����һЩȡ�����������
	short* audioData = new short[numSamples];
	for (int i = 0; i < numSamples; ++i) {
		double t = (double)i / wfx.nSamplesPerSec; // ȡϸ�ֵ�ʱ��
		double Intensity = 0;
		//ʮ��������ȥ�������õ������Һ�����Height==0��Ӧ��0���ȣ���Ӧ�������ֱ�����㣬�ܷ���
		for (int j = 0;j < 13;j++)
		{
			Intensity += HeightDistribution[j]*sin(TwelveToneSeries[j] * t);
		}
		
		Intensity /= ScreenWidth * ScreenHeight; //���ȡ��ֵ
		Intensity *= 5000;// �ʵ��Ŵ�ʹ�ò�������,�����Ͱ�����ģʽ�Ƚ��񼴿�
		
		audioData[i] = static_cast<short>(Intensity); 
	}

	header.lpData = (LPSTR)audioData;
	header.dwBufferLength = numSamples * sizeof(short);
	header.dwFlags = 0;
	header.dwLoops = 0;

	result = waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	result = waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
}


