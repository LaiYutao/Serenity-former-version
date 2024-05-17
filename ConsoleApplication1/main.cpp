#include"Gardener.h"
#include"Monitor.h"
#include"DiscJockey.h"
#include<fstream>
#include<string>
#include<chrono>
#include<random>

//������loop���ڵ����ɣ�������Ϸ������
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
		//��ȡElapsedTime(��λΪ����)
		tp2 = std::chrono::high_resolution_clock::now();
		auto ElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
		tp1 = tp2;

		//��ȡʱ�����е�ǰʱ��(��λΪ��)
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
				MusicMode = std::thread([&TheDiscJockey, ElapsedTime]() { TheDiscJockey.MakeClusters(FrameTime*0.35); }); 
			}
			else
			{
				//΢�ְ�����ģʽ
				MusicMode = std::thread([&TheDiscJockey, ElapsedTime]() { TheDiscJockey.MakeWhiteNoise(FrameTime*0.35); });
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

		//TheGardener����CompoundHeight
		TheGardener.UpdateCompoundHeight();

		//TheMonitor���ܸ��º��CompoundHeight
		TheMonitor.SetCompoundHeight(TheGardener.getCompoundHeight());

		//TheMonitor��Heightת��Ϊ�ַ�
		TheMonitor.ChangeIntoPixel(TheScreenManager.getRefScreenBuffer());

		//TheMonitor����PlantingPoint����ʾ
		TheMonitor.AddPlantingPoint(TheScreenManager.getRefScreenBuffer(), TheGardener.getPlantingPoint());

		//TheMonitor����״̬������ʾ
		TheMonitor.AddStatusBar(TheScreenManager.getScreenBuffer(),TheScreenManager.getRefScreenShow(),TheGardener.getPlantingPoint(),TheGardener.getNumOfCircularField(),TheGardener.getNumOfSpiralField());

		//TheDiscJockey����CompoundHeight
		TheDiscJockey.CalculateHeightDistribution(TheGardener.getCompoundHeight());
		
		//TheDiscJockey������΢�ְ�����ģʽ�·�������
		TheDiscJockey.CalculateHertz();

		//�ȴ�MusicMode�������
		if (MusicMode.joinable())
		{
			MusicMode.join();
		}
		
		//TheScreenManager���ͼ��
		TheScreenManager.ShowActImage();

		// ����֡�ʣ�
		if (ElapsedTime < FrameTime)//��λΪ����
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(FrameTime - ElapsedTime));
		}
	}
}

//�������ҳǰ�Ĳ�����ʾָ��
void Tips()
{
	using namespace std;
	cout << "\n�������ã���win11����ϵͳ��" << endl;
	cout << "1.��������Ϸ������������ơ�V���ķ��ţ�ƫ���ڡ�+���ұߣ����ٵ�����á�" << endl;
	cout << "2.�ڡ�������ѡ���У����Ҳ�ڶ��С�Ĭ���ն�Ӧ�ó����У�������Ҷ����ơ�V���İ�������ѡ������Ϊ��Windows����̨�����������½Ǵ������ɫ�������桱��" << endl;
	cout << "3.������ɡ���رղ����������˳���" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "��飺    ��!*!�������ϻ��������֣���ҳ�����Ϸ����鿴���������á�������!*!��" << endl;
	cout<< "����PlantingPoint��һ��ʼλ�����룩�����ƶ���ѡ��λ�ã����³�Դ�����ֿ�ѡ��������������������Ƶ�ʡ������������������һ�޶������ɸ��Ƶ��������ۣ�" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "����ָ�ϣ���!*!�������ϻ��������֣���ҳ�����Ϸ����鿴���������á�������!*!��" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "    W/S/A/D\t|����PlantingPoint ����/����/����/����" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "       J\t|����CircularField��Բ�β�����" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "       K\t|����SpiralField�������γ���" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "  ����J+����W\t|���ߴ�λ�õĳ���Ӧ���" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "  ����J+����S\t|���ʹ�λ�õĳ���Ӧ���" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "  ����K+����W\t|���ߴ�λ�õĳ���ӦƵ��" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "  ����K+����S\t|���ʹ�λ�õĳ���ӦƵ�ʣ�Ƶ��Ϊ0��ֹ��Ϊ�������˶���" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "      J+K\t|�л�����ģʽ�����ֿ�ѡ��" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "       M\t|����" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "     ��ʾ1\t|���豸���ܲ�ͬ��������濨�٣����԰�M�������Ի���" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "     ��ʾ2\t|��Ҫ�����������³�Դ�����°����󱣳�һ�£���������������һ��" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "     ��ʾ3\t|������Ϸ������󣨷��漰����ҳ֮�󣩣��·���һ�����������ɹ��鿴" << endl;
	cout << "-------------------------------------------------------------------------------------" << endl;
	cout << "�ڽ���������Ϸ�����У��벻Ҫ���ڴ��ڴ�С����ȫ�����粻С�Ĳ����ˣ���رղ���������" << endl;
	cout << "Have a good time!" << endl;
	system("pause");
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
			TheScreenManager.getRefScreenBuffer()[y * ScreenWidth + x] = Character[i];
			x++;
		}
	}
}

//������CoverPage()�����ڶ�ȡ��ĸ�ַ����ļ�
void ReadFile(std::vector<char>& Character, std::string Directory)
{
	char c;
	std::ifstream inFile(Directory);
	while (inFile.get(c))
	{
		Character.push_back(c);
	}
	inFile.close();
}

