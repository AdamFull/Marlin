#ifndef PRINTERCONTROL_H
#define PRINTERCONTROL_H

#include "Arduino.h"

class printercontrol
{
public:
    static void setHotendTemperature(int temperature, int id);
    static void setBedTemperature(int temperature);
    static void setFanSpeed(char *subbuff);

    static float getCurrentPosition(unsigned axis);
    static int16_t getDegTargetBed();
    static float getDegBed();
    static int16_t getDegTargetHotend(uint8_t id = 1);
    static float getDegHotend(uint8_t id = 1);
    static int getFanSpeed(int id = 0);
private:
    printercontrol() {}
};

#endif