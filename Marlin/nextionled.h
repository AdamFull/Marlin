#ifndef NEXTIONLED_H
#define NEXTIONLED_H

#include "MarlinConfig.h"

#if ENABLED(NEXTION_LED)
    void led_init();
    void led_update();
#else
    void led_init() {}
    void led_update() {}
#endif

#endif