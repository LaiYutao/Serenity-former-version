#include"Gardener.h"
#include"Monitor.h"
#include"DiscJockey.h"
#include<fstream>
#include<string>
#include<chrono>
#include<random>

//������loop���ڵ�����
void Act(ScreenManager TheScreenManager)
{
	//�趨ʱ��ԭ��
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	auto tp1 = std::chrono::high_resolution_clock::now();
	auto tp2 = std::chrono::high_resolution_clock::now();

	//��ʼ����������
	Gardener TheGardener;
	Monitor TheMonitor;
	DiscJockey TheDiscJockey;

	//��ѭ��
	while (1) 
	{
		//��ȡElapsedTime
		tp2 = std::chrono::high_resolution_clock::now();
		auto ElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
		tp1 = tp2;

		//��ȡʱ�����е�ǰʱ��
		double TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
		
		//TheDiscJockey����Ƿ��ھ���ģʽ
		TheDiscJockey.DetectIfMute();

		//TheDiscJockey���MusicType�Ƿ�ı�
		TheDiscJockey.DetectMusicTypeChange();

		//������Ǵ��ھ���ģʽ���򴴽���Ӧ����ģʽ���߳�
		std::thread MusicMode;
		if (TheDiscJockey.getIfMute() == false)
		{
			if (TheDiscJockey.getMusicType() == true)
			{
				//ʮ��������ģʽ
				MusicMode = std::thread([&TheDiscJockey, ElapsedTime]() { TheDiscJockey.MakeClusters(0.5*ElapsedTime); }); 
			}
			else
			{
				//΢�ְ�����ģʽ
				MusicMode = std::thread([&TheDiscJockey, ElapsedTime]() { TheDiscJockey.MakeWhiteNoise(0.5*ElapsedTime); });
			}
		}

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
		
		//TheDiscJockey������΢�ְ�����ģʽ�·�������
		TheDiscJockey.CalculateHertz();

		//�ȴ�MusicMode�������
		if (MusicMode.joinable())
		{
			MusicMode.join();
		}
		
		//TheScreenManager���ͼ��
		TheScreenManager.ShowImage();

		// ����֡�ʣ�
		if (ElapsedTime < FrameTime)//��λΪ����
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(FrameTime - ElapsedTime));
		}
	}
}

void CoverPage(ScreenManager TheScreenManager)
{
	//��ȡ�ļ�
	std::ifstream inFile("txtfile\\CoverPage.txt");
	char c;
	while (inFile.get(c))
	{
		if (c != '\n') //�������з�
		{
			TheScreenManager.getRefScreenBuffer().push_back(c);
		}
	}
	inFile.close();

	bool Out = false;
	bool KeySpacePressed = false;

	//���Page
	while (Out == false)
	{
		//����Ƿ��¿ո��
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
	//�趨ʱ��ԭ��
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	double TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
	//��ȡ�ļ�
	std::ifstream inFile("txtfile\\BridgePage.txt");
	char c;
	while (inFile.get(c))
	{
		TheScreenManager.getRefScreenBuffer().push_back(c);
	}
	inFile.close();

	//�趨��ҳ����ʱ��
	std::uniform_real_distribution<double> dis(0.2, 1.5);//�Ƚ������ʱ�����䣬�������ѡ����Ϊ��ҳ��ʾʱ��
	std::default_random_engine gen;
	double Duration= dis(gen);

	//���page
	while (TimeOfNow < Duration)
	{
		//��ȡʱ�����е�ǰʱ��
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