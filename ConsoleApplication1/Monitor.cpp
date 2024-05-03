#include"Monitor.h"

Monitor::Monitor()
{
	for (int i = 0;i < ScreenWidth * ScreenHeight;++i)
	{
		HeightLayer[i] = 0;
	}
}

