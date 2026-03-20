#include "FrameBuffer.h"
#include <string.h>

FrameBuffer::FrameBuffer() {
    cursorX = 0;
    cursorY = 0;
    cursorScale = 1;
    marginX = 0;
    fill(0xFF);
}

FrameBuffer::~FrameBuffer() {

}

void FrameBuffer::fill(uint8_t color) { // 0xFF=白, 0x00=黒
    memset(buf, color, sizeof(buf));
}

void FrameBuffer::clear() {
    fill(0xFF);
}

void FrameBuffer::setPixel(int x, int y, bool black) {
    if (x < 0 || x >= EPD_W || y < 0 || y >= EPD_H) return;
    int idx = y * (EPD_W / 8) + x / 8;
    uint8_t bit = 0x80 >> (x % 8);
    if (black)
        buf[idx] &= ~bit; // 0=黒
    else
        buf[idx] |= bit;  // 1=白
}

void FrameBuffer::drawChar(int x, int y, char c, int scale) {
    if (c < 0x20 || c > 0x7E) return;
    const uint8_t* glyph = font5x7[c - 0x20];
    for (int col = 0; col < 5; col++) {
        uint8_t bits = pgm_read_byte(&glyph[col]);
        for (int row = 0; row < 7; row++) {
            bool black = (bits >> row) & 1;
            for (int sx = 0; sx < scale; sx++)
                for (int sy = 0; sy < scale; sy++)
                    setPixel(x + col*scale + sx, y + row*scale + sy, black);
        }
    }
}

// ----- カーソル操作 -----
void FrameBuffer::setCursor(int x, int y) {
    cursorX = x;
    cursorY = y;
    marginX = x; // setCursorしたX位置が改行時の戻り先
}

void FrameBuffer::setScale(int scale) {
    cursorScale = scale;
}

void FrameBuffer::setMargin(int x) {
    marginX = x;
}

void FrameBuffer::advanceCursor(int scale) {
    cursorX += 6 * scale; // 5px + 1px 間隔

    // 画面端で折り返し
    if (cursorX + 5 * scale >= EPD_W) {
        cursorX = marginX;
        cursorY += 8 * scale; // 改行 (7px + 1px間隔)
    }
}

// ----- print/println -----
void FrameBuffer::print(char c) {
    if (c == '\n') {
        cursorX = marginX;
        cursorY += 8 * cursorScale;
        return;
    }
    drawChar(cursorX, cursorY, c, cursorScale);
    advanceCursor(cursorScale);
}

void FrameBuffer::print(const char* str) {
    while (*str) print(*str++);
}

void FrameBuffer::print(const String& str) {
    print(str.c_str());
}

void FrameBuffer::println(const char* str) {
    print(str);
    print('\n');
}

void FrameBuffer::println(const String& str) {
    println(str.c_str());
}

void FrameBuffer::println() {
    print('\n');
}
