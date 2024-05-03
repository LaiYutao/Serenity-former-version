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
void Act(ScreenManger screenManager)
{
	const int kFrameTime = 16;// 每帧时长为16毫秒，帧率为60fps
	//设定时间原点
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	auto tp1 = std::chrono::high_resolution_clock::now();
	auto tp2 = std::chrono::high_resolution_clock::now();

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
	
	}
}
