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
void Act(ScreenManger screenManager)
{
	const int kFrameTime = 16;// ÿ֡ʱ��Ϊ16���룬֡��Ϊ60fps
	//�趨ʱ��ԭ��
	double TimeOrigin = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	auto tp1 = std::chrono::high_resolution_clock::now();
	auto tp2 = std::chrono::high_resolution_clock::now();

	//��ѭ��
	while (1) 
	{
		//��ȡElapsedTime
		tp2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		double ElapsedTime = elapsedTime.count();

		//��ȡʱ�����е�ǰʱ��
		double TimeOfNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - TimeOrigin;
	
	}
}
