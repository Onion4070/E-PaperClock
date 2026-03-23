#include "FrameBuffer.h"

#include <string.h>

FrameBuffer::FrameBuffer() {
    cursorX = 0;
    cursorY = 0;
    marginX = 0;
    clear();
}

FrameBuffer::~FrameBuffer() {}

void FrameBuffer::fill(uint8_t color) {  // 0xFF=白, 0x00=黒
    memset(buf, color, sizeof(buf));
}

void FrameBuffer::clear() { fill(0xFF); }

void FrameBuffer::setPixel(int x, int y, bool black) {
    if (x < 0 || x >= EPD_W || y < 0 || y >= EPD_H) return;
    int idx = y * (EPD_W / 8) + x / 8;
    uint8_t bit = 0x80 >> (x % 8);
    if (black)
        buf[idx] &= ~bit;  // 0=黒
    else
        buf[idx] |= bit;  // 1=白
}

void FrameBuffer::drawChar(int x, int y, char c) {
    if (c < 0x20 || c > 0x7E) return;
    const uint8_t* glyph = font5x7[c - 0x20];
    for (int col = 0; col < 5; col++) {
        uint8_t bits = pgm_read_byte(&glyph[col]);
        for (int row = 0; row < 7; row++) {
            bool black = (bits >> row) & 1;
            setPixel(x + col, y + row, black);
        }
    }
}

void FrameBuffer::drawString(int x, int y, const char* str, int scale) {
    int cursorX = x;
    int cursorY = y;
    while (*str) {
        if (*str == '\n') {
            cursorX = x; // 改行時に元のX位置に戻る
            cursorY += 8; // 改行 (7px + 1px間隔)
        } else {
            drawChar(cursorX, cursorY, *str);
            cursorX += 6; // 5px + 1px 間隔
        }
        str++;
    }
}

void FrameBuffer::drawLine1(int x0, int y0, int x1, int y1) {
    // Bresenham's line algorithm
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (x0 <= x1 && y0 <= y1) {
        setPixel(x0, y0, true);
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void FrameBuffer::drawLine(int x0, int y0, int x1, int y1, int thickness) {
    // bresenham's line algorithm
    if (thickness <= 1) {
        drawLine1(x0, y0, x1, y1);
        return;
    }

    float dx = x1 - x0;
    float dy = y1 - y0;
    float length = sqrtf(dx * dx + dy * dy);
    if (length == 0) return;

    float px = (-dy / length) * (thickness / 2.0f);
    float py = (dx / length) * (thickness / 2.0f);

    Point points[4] = {
        {int(x0 + px), int(y0 + py)},
        {int(x0 - px), int(y0 - py)},
        {int(x1 - px), int(y1 - py)},
        {int(x1 + px), int(y1 + py)},
    };
    drawFilledPolygon(points, 4, true);
}

void FrameBuffer::drawFilledPolygon(Point points[], int numPoints, bool black) {
    // 頂点数: 最大255
    // Scanline fill algorithm
    const int MAX_POINTS = 255;

    if (numPoints < 3) return; // 三角形未満は描画しない
    if (numPoints > MAX_POINTS) return; // 配列サイズ制限

    // 最大・最小Y座標を求める
    int minY = points[0].y;
    int maxY = points[0].y;
    for (int i = 1; i < numPoints; i++) {
        if (points[i].y < minY) minY = points[i].y;
        if (points[i].y > maxY) maxY = points[i].y;
    }

    for (int y = minY; y <= maxY; y++) {
        int intersections[MAX_POINTS]; // 十分なサイズの配列を用意
        int count = 0;
        for (int i = 0; i < numPoints; i++) {
            int x0 = points[i].x;
            int y0 = points[i].y;
            int x1 = points[(i + 1) % numPoints].x;
            int y1 = points[(i + 1) % numPoints].y;

            if ((y0 <= y && y < y1) || (y1 <= y && y < y0)) {
                int x = x0 + (y - y0) * (x1 - x0) / (y1 - y0);
                intersections[count++] = x;
            }
        }
        std::sort(intersections, intersections + count);
        for (int i = 0; i < count; i += 2) {
            for (int x = intersections[i]; x <= intersections[i + 1]; x++) {
                setPixel(x, y, black);
            }
        }
    }
}

void FrameBuffer::drawRect(int x, int y, int w, int h) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            setPixel(x + i, y + j, true);
        }
    }
}

void FrameBuffer::blitImage(int x, int y, int w, int h, const uint8_t* img, int scale) {
    int bytesPerRow = (w + 7) / 8;
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            int byteIndex = row * bytesPerRow + col / 8;
            int bitIndex  = 7 - (col % 8);
            bool black    = !((img[byteIndex] >> bitIndex) & 1);

            for (int sy = 0; sy < scale; sy++)
                for (int sx = 0; sx < scale; sx++)
                    setPixel(x + col*scale + sx, y + row*scale + sy, black);
        }
    }
}