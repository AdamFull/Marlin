#include "printercontrol.h"
#include "temperature.h"
#include "MarlinConfig.h"
#include "Marlin.h"

void printercontrol::setHotendTemperature(int temperature, int id)
{
    thermalManager.setTargetHotend(temperature, id);
}

#if HAS_HEATED_BED
void printercontrol::setBedTemperature(int temperature)
{
    thermalManager.setTargetBed(temperature);
}
#endif

void printercontrol::setFanSpeed(char *subbuff)
{
    for(unsigned i = 0; i <= FAN_COUNT; i++)
	{
        fanSpeeds[i] = map(atoi(subbuff), 0, 100, 0, 255);
    }
}

float printercontrol::getCurrentPosition(unsigned axis)
{
    return current_position[axis];
}

#if HAS_HEATED_BED
int16_t printercontrol::getDegTargetBed()
{
    return thermalManager.degTargetBed();
}

float printercontrol::getDegBed()
{
    return thermalManager.degBed();
}
#endif

int16_t printercontrol::getDegTargetHotend(uint8_t id)
{
    return thermalManager.degTargetHotend(id);
}

float printercontrol::getDegHotend(uint8_t id)
{
    if(id <= HOTENDS)
        return thermalManager.degHotend(id);
    else
        return 0.0f;
}

int printercontrol::getFanSpeed(int id)
{
    return fanSpeeds[id];
}