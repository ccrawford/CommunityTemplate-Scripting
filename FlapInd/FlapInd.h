#pragma once

#include "Arduino.h"

class FlapInd
{
public:
    FlapInd();
    void begin();
    void attach();
    void detach();
    void set(int16_t messageID, char *setPoint);
    void update();
    void draw();
    
private:
    bool    _initialised;
    float   _flapHandlePct = 0.20;
    float   _flapIndicatorPct = 0.40;
};