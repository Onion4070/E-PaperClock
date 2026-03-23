#pragma once
#include "FrameBuffer.h"

class ClockWidget {
public:
    ClockWidget(FrameBuffer& fb);
    ~ClockWidget();

    void update(struct tm* timeinfo);
    
private:
    FrameBuffer& fb;
    bool is_first_update = true;
};