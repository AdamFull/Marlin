//Simple display picker

#ifndef DISPLAY_H
#define DISPLAY_H

    #if ENABLED(NEXTION_LCD) && !(ENABLED(ULTRA_LCD) || ENABLED(MALYAN_LCD))
        #include "nextionlcd.h"
    #elif (ENABLED(ULTRA_LCD) || ENABLED(MALYAN_LCD)) && !ENABLED(NEXTION_LCD)
        #include "ultralcd.h"
    #else
        #include "nondisplay.h"
    #endif

#endif