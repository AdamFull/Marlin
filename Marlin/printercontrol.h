#ifndef PRINTERCONTROL_H
#define PRINTERCONTROL_H

#if defined(ARDUINO) && ARDUINO >= 100
	    #include "arduino.h"
#else
	#include "WProgram.h"
#endif

class printercontrol
{
public:

    //Thermal
    void setHotendTemperature(int16_t value, uint8_t index);
    int16_t getHotendTemperature(uint8_t index);
    
    #if HAS_HEATED_BED
        void setBedTemperature(int16_t value); 
        int16_t getBedTemperature(uint8_t index);
    #else
        inline void setBedTemperature(int16_t value) {} 
        inline int16_t getBedTemperature(uint8_t index) { return sizeof(int16_t); }
    #endif

    /*void beginPreheat();
    void endHeat();

    //Hotend fan speed
    void setFanSpeed(byte index, float value);
    float getFanSpeed(byte index);

    //Axis movement
    void moveX(double value);
    void moveY(double value);
    void moveZ(double value);
    double getX();
    double getY();
    double getZ();
    
    void sendGcode(String gcode);
    void startPrint();
    void printPause();*/

private:
    bool isHeating = false;
    bool isPrinting = false;
    bool isPaused = false;

};


#endif