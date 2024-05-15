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
		//获取ElapsedTime(单位为毫秒)
		tp2 = std::chrono::high_resolution_clock::now();
		auto ElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
		tp1 = tp2;

		//获取时间轴中当前时间(单位为秒)
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

//进入封面页前的操作提示指引
void Tips()
{
	std::cout << std::endl;
	std::cout << "游戏玩法请参见文件内附Introduction图片及说明文档" << std::endl;
	std::cout << "进入封面页后（一开始是空白的），请用ctrl + 鼠标滚轮（向下滚动）调整画面，必要时可以拉伸侧边控制台窗口" << std::endl;
	std::cout << "调整好画面后，可以欣赏一下动态封面" << std::endl;
	std::cout << "进入游戏主画面后，觉得音乐诡异的话（也可以同时按JK键切换音乐模式），或者后期画面卡顿时，可以按M键静音" << std::endl;
	std::cout << "按任意键进入封面页，即可开始调整画面" << std::endl;
	system("pause");
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

//仅用于CoverPage()，用于读取字母字符画文件
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

//动态封面页
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
	std::vector<char> tips; //提示词

	//读取字母文件：读取
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
	bool KeySpacePressed = true;//防止上来就按着SPACE，直接把封面跳过了

	//设定时间原点
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	double TimeOfNow = 0;

	//写入静态BackGround
	TheScreenManager.getRefScreenBuffer() = BackGround;

	//"PRESS SPACE TO CONTINUE"是否显示；因为要做闪烁的提示词
	bool IfShowPressSpace = true;

	//输出Page
	while (Out == false)
	{
		//获取时间轴中当前时间(单位为秒)
		TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
		
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
		
		//写入字母
		if(2*TimeOfNow-int(2*TimeOfNow)<=0.001) //离每0.5秒足够近，就变换一次；这样一来，也保证了空格检测的灵敏度；
		{
			//从尾到头，从y开始；（由于y尾部横向延伸比较大）
			if (rand() % 2)AddCharacter(TheScreenManager, yl, 183, 29); //y所应该出现的区域的左上角
			else AddCharacter(TheScreenManager, yr, 183, 29);

			if (rand() % 2)AddCharacter(TheScreenManager, tl, 167, 27); //t所应该出现的区域的左上角
			else AddCharacter(TheScreenManager, tr, 167, 27);

			if (rand() % 2)AddCharacter(TheScreenManager, il, 153, 25); //i所应该出现的区域的左上角
			else AddCharacter(TheScreenManager, ir, 153, 25);

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
		
		//写入PRESS SPACE提示词
		if (4 * TimeOfNow - int(4 * TimeOfNow) <= 0.002) //每0.25秒闪一次
		{
			if (IfShowPressSpace == true) //如果有，就用空格覆盖
			{
				for (int i = 65 * ScreenWidth * 2;i < 69 * ScreenWidth * 2 + 2 * ScreenWidth - 1;++i)//提示词区域从（0,65）到（2*ScreenWidth-1,69）
				{
					TheScreenManager.getRefScreenBuffer()[i] = ' ';
				}
				IfShowPressSpace = false;
			}
			else //如果没有，就打上提示词
			{
				AddCharacter(TheScreenManager, tips, 0, 65);
				IfShowPressSpace = true;
			}
		}

		//输出页面
		TheScreenManager.ShowStaticImage();
	}
}

//开始前的过渡页
void BridgePage(ScreenManager TheScreenManager)
{
	//设定时间原点
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	double TimeOfNow = 0;
	//读取文件
	std::ifstream inFile("txtfile\\BridgePage.txt");
	char c;
	while (inFile.get(c))
	{
		TheScreenManager.getRefScreenBuffer().push_back(c);
	}
	inFile.close();

	//设定此页持续时间
	std::uniform_real_distribution<double> dis(0.5, 1.5);//比较随意的时间区间，从中随机选择，作为此页显示时间
	std::default_random_engine gen;
	double Duration= dis(gen);

	//输出page
	while (TimeOfNow < Duration)
	{
		//获取时间轴中当前时间(单位为秒)
		TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
		TheScreenManager.ShowStaticImage();
	}
	TheScreenManager.SetEmptyBuffer();
}

int main() 
{
	Tips();
	ScreenManager TheScreenManager;
	CoverPage(TheScreenManager);
	BridgePage(TheScreenManager);
	Act(TheScreenManager);
	return 0;
}