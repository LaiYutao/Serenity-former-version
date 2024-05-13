#include"Gardener.h"
#include"Monitor.h"
#include"DiscJockey.h"
#include<fstream>
#include<string>
#include<chrono>
#include<random>

//包含主loop在内的主干
void Act(ScreenManager TheScreenManager)
{
	//设定时间原点
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	auto tp1 = std::chrono::high_resolution_clock::now();
	auto tp2 = std::chrono::high_resolution_clock::now();

	//初始化三个单例
	Gardener TheGardener;
	Monitor TheMonitor;
	DiscJockey TheDiscJockey;

	//主循环
	while (1) 
	{
		//获取ElapsedTime
		tp2 = std::chrono::high_resolution_clock::now();
		auto ElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
		tp1 = tp2;

		//获取时间轴中当前时间
		double TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
		
		//TheDiscJockey检测是否处于静音模式
		TheDiscJockey.DetectIfMute();

		//TheDiscJockey检测MusicType是否改变
		TheDiscJockey.DetectMusicTypeChange();

		//如果不是处于静音模式，则创建对应音乐模式的线程
		std::thread MusicMode;
		if (TheDiscJockey.getIfMute() == false)
		{
			if (TheDiscJockey.getMusicType() == true)
			{
				//十二音音簇模式
				MusicMode = std::thread([&TheDiscJockey, ElapsedTime]() { TheDiscJockey.MakeClusters(0.5*ElapsedTime); }); 
			}
			else
			{
				//微分白噪音模式
				MusicMode = std::thread([&TheDiscJockey, ElapsedTime]() { TheDiscJockey.MakeWhiteNoise(0.5*ElapsedTime); });
			}
		}

		//启动PlantingPoint的选择
		TheGardener.SelectPosition();

		//启动“种植”功能
		TheGardener.PlantIt(TimeOfNow);

		//遍历CompoundField中的每一个场,让每个场开始在对应Medium层中对四周的Medium进行激活
		for (auto TheField : TheGardener.getRefCompoundField())
		{
			TheField->ActivateMedium(TimeOfNow, double(FrameTime)/1000);
		}
		
		//让每个场对应的Medium层的每个Medium都振动（改变Height）
		for (auto TheField : TheGardener.getRefCompoundField())
		{
			for (int i = 0;i < ScreenWidth * ScreenHeight;++i) 
			{
				(*TheField->getPtrMediumLayer())[i].Vibrate(TimeOfNow);
			}
		}

		//TheMonitor处理各Medium层的叠加
		TheMonitor.UpdateCompoundHeight(TheGardener.getCompoundMedium());

		//TheMonitor将Height转化为字符
		TheMonitor.ChangeIntoPixel(TheScreenManager.getRefScreenBuffer());

		//TheMonitor加上PlantingPoint的显示
		TheMonitor.AddPlantingPoint(TheScreenManager.getRefScreenBuffer(), TheGardener.getPlantingPoint());

		//TheDiscJockey接收CompoundHeight
		TheDiscJockey.CalculateHeightDistribution(TheMonitor.getCompoundHeight());
		
		//TheDiscJockey计算在微分白噪音模式下发出赫兹
		TheDiscJockey.CalculateHertz();

		//等待MusicMode进程完成
		if (MusicMode.joinable())
		{
			MusicMode.join();
		}
		
		//TheScreenManager输出图像
		TheScreenManager.ShowImage();

		// 控制帧率；
		if (ElapsedTime < FrameTime)//单位为毫秒
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(FrameTime - ElapsedTime));
		}
	}
}

void CoverPage(ScreenManager TheScreenManager)
{
	//读取文件
	std::ifstream inFile("txtfile\\CoverPage.txt");
	char c;
	while (inFile.get(c))
	{
		if (c != '\n') //消除换行符
		{
			TheScreenManager.getRefScreenBuffer().push_back(c);
		}
	}
	inFile.close();

	bool Out = false;
	bool KeySpacePressed = false;

	//输出Page
	while (Out == false)
	{
		//检查是否按下空格键
		if (GetAsyncKeyState((unsigned short)' ') & 0x8000)
		{
			if (!KeySpacePressed)
			{
				Out = true;
				KeySpacePressed = true;
			}
		}
		else
		{
			KeySpacePressed = false;
		}
		TheScreenManager.ShowStaticImage();
	}
	TheScreenManager.SetEmptyBuffer();
}

void BridgePage(ScreenManager TheScreenManager)
{
	//设定时间原点
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	double TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
	//读取文件
	std::ifstream inFile("txtfile\\BridgePage.txt");
	char c;
	while (inFile.get(c))
	{
		TheScreenManager.getRefScreenBuffer().push_back(c);
	}
	inFile.close();

	//设定此页持续时间
	std::uniform_real_distribution<double> dis(0.2, 1.5);//比较随意的时间区间，从中随机选择，作为此页显示时间
	std::default_random_engine gen;
	double Duration= dis(gen);

	//输出page
	while (TimeOfNow < Duration)
	{
		//获取时间轴中当前时间
		TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
		TheScreenManager.ShowStaticImage();
	}
	TheScreenManager.SetEmptyBuffer();
}

int main() 
{
	ScreenManager TheScreenManager;
	CoverPage(TheScreenManager);
	BridgePage(TheScreenManager);
	Act(TheScreenManager);
	return 0;
}