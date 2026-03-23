#include "EPDConsole.h"
#include "FrameBuffer.h"
#include <assert.h>

EPDConsole::EPDConsole(FrameBuffer& fb): fb(fb) {
    setCursor(0, 0);
}

EPDConsole::~EPDConsole() {

}

void EPDConsole::clear() {
    fb.clear();
    setCursor(marginX, 0);
}

void EPDConsole::setCursor(int x, int y) {
    assert(x % CHAR_W == 0);
    assert(y % CHAR_H == 0);

    cursorX = x;
    cursorY = y;
    marginX = x;
}

void EPDConsole::setMargin(int x) {
    marginX = x;
}

void EPDConsole::advanceCursor() {
    cursorX += CHAR_W;

    // はみ出したら改行
    if (cursorX + CHAR_W >= EPD_WIDTH) {
        cursorX = marginX;
        cursorY += CHAR_H;
    }
}

void EPDConsole::print(const char c) {
    if (c == '\n') {
        cursorX = marginX;
        cursorY += CHAR_H;
        return;
    }
    fb.drawChar(cursorX, cursorY, c);
    advanceCursor();
}

void EPDConsole::print(const char* s) {
    while (*s) print(*s++);
}

void EPDConsole::println(const char c) {
    print(c);
    print('\n');
}

void EPDConsole::println(const char* s) {
    print(s);
    print('\n');
}

void EPDConsole::println() {
    print('\n');
}