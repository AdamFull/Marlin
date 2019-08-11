#ifndef DISPLAY_H
#define DISPLAY_H

inline void lcd_init() {}
inline bool lcd_detected() { return true; }
inline void lcd_update() {}
inline void lcd_setalertstatusPGM(const char* message) { UNUSED(message); }



#endif