#include "../inc/MarlinConfig.h"
#include "../../Configiration_nextion.h"
#include "../module/temperature.h"

#if ENABLED(NEXTION_LCD)

	uint16_t files_count;
    char files_list[64][27];
    uint16_t files_less;
	bool sd_readed;
	bool sd_need_update;

	unsigned long last_time;

	bool readed = false;

	#if ENABLED(SDSUPPORT)
  		#include "../sd/cardreader.h"
  		//#include "SdFatConfig.h"
	#else
  		#define LONG_FILENAME_LENGTH 0
	#endif

    #include "nextionlcd.h"
	//NextionUI ui;
    #include "nextionlcdelements.h"
	#include "printercontrol.h"
	
	#include "../module/planner.h"
	#include "../libs/duration_t.h"
	#include "../module/printcounter.h"
    #include "command_queue.h"
	#include "../module/configuration_store.h"

	#include "../MarlinCore.h"

	#if ENABLED(NEOPIXEL_LED)
		#include "../feature/leds/leds.h"
	#endif

    #if ENABLED(NEXTION_TIME)
        #include "TimeLib.h"
    #endif

    const byte MAX_MESSAGE_LENGTH = 255;
    char lcd_status_message[MAX_MESSAGE_LENGTH + 1];
    uint8_t lcd_status_update_delay = 1, // First update one loop delayed
        lcd_status_message_level;

    command_queue inputQueue;
    nextionlcdelements dispfe;
    char buffer[32];
    bool strEnd = false, strStart = true;

    #define LCD_UPDATE_INTERVAL  200

    millis_t next_lcd_update_ms;
	millis_t sd_update_ms;

	int led_on = 0;

    void NextionUI::init() 
    {
		#if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
    		SET_INPUT_PULLUP(SD_DETECT_PIN);
  		#endif
		
        bool initState = nexInit();
        dispfe.setInitStatus(initState);
		#if ENABLED(NEXTION_DEBUG)
        	SERIAL_ECHO_START();
			SERIAL_ECHOLNPAIR("NEXTION INITIALISE IS ", initState ? "TRUE" : "FALSE");
			SERIAL_EOL();
		#endif
		
        update();
        dispfe.setStarted();
    }

	LCDViewAction NextionUI::lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW;

    void NextionUI::update() 
    {
        if(!inputQueue.isEmpty()) processBuffer(inputQueue.pop());
        SerialEvent();
        millis_t ms = millis();

	    if (ELAPSED(ms, next_lcd_update_ms))
        {
            char temp[10];

			if(dispfe.getPage() == main_page || dispfe.getPage() == printing_page)
			{
		    	dispfe.setExtruderActual(); //Set current extruder temperature
		    	dispfe.setExtruderTarget(); //Set extruder target temperature

				#if HAS_HEATED_BED
		    		dispfe.setBedActual();
		    		dispfe.setBedTarget(temp);
				#endif

				#if FAN_COUNT > 0
					dispfe.setHasFan(true);
		    		dispfe.setFan(temp);
				#endif
			}

			if(dispfe.getPage() == movement_page)
			{
				dispfe.setXPos();
		    	dispfe.setYPos();
		    	dispfe.setZPos();
			}

            
		    dispfe.setIsPrinting(isPrinting());
            

		    dispfe.setIsHomed(all_axes_homed());
			dispfe.setExtruders(EXTRUDERS);
			dispfe.setIsPaused(card.isPaused());

		    dispfe.resetPageChanged();

            #if ENABLED(NEXTION_TIME)
		        //dispfe.setTime(year(), month(), day(), hour(), minute());
		        dispfe.setTime();
            #endif

			update_sd();

			if(sd_readed && dispfe.getPage() == SD_page)
			{
				if(!sd_need_update)
					dispfe.update_sd(files_list, files_less, files_count);
				sd_need_update = true;
			}
			else
			{
				sd_need_update = false;
			}

			#if HAS_PRINT_PROGRESS
				uint8_t progress = 0;
				#if ENABLED(SDSUPPORT)
					if (isPrinting()) 
					{
						progress = card.percentDone();
						dispfe.setTPercentage(progress);
						char time_buffer[21];
  						duration_t elapsed = print_job_timer.duration();
						uint32_t remaining_l = (elapsed.value*card.getIndex()/card.get_filesize())-elapsed.value;
						duration_t remaining = duration_t(remaining_l);
  						elapsed.toString(time_buffer);
						dispfe.setElapsed(time_buffer);
						remaining.toString(time_buffer);
						dispfe.setETA(time_buffer);
					}
				#endif
			#endif

		    next_lcd_update_ms = ms + LCD_UPDATE_INTERVAL;
        }
    }

	void NextionUI::read_sd_again()
	{
		sd_readed = false;
		dispfe.setSDState(card.flag.mounted && files_count > 0);
		if(IS_SD_INSERTED())
		{
			if(card.isMounted())
			{
				read_sd();
			}
		}
	}

	void NextionUI::read_sd()
	{
		if(!sd_readed)
		{
			files_count = card.get_num_Files();
			for(uint16_t i = 0; i<files_count; i++)
			{
				card.getfilename_sorted(i);
				strcpy(files_list[i], card.filename);
			}
			sd_readed = true;
		}
	}

	void NextionUI::update_sd()
	{
		#if ENABLED(SDSUPPORT)
			dispfe.setSDState(card.flag.mounted);
			if (!IS_SD_INSERTED())
			{
				if(!card.isMounted())
				{
					card.mount();
					read_sd();
				}
			}
			else
			{
        		sd_readed = false;
				files_count = 0;
				memset(files_list[0], 0, 27*64);
        		card.release();
      		}
		#endif
	}

	void NextionUI::kill_screen(PGM_P lcd_error, PGM_P lcd_component)
	{
  		// RED ALERT. RED ALERT.
    	leds.set_color(LEDColorRed());

  		dispfe.set_allert_screen();
		for(;;)  //wait for reboot
		{
		  	if(!inputQueue.isEmpty()) processBuffer(inputQueue.pop());
        	SerialEvent();

			millis_t ms = millis();

	    	if (ELAPSED(ms, next_lcd_update_ms))
        	{
				dispfe.set_allert_message(lcd_error);
				next_lcd_update_ms = ms + LCD_UPDATE_INTERVAL;
			}
		}
	}

	#if HAS_PRINT_PROGRESS
    	uint8_t NextionUI::get_progress() {
      	#if ENABLED(LCD_SET_PROGRESS_MANUALLY)
        	uint8_t &progress = progress_bar_percent;
        	#define _PLIMIT(P) ((P) & 0x7F)
      	#else
        	#define _PLIMIT(P) P
        	uint8_t progress = 0;
      	#endif
      	#if ENABLED(SDSUPPORT)
        	if (IS_SD_PRINTING()) progress = card.percentDone();
      	#endif
      	return _PLIMIT(progress);
    }
 	#endif

    bool NextionUI::has_status() { return (lcd_status_message[0] != '\0'); }
    void NextionUI::set_alert_status_P(PGM_P message) { processMessage(message); }
    void NextionUI::set_status_P(const char* message, const int8_t level) { processMessage(message);}
    void NextionUI::reset_status(const bool no_welcome) {}

	void NextionUI::abort_print()
	{
		#if ENABLED(SDSUPPORT)
      		wait_for_heatup = false;
      		card.flag.abort_sd_printing = true;
    	#endif
    	#ifdef ACTION_ON_CANCEL
      		host_action_cancel();
    	#endif
    	#if ENABLED(HOST_PROMPT_SUPPORT)
      		host_prompt_open(PROMPT_INFO, PSTR("UI Aborted"), DISMISS_STR);
    	#endif
    	print_job_timer.stop();
    	set_status_P(GET_TEXT(MSG_PRINT_ABORTED));
    	#if HAS_LCD_MENU
      		return_to_status();
    	#endif
	}

	void NextionUI::set_status(const char * const message, const bool persist)
	{
		if (alert_level) return;

    	/*#if ENABLED(HOST_PROMPT_SUPPORT)
    	  host_action_notify(message);
    	#endif

    	// Here we have a problem. The message is encoded in UTF8, so
    	// arbitrarily cutting it will be a problem. We MUST be sure
    	// that there is no cutting in the middle of a multibyte character!

    	// Get a pointer to the null terminator
    	const char* pend = message + strlen(message);

    	//  If length of supplied UTF8 string is greater than
    	// our buffer size, start cutting whole UTF8 chars
    	while ((pend - message) > MAX_MESSAGE_LENGTH) {
      	--pend;
      	while (!START_OF_UTF8_CHAR(*pend)) --pend;
    	};

    	// At this point, we have the proper cut point. Use it
    	uint8_t maxLen = pend - message;
    	strncpy(status_message, message, maxLen);
    	status_message[maxLen] = '\0';

    	finish_status(persist);*/
	}

    void NextionUI::processBuffer(const char* receivedString)
    {
        int receivedByte = strlen(receivedString);
	    int strLength = 0;
	    char subbuff[32] = { 0 };

	    switch (receivedString[0]) {
		case 'A':
		{
			strLength = receivedByte - 2;
			memcpy(subbuff, &receivedString[2], strLength);
			subbuff[strLength + 1] = '\0';

			thermalManager.setTargetBed(atoi(subbuff));
			#if ENABLED(PRINTJOB_TIMER_AUTOSTART)
    			thermalManager.check_timer_autostart(false, true);
  			#endif
		}break;
		case 'B':
		{
			strLength = receivedByte - 2;
			memcpy(subbuff, &receivedString[2], strLength);
			SERIAL_ECHO_START();
			SERIAL_ECHOLNPAIR("Stop received: ", subbuff);
			SERIAL_EOL();
			abort_print();
		}break;
		case 'E':
		{
			strLength = receivedByte - 3;
			memcpy(subbuff, &receivedString[3], strLength);
			subbuff[strLength + 1] = '\0';

			const int8_t target_extruder = ((int)receivedString[1]) - 1;
			const int16_t temp = atoi(subbuff);

			thermalManager.setTargetHotend(temp, target_extruder);
			#if ENABLED(PRINTJOB_TIMER_AUTOSTART)
    			thermalManager.check_timer_autostart(false, true);
  			#endif
			#if ENABLED(AUTOTEMP)
    			planner.autotemp_M104_M109();
  			#endif
			(void)thermalManager.wait_for_hotend(target_extruder, true);
		}break;
		#if FAN_COUNT > 0
	    case 'F':
		{
			strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[strLength + 1] = '\0';

			printercontrol::setFanSpeed(subbuff); //(ceil(atoi(subbuff) * 2.54));
			#if ENABLED(NEXTION_DEBUG)
				SERIAL_ECHO_START();
				SERIAL_ECHOLNPAIR("New fan speed: ", subbuff);
				SERIAL_EOL();
			#endif
		}break;
		#endif //END FAN_COUNT
		case 'G': //Send g-code
		{
			strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[strLength] = '\0';
			SERIAL_ECHO_START();
			SERIAL_ECHOLNPAIR("GCODE RECEIVED: ", subbuff);
			SERIAL_EOL();
		    queue.enqueue_one_now(subbuff);
		}break;
		case 'K':
		{
			strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
			if(subbuff == "1")
			{
				#if ENABLED(NEXTION_DEBUG)
					SERIAL_ECHO_START();
					SERIAL_ECHOLNPAIR("Stop status: ", subbuff);
					SERIAL_EOL();
				#endif
				void(*resetFunc)(void) = 0; // Declare resetFunc() at address 0
    			resetFunc();                // Jump to address 0
			}
		}break;
		case 'L':
		{
			strLength = receivedByte - 2;
			memcpy(subbuff, &receivedString[2], strLength);
			//todo add feedback
			if(atoi(subbuff) == 1)
				leds.set_white();
			else
				leds.set_off();
		}break;
	    case 'P':
		{
			strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[receivedByte - strLength] = '\0';

		    dispfe.setPage(atoi(subbuff));
			#if ENABLED(NEXTION_DEBUG)
				SERIAL_ECHO_START();
				SERIAL_ECHOLNPAIR("Page id: ", subbuff);
				SERIAL_EOL();
			#endif
		}break;
		case 'R':
		{
			strLength = receivedByte - 2;
			memcpy(subbuff, &receivedString[2], strLength);
			read_sd_again();
			dispfe.update_sd(files_list, files_less, files_count);
		}break;
		case 'S':
		{
			strLength = receivedByte - 2;
			memcpy(subbuff, &receivedString[2], strLength);
			files_less += atoi(subbuff);
			dispfe.update_sd(files_list, files_less, files_count);
		}break;
		case 'M':
		{
			strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
			int setting_num = atoi(subbuff);
			switch (setting_num)
			{
			char char_buff[32];
			case 0: dispfe.setKeyValue(itoa(planner.settings.max_feedrate_mm_s[X_AXIS],char_buff, 10)); break;
			case 1: dispfe.setKeyValue(itoa(planner.settings.max_feedrate_mm_s[Y_AXIS],char_buff, 10)); break;
			case 2: dispfe.setKeyValue(itoa(planner.settings.max_feedrate_mm_s[Z_AXIS],char_buff, 10)); break;
			case 3: dispfe.setKeyValue(itoa(planner.settings.max_feedrate_mm_s[E_AXIS],char_buff, 10)); break;
			case 4: dispfe.setKeyValue(itoa(planner.settings.max_acceleration_mm_per_s2[X_AXIS],char_buff, 10)); break;
			case 5: dispfe.setKeyValue(itoa(planner.settings.max_acceleration_mm_per_s2[Y_AXIS],char_buff, 10)); break;
			case 6: dispfe.setKeyValue(itoa(planner.settings.max_acceleration_mm_per_s2[Z_AXIS],char_buff, 10)); break;
			case 7: dispfe.setKeyValue(itoa(planner.settings.max_acceleration_mm_per_s2[E_AXIS],char_buff, 10)); break;
			case 8: dispfe.setKeyValue(itoa(planner.max_jerk[X_AXIS],char_buff, 10)); break;
			case 9: dispfe.setKeyValue(itoa(planner.max_jerk[Y_AXIS],char_buff, 10)); break;
			case 10: dispfe.setKeyValue(itoa(planner.max_jerk[Z_AXIS],char_buff, 10)); break;
			case 11: dispfe.setKeyValue(itoa(planner.max_jerk[E_AXIS],char_buff, 10)); break;
			case 13: dispfe.setKeyValue(itoa(planner.settings.axis_steps_per_mm[X_AXIS],char_buff, 10)); break;
			case 14: dispfe.setKeyValue(itoa(planner.settings.axis_steps_per_mm[Y_AXIS],char_buff, 10)); break;
			case 15: dispfe.setKeyValue(itoa(planner.settings.axis_steps_per_mm[Z_AXIS],char_buff, 10)); break;
			case 16: dispfe.setKeyValue(itoa(planner.settings.axis_steps_per_mm[E_AXIS],char_buff, 10)); break;
			
			default:
				break;
			}
			SERIAL_ECHO_START();
			SERIAL_ECHOLNPAIR("M received: ", subbuff);
			SERIAL_EOL();
		}break;
	    }
    }

    void NextionUI::ProcessPage(char * inputString, uint8_t receivedBytes)
    {
        char subbuff[4];
	    uint8_t strLength = receivedBytes - 2;

	    memcpy(subbuff, &inputString[2], strLength);
	    subbuff[strLength] = 0;
	    dispfe.setPage(atoi(subbuff));

		#if ENABLED(NEXTION_DEBUG)
			SERIAL_ECHO_START();
			SERIAL_ECHOLNPAIR("Front-end page state: ", dispfe.getPage());
			SERIAL_EOL();
		#endif
    }

    void NextionUI::processMessage(const char * message)
    {
		#if ENABLED(NEXTION_DEBUG)
        	SERIAL_ECHO_START();
	    	SERIAL_ECHOLNPAIR("Received message: ", message);
	    	SERIAL_EOL();
		#endif

        char subbuff[32];
	    char buff[5];
        
        if (message[0] == 'L' && message[1] == 'a') //La/yer
	    {
		    const uint8_t startPos = 6;
		    uint8_t toCounter = startPos;
		    while (toCounter<32)
		    {
			    if (message[toCounter] == ' ')
			    {
				    memcpy(subbuff, &message[startPos], toCounter - startPos);
				    subbuff[toCounter - startPos] = '\0';
				    break;
			    }
			    toCounter++;
		    }
		    //dispfe.setTLayers(subbuff);
			#if ENABLED(NEXTION_DEBUG)
				SERIAL_ECHO_START();
				SERIAL_ECHOLNPAIR("Layer count: ", subbuff);
				SERIAL_EOL();
			#endif
		    return;
	    }
        #if ENABLED(NEXTION_TIME)
        else if (message[0] == 'T' && message[1] == 'M' && message[2] == '=')
	    {
		    unsigned long myTime;
		    strlcpy(subbuff, &message[3], 11);
		    myTime = atol(subbuff) + 7200;
		    setTime(myTime);
        }
        #endif
        else
        {
            bool dotFound = false;
		    uint8_t dotLocation = 0;
		    if (message[0] != 'I' && message[1] != 'P' && message[0]!='H') //exclude IP Address and Heating...
		    {
                for (uint8_t i = 0; i < 9; i++)
			    {
				    if (message[i] == '.')
				    {
					    dotFound = true;
					    dotLocation = i;
				    }
			    }
            }
            if (dotFound)
		    {
			    memcpy(subbuff, &message[0], dotLocation);
			    subbuff[dotLocation] = '%';
			    subbuff[dotLocation + 1] = '\0';
			    //dispfe.setTPercentage(subbuff);
				#if ENABLED(NEXTION_DEBUG)
					SERIAL_ECHO_START();
					SERIAL_ECHOLNPAIR("Percentage: ", subbuff);
					SERIAL_EOL();
				#endif
			    return;
		    }
			#if ENABLED(NEXTION_DEBUG)
				SERIAL_ECHO_START();
				SERIAL_ECHOLNPAIR("Displayed message: ", subbuff);
				SERIAL_EOL();
			#endif
            return;
        }
        
    }

    void NextionUI::SerialEvent() 
    {
        volatile char inputChar;
	    volatile uint8_t stringLength = 0;
	    strStart = false;
	    strEnd = false;
	    memset(buffer, 0, 32);
	    while (nexSerial.available())
	    {
            inputChar = (char)nexSerial.read();
		    delayMicroseconds(200);
		    if ((int)inputChar >= 32 && (int)inputChar < 255)
		    {
			    if (inputChar != '!')
			    {
				    strEnd = false;
			    }
			    if (!strEnd && inputChar != '!' && inputChar != '*')
			    {
				    buffer[stringLength] = inputChar;
				    stringLength++;
			    }

			    if (inputChar == '!' && !strEnd)
			    {
				    strEnd = true;
				    buffer[stringLength] = 0;

				    if (buffer[0] == 'P')
				    {
					    ProcessPage(buffer, stringLength);
					    stringLength = 0;
				    }
				    else
				    {
					    inputQueue.push(buffer);
					    stringLength = 0;
				    }
			    }
		    }
        }
    }

	bool NextionUI::isPrintingFromMedia() { return IFSD(card.isFileOpen(), false); }
    bool NextionUI::isPrinting() { return (planner.movesplanned() || isPrintingFromMedia() || IFSD(IS_SD_PRINTING(), false)); }
    bool NextionUI::isMoving() { return planner.has_blocks_queued(); }

#endif