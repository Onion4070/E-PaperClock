#include "ClockWidget.h"
#include "images.h"
#include <Display_EPD_W21.h>

ClockWidget::ClockWidget(FrameBuffer& fb) : fb(fb) {
    is_first_update = true;
}

ClockWidget::~ClockWidget() {

}

void ClockWidget::update(struct tm* timeinfo) {
    if (timeinfo == nullptr) return;

    Serial.println("Clear buffer");
    fb.clear(0, 0, EPD_W, FONT_HEIGHT);

    // 23:45 -> h1 = 2, h2 = 3, m1 = 4, m2 = 5
    int h1 = timeinfo->tm_hour / 10;
    int h2 = timeinfo->tm_hour % 10;
    int m1 = timeinfo->tm_min / 10;
    int m2 = timeinfo->tm_min % 10;

    int spacing = 16; // 文字間の隙間
    int x = 16;       // 書き出し位置
    int y = 0;

    // EPD_SetRAMValue_BaseMap()を予め実行すること
    EPD_Init_Part(); 
    
    Serial.println("Writing to buffer");
    // hour
    int h1_offset = x;
    int h2_offset = h1_offset + FONT_WIDTH + spacing;
    
    fb.blitImage(h1_offset, y, FONT_WIDTH, FONT_HEIGHT, font[h1].data);
    fb.blitImage(h2_offset, y, FONT_WIDTH, FONT_HEIGHT, font[h2].data);

    // colon
    int colon_offset = h2_offset + FONT_WIDTH + spacing;
    fb.blitImage(colon_offset, y, COLON_WIDTH, COLON_HIGHT, font[10].data);
    
    // minute
    int m1_offset = colon_offset + COLON_WIDTH + spacing;
    int m2_offset = m1_offset + FONT_WIDTH + spacing;
    fb.blitImage(m1_offset, y, FONT_WIDTH, FONT_HEIGHT, font[m1].data);
    fb.blitImage(m2_offset, y, FONT_WIDTH, FONT_HEIGHT, font[m2].data);
    
    Serial.println("Writing to EPD");
    unsigned char mode = is_first_update ? 0 : 1;
    EPD_Dis_Part(0, 0, fb.buf, FONT_HEIGHT, EPD_WIDTH, mode);
    EPD_DeepSleep();
    
    is_first_update = false;
}