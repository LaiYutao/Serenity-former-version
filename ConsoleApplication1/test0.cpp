#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <vector>
#include <windows.h>
#define M_PI 3.1415926f

const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 75;
const char PIXEL_EMPTY = ' ';
const char PIXEL_ACTIVE = '#';

struct Point {
    int x;
    int y;
};

// 用于隐藏和显示光标的函数
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE; // 隐藏光标
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void showCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = TRUE; // 显示光标
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// 绘制一个像素
void drawPixel(int x, int y, char symbol) {
    COORD pos = { x * 2, y }; // 加倍x以调整比例
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    std::cout << symbol << symbol; // 使用两个字符模拟正方形
}

// 模拟波传播
void simulateWavePropagation(const Point& source, double speed, double maxRadius) {
    hideCursor(); // 在绘制前隐藏光标

    char screen[SCREEN_HEIGHT][SCREEN_WIDTH];

    // 初始化屏幕
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            screen[y][x] = PIXEL_EMPTY;
        }
    }

    // 在波的半径范围内传播
    for (double r = 0; r <= maxRadius; r += speed) {
        // 通过角度来确定波的坐标
        for (double angle = 0; angle < 2 * M_PI; angle += 0.05) {
            int x = source.x + static_cast<int>(r * cos(angle));
            int y = source.y + static_cast<int>(r * sin(angle));

            if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
                screen[y][x] = PIXEL_ACTIVE; // 将这个点设为活跃
            }
        }

        // 清除屏幕，然后重新绘制所有点
        for (int y = 0; y < SCREEN_HEIGHT; ++y) {
            for (int x = 0; x < SCREEN_WIDTH; ++x) {
                drawPixel(x, y, screen[y][x]); // 绘制屏幕上的所有点
            }
        }

        // 每次更新后等待一下，制造动画效果
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

   //showCursor(); // 在绘制后显示光标
}

int main() {
    // 波的中心点
    Point source = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

    // 速度和最大半径
    double speed = 0.2;
    double maxRadius = 100.0;

    simulateWavePropagation(source, speed, maxRadius); // 开始模拟

    return 0;
}
