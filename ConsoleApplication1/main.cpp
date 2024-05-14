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
		
		//TheDiscJockey检测是否处于静音模式
		TheDiscJockey.DetectIfMute();

		//TheDiscJockey检测MusicType是否改变
		TheDiscJockey.DetectMusicTypeChange();

		//如果不是处于静音模式，则创建对应音乐模式的线程
		std::thread MusicMode;
		if (TheDiscJockey.getIfMute() == false)
		{
			if (TheDiscJockey.getMusicType() == true)
			{
				//十二音音簇模式
				MusicMode = std::thread([&TheDiscJockey, ElapsedTime]() { TheDiscJockey.MakeClusters(0.4*FrameTime); }); 
			}
			else
			{
				//微分白噪音模式
				MusicMode = std::thread([&TheDiscJockey, ElapsedTime]() { TheDiscJockey.MakeWhiteNoise(0.4*FrameTime); });
			}
		}

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
		
		//TheDiscJockey计算在微分白噪音模式下发出赫兹
		TheDiscJockey.CalculateHertz();

		//等待MusicMode进程完成
		if (MusicMode.joinable())
		{
			MusicMode.join();
		}
		
		//TheScreenManager输出图像
		TheScreenManager.ShowImage();

		// 控制帧率；
		if (ElapsedTime < FrameTime)//单位为毫秒
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(FrameTime - ElapsedTime));
		}
	}
}

//仅用于CoverPage()，用于覆盖动态的字母
void AddCharacter(ScreenManager& TheScreenManager,std::vector<char> Character,int X, int Y) //由于本函数仅在下面CoverPage函数单一位置反复调用；故其实可以视为该函数本身的一部分，因此形参都用了大写开头
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
	//读取背景文件
	std::ifstream inFile("txtfile\\CoverPage.txt");
	std::vector<char> BackGround;
	char c;
	while (inFile.get(c))
	{
		if (c != '\n') //消除换行符
		{
			//将文件内容写入静态BackGround备用
			BackGround.push_back(c);
		}
	}
	inFile.close();

	//读取字母文件：声明
	std::vector<char> Sl; //向左倒的S
	std::vector<char> Sr; //向右倒的S
	std::vector<char> el; //向左倒的e
	std::vector<char> er; //向右倒的e
	std::vector<char> rl; //向左倒的r
	std::vector<char> rr; //向右倒的r
	std::vector<char> nl; //向左倒的n
	std::vector<char> nr; //向右倒的n
	std::vector<char> il; //向左倒的i
	std::vector<char> ir; //向右倒的i
	std::vector<char> tl; //向左倒的t
	std::vector<char> tr; //向右倒的t
	std::vector<char> yl; //向左倒的y
	std::vector<char> yr; //向右倒的y

	//读取字母文件：读取
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

	//输出Page
	while (Out == false)
	{
		//检查是否按下空格键
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
		
		//写入静态BackGround
		TheScreenManager.getRefScreenBuffer() = BackGround;
		
		//写入字母
		{
			//从尾之前，从y开始
			if (rand() % 2)AddCharacter(TheScreenManager, yl, 185, 29); //y所应该出现的区域的左上角
			else AddCharacter(TheScreenManager, yr, 185, 29);

			if (rand() % 2)AddCharacter(TheScreenManager, tl, 168, 25); //t所应该出现的区域的左上角
			else AddCharacter(TheScreenManager, tr, 168, 25);

			if (rand() % 2)AddCharacter(TheScreenManager, il, 154, 23); //i所应该出现的区域的左上角
			else AddCharacter(TheScreenManager, ir, 154, 23);

			if (rand() % 2)AddCharacter(TheScreenManager, nl, 125, 29); //n所应该出现的区域的左上角
			else AddCharacter(TheScreenManager, nr, 125, 29);

			if (rand() % 2)AddCharacter(TheScreenManager, el, 102, 29); //e所应该出现的区域的左上角
			else AddCharacter(TheScreenManager, er, 102, 29);

			if (rand() % 2)AddCharacter(TheScreenManager, rl, 81, 29); //r所应该出现的区域的左上角
			else AddCharacter(TheScreenManager, rr, 81, 29);
			
			if (rand() % 2)AddCharacter(TheScreenManager, el, 58, 29); //e所应该出现的区域的左上角
			else AddCharacter(TheScreenManager, er, 58, 29);

			if (rand() % 2)AddCharacter(TheScreenManager, Sl, 30, 22); //S所应该出现的区域的左上角
			else AddCharacter(TheScreenManager, Sr, 30, 22);
		}
		
		//输出页面
		TheScreenManager.ShowImage();
		//std::this_thread::sleep_for(std::chrono::milliseconds(500));  //又要让封面字母动得慢一点，又要让空格检测保证灵敏度
	}
}

void BridgePage(ScreenManager TheScreenManager)
{
	//设定时间原点
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	double TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
	//读取文件
	std::ifstream inFile("txtfile\\BridgePage.txt");
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
	CoverPage(TheScreenManager);
	BridgePage(TheScreenManager);
	Act(TheScreenManager);
	return 0;
}