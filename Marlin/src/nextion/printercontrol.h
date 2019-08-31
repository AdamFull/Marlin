#pragma once

#include "Arduino.h"
#include "../inc/MarlinConfig.h"

class printercontrol
{
public:
    static void setHotendTemperature(int temperature, int id);
    #if HAS_HEATED_BED
        static void setBedTemperature(int temperature);
    #else
        inline static void setBedTemperature(int temperature) {}
    #endif
    static void setFanSpeed(char *subbuff);

    static float getCurrentPosition(unsigned axis);
    #if HAS_HEATED_BED
        static int16_t getDegTargetBed();
        static float getDegBed();
    #else
        inline static int16_t getDegTargetBed() { return 0;}
        inline static float getDegBed() { return 0; }
    #endif
    static int16_t getDegTargetHotend(uint8_t id = 1);
    static float getDegHotend(uint8_t id = 1);
    static int getFanSpeed(int id = 0);
private:
    printercontrol() {}
};