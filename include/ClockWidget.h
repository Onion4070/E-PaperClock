#pragma once
#include "FrameBuffer.h"
#include "images.h"

class ClockWidget {
public:
    ClockWidget(FrameBuffer& fb);
    ~ClockWidget();

    void update(struct tm* timeinfo);
    
private:
    FrameBuffer& fb;
    bool is_first_update = true;
    int last_hour = -1;
    int last_min = -1;
    int partial_count = 0;
    const int FAST_REFRESH_THRESHOLD = 15;      // 15回に1回 FastRefresh

    const int spacing = 16; // 文字間の隙間
    const int x = 16;       // 書き出し位置
    const int y = 0;

    int h1_offset = x;
    int h2_offset = h1_offset + FONT_WIDTH + spacing;
    int colon_offset = h2_offset + FONT_WIDTH + spacing;
    int m1_offset = colon_offset + COLON_WIDTH + spacing;
    int m2_offset = m1_offset + FONT_WIDTH + spacing;
};