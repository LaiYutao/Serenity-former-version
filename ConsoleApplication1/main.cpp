#include"ScreenManager.h"
#include"Monitor.h"
#include"DiscJockey.h"
#include"Gardener.h"
#include"CircularField.h"
#include"SpiralField.h"
#include"Medium.h"
#include<chrono>
#include<thread>

//包含主loop在内的主干
void Act(ScreenManager TheScreenManager)
{
	//设定时间原点
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	auto tp1 = std::chrono::high_resolution_clock::now();
	auto tp2 = std::chrono::high_resolution_clock::now();

	Gardener TheGardener;
	Monitor TheMonitor;
	//主循环
	while (1) 
	{
		//获取ElapsedTime
		tp2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		double ElapsedTime = elapsedTime.count();

		//获取时间轴中当前时间
		double TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
		
		TheGardener.SelectPosition();
		TheGardener.PlantIt(TimeOfNow);

		//遍历CompoundField中的每一个场,让每个场开始在对应Medium层中对四周的Medium进行激活
		for (auto TheField : TheGardener.getRefCompoundField())
		{
			TheField->ActivateMedium(TimeOfNow, FrameTime);
		}
		
		//让每个场对应的Medium层的Medium都振动（改变Height）
		for (auto TheField : TheGardener.getRefCompoundField())
		{
			for (int i = 0;i < ScreenWidth * ScreenHeight;++i) 
			{
				TheField->getRefMediumLayer()[i].Vibrate(TimeOfNow);
			}
		}

		//Monitor处理各Medium层的叠加
		TheMonitor.UpdateCompoundHeight(TheGardener.getCompoundMedium());

		//Monitor将Height转化为字符
		TheMonitor.ChangeIntoPixel(TheScreenManager.getRefScreenBuffer(),TheMonitor.getCompoundHeight());

		//Monitor加上PlantingPoint的显示
		TheMonitor.AddPlantingPoint(TheScreenManager.getRefScreenBuffer(), TheGardener.getPlantingPoint());

		TheScreenManager.ShowPixel();

		// 控制帧率
		if (ElapsedTime < FrameTime)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(FrameTime - int(ElapsedTime)));
		}
	}
}

int main() 
{
	ScreenManager TheScreenManager;
	Act(TheScreenManager);
	return 0;
}