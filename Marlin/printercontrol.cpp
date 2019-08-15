#include "printercontrol.h"
#include "Marlin.h"
#include "temperature.h"
#include "planner.h"
#include "cardreader.h"
#include "stepper.h"
#include "configuration_store.h"
#include "utility.h"
#include "parser.h"

void printercontrol::setHotendTemperature(int16_t value, uint8_t index)
{
    thermalManager.setTargetHotend(value, index);
}

int16_t printercontrol::getHotendTemperature(uint8_t index)
{
    return thermalManager.degTargetHotend(index);
}

#if HAS_HEATED_BED
void printercontrol::setBedTemperature(int16_t value)
{
    thermalManager.setTargetBed(value);
}

int16_t printercontrol::getBedTemperature(uint8_t index)
{
    return thermalManager.degTargetBed(index);
}
#endif