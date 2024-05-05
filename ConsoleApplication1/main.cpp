#include"ScreenManager.h"
#include"Monitor.h"
#include"DiscJockey.h"
#include"Gardener.h"
#include"CircularField.h"
#include"SpiralField.h"
#include"Medium.h"
#include<chrono>
#include<thread>

//������loop���ڵ�����
void Act(ScreenManager TheScreenManager)
{
	//�趨ʱ��ԭ��
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	auto tp1 = std::chrono::high_resolution_clock::now();
	auto tp2 = std::chrono::high_resolution_clock::now();

	Gardener TheGardener;
	Monitor TheMonitor;

	char* screen = new char[2*ScreenWidth * ScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	//��ѭ��
	while (1) 
	{
		//��ȡElapsedTime
		tp2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		double ElapsedTime = elapsedTime.count();

		//std::thread ScreenShow(&ScreenManager::ShowPixel,&TheScreenManager);
		//ScreenShow.join();

		//��ȡʱ�����е�ǰʱ��
		double TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
		
		//����PlantingPoint��ѡ��
		TheGardener.SelectPosition();

		//��������ֲ������
		TheGardener.PlantIt(TimeOfNow);

		//����CompoundField�е�ÿһ����,��ÿ������ʼ�ڶ�ӦMedium���ж����ܵ�Medium���м���
		for (auto TheField : TheGardener.getRefCompoundField())
		{
			TheField->ActivateMedium(TimeOfNow, FrameTime);
		}
		
		//��ÿ������Ӧ��Medium���Medium���񶯣��ı�Height��
		for (auto TheField : TheGardener.getRefCompoundField())
		{
			for (int i = 0;i < ScreenWidth * ScreenHeight;++i) 
			{
				TheField->getRefMediumLayer()[i].Vibrate(TimeOfNow);
			}
		}

		//Monitor�����Medium��ĵ���
		TheMonitor.UpdateCompoundHeight(TheGardener.getCompoundMedium());

		//Monitor��Heightת��Ϊ�ַ�
		TheMonitor.ChangeIntoPixel(TheScreenManager.getRefScreenBuffer(),TheMonitor.getCompoundHeight());

		//Monitor����PlantingPoint����ʾ
		TheMonitor.AddPlantingPoint(TheScreenManager.getRefScreenBuffer(), TheGardener.getPlantingPoint());

		for (int i = 0;i < 2 * ScreenWidth * ScreenHeight;++i)
		{
			screen[i] = TheScreenManager.getScreenBuffer()[i];
		}

		WriteConsoleOutputCharacterA(hConsole, screen, 2*ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
		TheScreenManager.SetEmptyBuffer();
		// ����֡��
		if (ElapsedTime < FrameTime)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(FrameTime - int(1000*ElapsedTime)));
		}
	}
}

int main() 
{
	ScreenManager TheScreenManager;
	Act(TheScreenManager);
	return 0;
}