#include"Gardener.h"
#include"Monitor.h"
#include"DiscJockey.h"
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
	DiscJockey TheDiscJockey;

	char* screen = new char[2*ScreenWidth * ScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	//主循环
	while (1) 
	{
		//获取ElapsedTime
		tp2 = std::chrono::high_resolution_clock::now();
		auto ElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
		tp1 = tp2;

		//std::thread ScreenShow(&ScreenManager::ShowPixel,&TheScreenManager);
		//ScreenShow.join();

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

		//TheDiscJockey接收CompoundHeight
		TheDiscJockey.CalculateHeightDistribution(TheMonitor.getCompoundHeight());
		
		//待定！！！TheDiscJockey计算发出赫兹（*）
		TheDiscJockey.CalculateHertz();
		//与B4的赫兹比较，输出更小的，保险起见，防止损坏设备（*）
		double OutputHertz = (TheDiscJockey.getCalculatedHertz() <= 493.88) ? TheDiscJockey.getCalculatedHertz() : 493.88;

		for (int i = 0;i < 2 * ScreenWidth * ScreenHeight;++i)
		{
			screen[i] = TheScreenManager.getScreenBuffer()[i];
		}

		WriteConsoleOutputCharacterA(hConsole, screen, 2*ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
		TheScreenManager.SetEmptyBuffer();
		
		// 控制帧率
		if (ElapsedTime < FrameTime)
		{
			//发声的尝试，待定！！！（*）
			if (OutputHertz <= 37)//赫兹小于37就不发声，Beep也有参数范围
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(FrameTime - ElapsedTime));
			}
			else
			{
				Beep(OutputHertz, FrameTime - ElapsedTime);
			}
			
		}
	}
}

int main() 
{
	ScreenManager TheScreenManager;
	Act(TheScreenManager);
	return 0;
}