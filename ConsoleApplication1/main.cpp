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

		//TheDiscJockey检测MusicType是否改变
		TheDiscJockey.DetectMusicTypeChange();

		//TheDiscJockey接收CompoundHeight
		TheDiscJockey.CalculateHeightDistribution(TheMonitor.getCompoundHeight());
		
		//TheDiscJockey计算在微分白噪音模式下发出赫兹
		TheDiscJockey.CalculateHertz();

		//TheScreenManager输出图像
		TheScreenManager.ShowImage();

		// 控制帧率；并在等待时间发声；
		if (ElapsedTime < FrameTime)//单位为毫秒
		{
			if (!TheDiscJockey.getMusicType())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(FrameTime - ElapsedTime));
			}
			else
			{
				TheDiscJockey.MakeWhiteNoise(FrameTime - ElapsedTime);
			}
			
		}
	}
}

void CoverPage() 
{

}

void IntroductionPage()
{

}

void BridgePage(ScreenManager TheScreenManager)
{
	//设定时间原点
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	double TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
	//读取文件
	std::ifstream inFile("BridgePage.txt");
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
	BridgePage(TheScreenManager);
	Act(TheScreenManager);
	return 0;
}