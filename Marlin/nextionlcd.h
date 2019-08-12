#ifndef NEXTIONLCD_H
#define NEXTIONLCD_H

#include "MarlinConfig.h"

#if ENABLED(NEXTION_LCD)
    void lcd_init();
    bool lcd_detected();
    void lcd_update();
    void lcd_setalertstatusPGM(const char* message);
#else
    inline void lcd_init() {}
    inline bool lcd_detected() { return true; }
    inline void lcd_update() {}
    inline void lcd_setalertstatusPGM(const char* message) { UNUSED(message); }
#endif



#endif