//��̬����ҳ
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
	std::vector<char> tips; //��ʾ��

	//��ȡ��ĸ�ļ�����ȡ
	{
		ReadFile(Sl, "txtfile\\character\\S l.txt");
		ReadFile(Sr, "txtfile\\character\\S r.txt");
		ReadFile(el, "txtfile\\character\\e l.txt");
		ReadFile(er, "txtfile\\character\\e r.txt");
		ReadFile(rl, "txtfile\\character\\r l.txt");
		ReadFile(rr, "txtfile\\character\\r r.txt");
		ReadFile(nl, "txtfile\\character\\n l.txt");
		ReadFile(nr, "txtfile\\character\\n r.txt");
		ReadFile(il, "txtfile\\character\\i l.txt");
		ReadFile(ir, "txtfile\\character\\i r.txt");
		ReadFile(tl, "txtfile\\character\\t l.txt");
		ReadFile(tr, "txtfile\\character\\t r.txt");
		ReadFile(yl, "txtfile\\character\\y l.txt");
		ReadFile(yr, "txtfile\\character\\y r.txt");
		ReadFile(tips, "txtfile\\Tips.txt");
	}
	
	bool Out = false;
	bool KeySpacePressed = true;//��ֹ�����Ͱ���SPACE��ֱ�Ӱѷ���������

	//�趨ʱ��ԭ��
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	double TimeOfNow = 0;

	//д�뾲̬BackGround
	TheScreenManager.getRefScreenBuffer() = BackGround;

	//���������ʾ���ǵ㣬�ḻ����
	std::srand(std::time(0));
	for (int i = 0;i < 1000;++i)
	{
		int X = std::rand() % TheScreenManager.getScreenWidth();
		int Y = std::rand() % TheScreenManager.getScreenHeight();

		if (!(((X >= 73) && (X <= 158) && (Y >= 45) && (Y <= 49)) || ((X >= 60) && (X <= 172) && (Y >= 55) && (Y <= 62)))) //�����ڵ�����
		{
			TheScreenManager.getRefScreenBuffer()[Y * TheScreenManager.getScreenWidth() + X] = 'o';
		}
	}

	//"PRESS SPACE TO CONTINUE"�Ƿ���ʾ����ΪҪ����˸����ʾ��
	bool IfShowPressSpace = true;

	//���Page
	while (Out == false)
	{
		//��ȡʱ�����е�ǰʱ��(��λΪ��)
		TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
		
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
		
		//д����ĸ
		if(2*TimeOfNow-int(2*TimeOfNow)<=0.002) //��������ÿ0.5���㹻�����ͱ任һ�Σ�����һ����Ҳ��֤�˿ո���������ȣ�
		{                                       //��ʵ���ϣ�����û�п���֡�ʣ�������ƫ������ڷ���ҳûʲô��ν��������������ԣ�
			//��β��ͷ����y��ʼ��������yβ����������Ƚϴ�
			if (rand() % 2)AddCharacter(TheScreenManager, yl, 183, 29); //y��Ӧ�ó��ֵ���������Ͻ�
			else AddCharacter(TheScreenManager, yr, 183, 29);

			if (rand() % 2)AddCharacter(TheScreenManager, tl, 167, 27); //t��Ӧ�ó��ֵ���������Ͻ�
			else AddCharacter(TheScreenManager, tr, 167, 27);

			if (rand() % 2)AddCharacter(TheScreenManager, il, 153, 25); //i��Ӧ�ó��ֵ���������Ͻ�
			else AddCharacter(TheScreenManager, ir, 153, 25);

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
		
		//д��PRESS SPACE��ʾ��
		if (4 * TimeOfNow - int(4 * TimeOfNow) <= 0.004) //����ÿ0.25����һ�Σ�ʵ���ϣ�����û�п���֡�ʣ�������ƫ������ڷ���ҳûʲô��ν��������������ԣ�
		{
			if (IfShowPressSpace == true) //����У����ÿո񸲸�
			{
				for (int i = 65 * TheScreenManager.getScreenWidth();i < 69 * TheScreenManager.getScreenWidth() + TheScreenManager.getScreenWidth() - 1;++i)//��ʾ������ӣ�0,65������ScreenWidth-1,69��
				{
					TheScreenManager.getRefScreenBuffer()[i] = ' ';
				}
				IfShowPressSpace = false;
			}
			else //���û�У��ʹ�����ʾ��
			{
				AddCharacter(TheScreenManager, tips, 0, 65);
				IfShowPressSpace = true;
			}
		}

		//���ҳ��
		TheScreenManager.ShowPageImage();
	}
}

//��ʼǰ�Ĺ���ҳ
void BridgePage(ScreenManager TheScreenManager)
{
	//�趨ʱ��ԭ��
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	double TimeOfNow = 0;
	//��ȡ�ļ�
	std::ifstream inFile("txtfile\\BridgePage.txt");
	char c;
	while (inFile.get(c))
	{
		TheScreenManager.getRefScreenBuffer().push_back(c);
	}
	inFile.close();

	//�趨��ҳ����ʱ��
	std::uniform_real_distribution<double> dis(0.5, 1.5);//�Ƚ������ʱ�����䣬�������ѡ����Ϊ��ҳ��ʾʱ��
	std::default_random_engine gen;
	double Duration= dis(gen);

	//���page
	while (TimeOfNow < Duration)
	{
		//��ȡʱ�����е�ǰʱ��(��λΪ��)
		TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
		//���ҳ��
		TheScreenManager.ShowPageImage();
	}
}

int main() 
{
	Tips();
	ScreenManager PageManager(ScreenWidth, ScreenHeight / 2, FontWidth, FontHeight * 2);//���ڵ����������Ϸ������һ��
	CoverPage(PageManager);
	BridgePage(PageManager);
	ScreenManager TheScreenManager;
	Act(TheScreenManager);
	return 0;
}