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
				MusicMode = std::thread([&TheDiscJockey, ElapsedTime]() { TheDiscJockey.MakeClusters(0.4*FrameTime); }); 
			}
			else
			{
				//΢�ְ�����ģʽ
				MusicMode = std::thread([&TheDiscJockey, ElapsedTime]() { TheDiscJockey.MakeWhiteNoise(0.4*FrameTime); });
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

//������CoverPage()�����ڸ��Ƕ�̬����ĸ
void AddCharacter(ScreenManager& TheScreenManager,std::vector<char> Character,int X, int Y) //���ڱ�������������CoverPage������һλ�÷������ã�����ʵ������Ϊ�ú��������һ���֣�����βζ����˴�д��ͷ
{
	int x = X;
	int y = Y;

	for (int i = 0;i < Character.size();++i)
	{
		if (Character[i] == '\n')
		{
			y++;
			x = X;
		}
		else
		{
			TheScreenManager.getRefScreenBuffer()[y * ScreenWidth * 2 + x] = Character[i];
			x++;
		}
	}
}

void CoverPage(ScreenManager TheScreenManager)
{
	//��ȡ�����ļ�
	std::ifstream inFile("txtfile\\CoverPage.txt");
	std::vector<char> BackGround;
	char c;
	while (inFile.get(c))
	{
		if (c != '\n') //�������з�
		{
			//���ļ�����д�뾲̬BackGround����
			BackGround.push_back(c);
		}
	}
	inFile.close();

	//��ȡ��ĸ�ļ�������
	std::vector<char> Sl; //���󵹵�S
	std::vector<char> Sr; //���ҵ���S
	std::vector<char> el; //���󵹵�e
	std::vector<char> er; //���ҵ���e
	std::vector<char> rl; //���󵹵�r
	std::vector<char> rr; //���ҵ���r
	std::vector<char> nl; //���󵹵�n
	std::vector<char> nr; //���ҵ���n
	std::vector<char> il; //���󵹵�i
	std::vector<char> ir; //���ҵ���i
	std::vector<char> tl; //���󵹵�t
	std::vector<char> tr; //���ҵ���t
	std::vector<char> yl; //���󵹵�y
	std::vector<char> yr; //���ҵ���y

	//��ȡ��ĸ�ļ�����ȡ
	{
		std::ifstream inFile1("txtfile\\character\\S l.txt");
		char lS;
		while (inFile1.get(lS))
		{
			Sl.push_back(lS);
		}
		inFile1.close();

		std::ifstream inFile2("txtfile\\character\\S r.txt");
		char rS;
		while (inFile2.get(rS))
		{
			Sr.push_back(rS);
		}
		inFile2.close();

		std::ifstream inFile3("txtfile\\character\\e l.txt");
		char le;
		while (inFile3.get(le))
		{
			el.push_back(le);
		}
		inFile3.close();

		std::ifstream inFile4("txtfile\\character\\e r.txt");
		char re;
		while (inFile4.get(re))
		{
			er.push_back(re);
		}
		inFile4.close();

		std::ifstream inFile5("txtfile\\character\\r l.txt");
		char lr;
		while (inFile5.get(lr))
		{
			rl.push_back(lr);
		}
		inFile5.close();

		std::ifstream inFile6("txtfile\\character\\r r.txt");
		char r;
		while (inFile6.get(r))
		{
			rr.push_back(r);
		}
		inFile6.close();

		std::ifstream inFile7("txtfile\\character\\n l.txt");
		char ln;
		while (inFile7.get(ln))
		{
			nl.push_back(ln);
		}
		inFile7.close();

		std::ifstream inFile8("txtfile\\character\\n r.txt");
		char rn;
		while (inFile8.get(rn))
		{
			nr.push_back(rn);
		}
		inFile8.close();

		std::ifstream inFile9("txtfile\\character\\i l.txt");
		char li;
		while (inFile9.get(li))
		{
			il.push_back(li);
		}
		inFile9.close();

		std::ifstream inFilea("txtfile\\character\\i r.txt");
		char ri;
		while (inFilea.get(ri))
		{
			ir.push_back(ri);
		}
		inFilea.close();

		std::ifstream inFileb("txtfile\\character\\t l.txt");
		char lt;
		while (inFileb.get(lt))
		{
			tl.push_back(lt);
		}
		inFileb.close();

		std::ifstream inFilec("txtfile\\character\\t r.txt");
		char rt;
		while (inFilec.get(rt))
		{
			tr.push_back(rt);
		}
		inFilec.close();

		std::ifstream inFiled("txtfile\\character\\y l.txt");
		char ly;
		while (inFiled.get(ly))
		{
			yl.push_back(ly);
		}
		inFiled.close();

		std::ifstream inFilee("txtfile\\character\\y r.txt");
		char ry;
		while (inFilee.get(ry))
		{
			yr.push_back(ry);
		}
		inFilee.close();
	}
	
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
		
		//д�뾲̬BackGround
		TheScreenManager.getRefScreenBuffer() = BackGround;
		
		//д����ĸ
		{
			//��β֮ǰ����y��ʼ
			if (rand() % 2)AddCharacter(TheScreenManager, yl, 185, 29); //y��Ӧ�ó��ֵ���������Ͻ�
			else AddCharacter(TheScreenManager, yr, 185, 29);

			if (rand() % 2)AddCharacter(TheScreenManager, tl, 168, 25); //t��Ӧ�ó��ֵ���������Ͻ�
			else AddCharacter(TheScreenManager, tr, 168, 25);

			if (rand() % 2)AddCharacter(TheScreenManager, il, 154, 23); //i��Ӧ�ó��ֵ���������Ͻ�
			else AddCharacter(TheScreenManager, ir, 154, 23);

			if (rand() % 2)AddCharacter(TheScreenManager, nl, 125, 29); //n��Ӧ�ó��ֵ���������Ͻ�
			else AddCharacter(TheScreenManager, nr, 125, 29);

			if (rand() % 2)AddCharacter(TheScreenManager, el, 102, 29); //e��Ӧ�ó��ֵ���������Ͻ�
			else AddCharacter(TheScreenManager, er, 102, 29);

			if (rand() % 2)AddCharacter(TheScreenManager, rl, 81, 29); //r��Ӧ�ó��ֵ���������Ͻ�
			else AddCharacter(TheScreenManager, rr, 81, 29);
			
			if (rand() % 2)AddCharacter(TheScreenManager, el, 58, 29); //e��Ӧ�ó��ֵ���������Ͻ�
			else AddCharacter(TheScreenManager, er, 58, 29);

			if (rand() % 2)AddCharacter(TheScreenManager, Sl, 30, 22); //S��Ӧ�ó��ֵ���������Ͻ�
			else AddCharacter(TheScreenManager, Sr, 30, 22);
		}
		
		//���ҳ��
		TheScreenManager.ShowImage();
		//std::this_thread::sleep_for(std::chrono::milliseconds(500));  //��Ҫ�÷�����ĸ������һ�㣬��Ҫ�ÿո��Ᵽ֤������
	}
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