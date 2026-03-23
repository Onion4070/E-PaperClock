#pragma once
#include <Display_EPD_W21.h>
#include "FrameBuffer.h"

class EPDConsole {
public:
    EPDConsole(FrameBuffer& fb);
    ~EPDConsole();

    void clear();

    void setCursor(int x, int y);
    void setMargin(int x);

    // ----- 文字列出力 -----
    void print(const char c);
    void print(const char* str);
    void println(const char c);
    void println(const char* str);
    void println();
    
private:
    FrameBuffer& fb;
    static constexpr int CHAR_W = 6, CHAR_H = 8;
    static constexpr int COLS = EPD_WIDTH  / CHAR_W;
    static constexpr int ROWS = EPD_HEIGHT / CHAR_H;
    static constexpr int BYTES_PER_ROW = EPD_WIDTH / CHAR_H;
    static constexpr int LINE_BYTES = CHAR_H * BYTES_PER_ROW;

    int cursorX, cursorY;
    int marginX;

    void advanceCursor();
};