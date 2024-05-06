#include"ScreenManager.h"
#include"Monitor.h"
#include"Gardener.h"
#include"DiscJockey.h"
#include"CircularField.h"
#include"SpiralField.h"
#include"Medium.h"
#include<chrono>
#include<thread>

//������loop���ڵ�����
void ActTest01(ScreenManager TheScreenManager)
{
	//�趨ʱ��ԭ��
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	auto tp1 = std::chrono::high_resolution_clock::now();
	auto tp2 = std::chrono::high_resolution_clock::now();

	Gardener TheGardener;
	Monitor TheMonitor;

	char* screen = new char[2 * ScreenWidth * ScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	//������
	CircularField CF(0, Point(ScreenWidth / 2, ScreenHeight/2), 2, 0.25, PI / 2, 3);
	std::vector<std::vector<Medium>> compoundMedium;
	compoundMedium.push_back(CF.getRefMediumLayer());
	
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

		TheGardener.SelectPosition();

		//����
		CF.ActivateMedium(TimeOfNow, FrameTime);

		//�񶯣��ı�Height��
		for(int i=0;i<ScreenWidth*ScreenHeight;++i)
		{
			CF.getRefMediumLayer()[i].Vibrate(TimeOfNow);
		}

		//Monitor�����Medium��ĵ���
		TheMonitor.UpdateCompoundHeight(compoundMedium);

		//Monitor��Heightת��Ϊ�ַ�
		TheMonitor.ChangeIntoPixel(TheScreenManager.getRefScreenBuffer(), TheMonitor.getCompoundHeight());

		//Monitor����PlantingPoint����ʾ
		TheMonitor.AddPlantingPoint(TheScreenManager.getRefScreenBuffer(), TheGardener.getPlantingPoint());

		for (int i = 0;i < 2 * ScreenWidth * ScreenHeight;++i)
		{
			screen[i] = TheScreenManager.getScreenBuffer()[i];
		}

		WriteConsoleOutputCharacterA(hConsole, screen, 2 * ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
		TheScreenManager.SetEmptyBuffer();
		// ����֡��
		if (ElapsedTime < FrameTime)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(FrameTime - int(1000 * ElapsedTime)));
		}
	}
}

//int main()
//{
//	ScreenManager TheScreenManager;
//	ActTest01(TheScreenManager);
//	return 0;
//}