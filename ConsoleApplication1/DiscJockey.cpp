#include "DiscJockey.h"

DiscJockey::DiscJockey()
{
	MusicTypeFlag = true; //һ��ʼĬ��ʮ��������ģʽ��
	IfMute = false;
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

bool DiscJockey::getIfMute() const
{
	return IfMute;
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

void DiscJockey::DetectIfMute()
{
	static bool KeyMPressed = false;
	if (GetAsyncKeyState((unsigned short)'M') & 0x8000)
	{
		if (!KeyMPressed)
		{
			IfMute = !IfMute;
			KeyMPressed = true;
		}
	}
	else
	{
		KeyMPressed = false;
	}
}

void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	if (uMsg == WOM_DONE) {
	}
}

void DiscJockey::MakeWhiteNoise(const int& kDuration)
{
	result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, (DWORD_PTR)waveOutProc, 0, CALLBACK_FUNCTION);

	// �����������Ƶ����
	const int numSamples = int(44100 * kDuration / 1000); //��һǧ����ת����λ
	wfx.nSamplesPerSec = int(numSamples / (double(kDuration) / 1000)) + 1;//����һЩȡ������������С��0.001�������ң�
	short* audioData = new short[numSamples];
	for (int i = 0; i < numSamples; ++i)
	{
		double t = (double)i / wfx.nSamplesPerSec; // ȡϸ�ֵ�ʱ��
		audioData[i] = static_cast < short>(10000*sin(CalculatedHertz * 2 * PI * t));
	}

	//�ٳ�ĩ�˲����������ͼ�����������ֵ�Ƚϸߵ�ʱ��ͻȻֹͣ��Ƶ�������������ú������������������Ϊĳ�̶ֳ��ϵĵ�ͨ�˲���
	if (kDuration > 20) //��ֹԽ��
	{
		double absminIntensity = 10000;
		int minNum = numSamples - int(0.02 * wfx.nSamplesPerSec);
		for (int i = numSamples - 1;i > numSamples - int(0.02 * wfx.nSamplesPerSec);--i)
		{
			if (abs(audioData[i]) < absminIntensity)
			{
				absminIntensity = abs(audioData[i]);
				minNum = i;
			}
		}
		for (int i = minNum + 1;i < numSamples; ++i)
		{
			audioData[i] = static_cast<short>(absminIntensity * (numSamples - i) / (numSamples - minNum - 1));
		}
	}

	header.lpData = (LPSTR)audioData;
	header.dwBufferLength = numSamples * sizeof(short);
	header.dwFlags = 0;
	header.dwLoops = 0;

	//д�벢����
	result = waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	result = waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
}

void DiscJockey::MakeClusters(const int& kDuration)
{
	result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, (DWORD_PTR)waveOutProc, 0, CALLBACK_FUNCTION);
	
	// �����������Ƶ����
	const int numSamples = int(44100 * kDuration/1000); //��һǧ����ת����λ
	wfx.nSamplesPerSec = int(numSamples / (double(kDuration) / 1000))+1;//����һЩȡ������������С��0.001�������ң�
	short* audioData = new short[numSamples];
	for (int i = 0; i < numSamples; ++i) 
	{
		double t = (double)i / wfx.nSamplesPerSec; // ȡϸ�ֵ�ʱ��
		double Intensity = 0;
		//ʮ��������ȥ�������õ������Һ�����Height==0��Ӧ��0���ȣ���Ӧ�������ֱ�����㣬�ܷ���
		for (int j = 0;j < 13;j++)
		{
			//��һ���ǻ�������������������������������Ƶ��ͼ�������������ͼ���Ե�ģ�������ɫ
			Intensity += HeightDistribution[j]*sin(TwelveToneSeries[j]* 2*PI * t) + 
			0.1*HeightDistribution[j] * sin(TwelveToneSeries[j]* 2 * 2 * PI * t) + 0.056 * HeightDistribution[j] * sin(TwelveToneSeries[j] * 3 * 2 * PI * t)+
			0.042 * HeightDistribution[j] * sin(TwelveToneSeries[j] * 4 * 2 * PI * t)+ 0.037 * HeightDistribution[j] * sin(TwelveToneSeries[j] *5 * 2 * PI * t);
		}
		
		Intensity /= ScreenWidth * ScreenHeight; //���ȡ��ֵ
		Intensity *= 12000;// �ʵ��Ŵ�ʹ�ò�������,�����Ͱ�����ģʽ�ӽ�����
		audioData[i] = static_cast<short>(Intensity); 
	}

	//�ٳ�ĩ�˲����������ͼ��������Ƚϸߵ�ʱ��ͻȻֹͣ��Ƶ�������������ú������������������Ϊĳ�̶ֳ��ϵĵ�ͨ�˲���
	if(kDuration>20) //��ֹԽ��
	{
		double absminIntensity = 10000;
		int minNum = numSamples - int(0.02 * wfx.nSamplesPerSec);
		for (int i = numSamples - 1;i > numSamples - int(0.02 * wfx.nSamplesPerSec);--i)
		{
			if (abs(audioData[i]) < absminIntensity)
			{
				absminIntensity = abs(audioData[i]);
				minNum = i;
			}
		}
		for (int i = minNum + 1;i < numSamples; ++i)
		{
			audioData[i] = static_cast<short>(absminIntensity * (numSamples - i) / (numSamples - minNum - 1));
		}
	}
	
	header.lpData = (LPSTR)audioData;
	header.dwBufferLength = numSamples * sizeof(short);
	header.dwFlags = 0;
	header.dwLoops = 0;

	//д�벢����
	result = waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	result = waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
}