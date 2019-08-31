#ifndef DISPLAY_H
#define DISPLAY_H

#include "inc/MarlinConfig.h"
#include "../Configiration_nextion.h"

#if ENABLED(NEXTION_LCD) && !ENABLED(ULTRA_LCD)
    #include "nextionlcd.h"
#elif ENABLED(ULTRA_LCD) && !ENABLED(NEXTION_LCD)
    #include "lcd/ultralcd.h"
#else

    #include "lcd/ultralcd.h"

#endif
    
#endif