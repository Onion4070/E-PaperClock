#include "ClockWidget.h"
#include <Display_EPD_W21.h>
#include <time.h>

ClockWidget::ClockWidget(FrameBuffer& fb) : fb(fb) {
    is_first_update = true;
}

ClockWidget::~ClockWidget() {

}

// 時計を更新して描画
// EPD_SetRAMValue_BaseMap()を予め実行すること
void ClockWidget::update(struct tm* timeinfo) {
    if (timeinfo == nullptr) return;

    bool hour_changed = (timeinfo->tm_hour != last_hour);
    bool min_changed  = (timeinfo->tm_min  != last_min);

    last_hour = timeinfo->tm_hour;
    last_min = timeinfo->tm_min;

    if (!hour_changed && !min_changed) return;

    fb.clear(0, 0, EPD_WIDTH, FONT_HEIGHT);

    // 23:45 -> h1 = 2, h2 = 3, m1 = 4, m2 = 5
    int h1 = timeinfo->tm_hour / 10;
    int h2 = timeinfo->tm_hour % 10;
    int m1 = timeinfo->tm_min / 10;
    int m2 = timeinfo->tm_min % 10;

    // hour
    fb.blitImage(h1_offset, y, FONT_WIDTH, FONT_HEIGHT, font[h1].data);
    fb.blitImage(h2_offset, y, FONT_WIDTH, FONT_HEIGHT, font[h2].data);

    // colon
    fb.blitImage(colon_offset, y, COLON_WIDTH, COLON_HIGHT, font[10].data);

    // minute
    fb.blitImage(m1_offset, y, FONT_WIDTH, FONT_HEIGHT, font[m1].data);
    fb.blitImage(m2_offset, y, FONT_WIDTH, FONT_HEIGHT, font[m2].data);

    // Fast Refreshでゴースト抑制
    if (partial_count++ >= FAST_REFRESH_THRESHOLD) {
        EPD_Init_Fast();
        EPD_Display(fb.buf);
        EPD_DeepSleep();
        partial_count = 0;
        return;
    }

    unsigned char mode = is_first_update ? 0 : 1;

    EPD_Init_Part();
    EPD_Dis_Part(0, 0, fb.buf, FONT_HEIGHT, EPD_WIDTH, mode);
    EPD_DeepSleep();

    is_first_update = false;
}