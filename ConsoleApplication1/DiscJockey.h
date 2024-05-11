#ifndef DISCJOCKEY_H
#define DISCJOCKEY_H //由Medium的Height控制音符的映射。作为管理。
#include<vector>
#include"ScreenManager.h"
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define PI 3.14159
#define PI_ 0.318309


class DiscJockey
{
public:
	DiscJockey();
	void CalculateHeightDistribution(const std::vector<double>& compoundHeight);
	void CalculateHertz();
	double getCalculatedHertz()const;
	bool getMusicType()const;
	void DetectMusicTypeChange();
	void MakeWhiteNoise(const int& kDuration);
	void MakeClusters(const int& kDuration);

private:
	//初始化十二音音列，从中央C开始的十二个音，中间项为0；
	double TwelveToneSeries[13]=
	{ 261.63,277.18,293.66,311.13,329.63,349.23,0,369.99,392.00,415.30,440.00,466.16,493.88 };
	int HeightDistribution[GrayScale];
	bool MusicTypeFlag;// true对应微分白噪音模式，false对应十二音音簇模式
	double CalculatedHertz;//计算结果储存于内部，使得计算和发声分离
	
	//为了实现MakeClusters()而用到的变量，均为Windows API 相关变量，故不按照本项目其他部分的命名方式
	WAVEFORMATEX wfx;
	HWAVEOUT hWaveOut;
	WAVEHDR header;
	MMRESULT result;
};
#endif // DISCJOCKEY_H
