#include"Gardener.h"
#include"Monitor.h"
#include"DiscJockey.h"
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
	DiscJockey TheDiscJockey;

	char* screen = new char[2*ScreenWidth * ScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	//��ѭ��
	while (1) 
	{
		//��ȡElapsedTime
		tp2 = std::chrono::high_resolution_clock::now();
		auto ElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
		tp1 = tp2;

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
			TheField->ActivateMedium(TimeOfNow, double(FrameTime)/1000);
		}
		
		//��ÿ������Ӧ��Medium���ÿ��Medium���񶯣��ı�Height��
		for (auto TheField : TheGardener.getRefCompoundField())
		{
			for (int i = 0;i < ScreenWidth * ScreenHeight;++i) 
			{
				(*TheField->getPtrMediumLayer())[i].Vibrate(TimeOfNow);
			}
		}

		//TheMonitor�����Medium��ĵ���
		TheMonitor.UpdateCompoundHeight(TheGardener.getCompoundMedium());

		//TheMonitor��Heightת��Ϊ�ַ�
		TheMonitor.ChangeIntoPixel(TheScreenManager.getRefScreenBuffer());

		//TheMonitor����PlantingPoint����ʾ
		TheMonitor.AddPlantingPoint(TheScreenManager.getRefScreenBuffer(), TheGardener.getPlantingPoint());

		//TheDiscJockey����CompoundHeight
		TheDiscJockey.CalculateHeightDistribution(TheMonitor.getCompoundHeight());
		
		//����������TheDiscJockey���㷢�����ȣ�*��
		TheDiscJockey.CalculateHertz();
		//��B4�ĺ��ȱȽϣ������С�ģ������������ֹ���豸��*��
		double OutputHertz = (TheDiscJockey.getCalculatedHertz() <= 493.88) ? TheDiscJockey.getCalculatedHertz() : 493.88;

		for (int i = 0;i < 2 * ScreenWidth * ScreenHeight;++i)
		{
			screen[i] = TheScreenManager.getScreenBuffer()[i];
		}

		WriteConsoleOutputCharacterA(hConsole, screen, 2*ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
		TheScreenManager.SetEmptyBuffer();
		
		// ����֡��
		if (ElapsedTime < FrameTime)
		{
			//�����ĳ��ԣ�������������*��
			if (OutputHertz <= 37)//����С��37�Ͳ�������BeepҲ�в�����Χ
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