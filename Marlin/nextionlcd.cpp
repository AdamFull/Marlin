#include "MarlinConfig.h"

#if ENABLED(NEXTION_LCD)

#include "nextionlcd.h"
#include "Marlin.h"
#include "language.h"
#include "cardreader.h"
#include "temperature.h"
#include "planner.h"
#include "stepper.h"
#include "configuration_store.h"
#include "utility.h"
#include "parser.h"

#if HAS_BUZZER && DISABLED(LCD_USE_I2C_BUZZER)
  #include "buzzer.h"
#endif

#include "printcounter.h"

#if ENABLED(PRINTCOUNTER)
  #include "duration_t.h"
#endif

#if ENABLED(BLTOUCH)
  #include "endstops.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "ubl.h"
#elif HAS_ABL
  #include "planner.h"
#elif ENABLED(MESH_BED_LEVELING) && ENABLED(LCD_BED_LEVELING)
  #include "mesh_bed_leveling.h"
#endif

#if ENABLED(FWRETRACT)
  #include "fwretract.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "power_loss_recovery.h"
#endif

#if ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)
  millis_t previous_lcd_status_ms = 0;
#endif

#if ENABLED(LCD_SET_PROGRESS_MANUALLY)
  uint8_t progress_bar_percent;
#endif

void lcd_status_screen();

uint8_t lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW; // Set when the LCD needs to draw, decrements after every draw. Set to 2 in LCD routines so the LCD gets at least 1 full redraw (first redraw is partial)
uint16_t max_display_update_time = 0;

// Initialized by settings.load()
  int16_t lcd_preheat_hotend_temp[2], lcd_preheat_bed_temp[2], lcd_preheat_fan_speed[2];

  #if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(G26_MESH_VALIDATION)
    bool lcd_external_control; // = false
  #endif

  #if ENABLED(BABYSTEPPING)
    long babysteps_done = 0;
    #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
      static void lcd_babystep_zoffset();
    #else
      static void lcd_babystep_z();
    #endif
  #endif

  #if ENABLED(DAC_STEPPER_CURRENT)
    #include "stepper_dac.h" //was dac_mcp4728.h MarlinMain uses stepper dac for the m-codes
    uint8_t driverPercent[XYZE];
  #endif


  #if DISABLED(SLIM_LCD_MENUS)
    static const bool lcd_control_temperature_preheat_material1_settings_menu = true;
    static const bool lcd_control_temperature_preheat_material2_settings_menu = true;
  #else
    static const bool lcd_control_temperature_preheat_material1_settings_menu;
    static const bool lcd_control_temperature_preheat_material2_settings_menu;
  #endif

  #if DISABLED(NO_VOLUMETRICS) || ENABLED(ADVANCED_PAUSE_FEATURE)
    static const bool lcd_control_filament_menu = true;
  #else
    static const bool lcd_control_filament_menu;
  #endif

  #if ENABLED(LCD_INFO_MENU)
    #if ENABLED(PRINTCOUNTER)
      bool lcd_info_stats_menu = true;
    #else
      bool lcd_info_stats_menu;
    #endif
    static const bool lcd_info_thermistors_menu = true;
    static const bool lcd_info_board_menu = true;
    static const bool lcd_info_menu = true;
  #else
    static const bool lcd_info_thermistors_menu;
    static const bool lcd_info_board_menu;
    static const bool lcd_info_menu;
  #endif // LCD_INFO_MENU

  #if ENABLED(LED_CONTROL_MENU)
    #include "leds.h"
    static const bool lcd_led_menu = true;
  #else
    static const bool lcd_led_menu;
  #endif

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    #if E_STEPPERS > 1 || ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      static const bool lcd_change_filament_menu = true;
      static const bool lcd_temp_menu_e0_filament_change;
    #else
      static const bool lcd_change_filament_menu;
      static const bool lcd_temp_menu_e0_filament_change = true;
    #endif
  #endif

  #if ENABLED(DAC_STEPPER_CURRENT) //edit this
    void dac_driver_commit();
    void dac_driver_getValues();
    void lcd_dac_menu();
    void lcd_dac_write_eeprom();
  #endif

  #if ENABLED(FWRETRACT)
    static const bool lcd_control_retract_menu = true;
  #else
    static const bool lcd_control_retract_menu;
  #endif

  #if ENABLED(DELTA_CALIBRATION_MENU) || ENABLED(DELTA_AUTO_CALIBRATION)
    static const bool lcd_delta_calibrate_menu = true;
  #else
    static const bool lcd_delta_calibrate_menu;
  #endif

  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    static float new_z_fade_height;
    void _lcd_set_z_fade_height() { set_z_fade_height(new_z_fade_height); }
  #endif

void lcd_init()
{

}

#endif