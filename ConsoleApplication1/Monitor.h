#ifndef MONITOR_H
#define MONITOR_H //用于管理每个Medium的显示字符，传到一个字符串中储存，后续再到buffer？
#include"Medium.h"                 //目前和ScreenManager的关系还不明确
class Monitor {
public:
    void ShowPixel(double mediumHeight);//接受每个Medium的Height,然后映射到对应字符。
};
#endif // MONITOR_H