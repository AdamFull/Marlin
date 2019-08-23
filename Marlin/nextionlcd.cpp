#include "MarlinConfig.h"

#if ENABLED(NEXTION_LCD)

	#if ENABLED(SDSUPPORT)
  		#include "cardreader.h"
  		#include "SdFatConfig.h"
	#else
  		#define LONG_FILENAME_LENGTH 0
	#endif

    #include "nextionlcd.h"
    #include "nextionlcdelements.h"
	
	#include "planner.h"
	#include "stepper.h"
	#include "duration_t.h"
	#include "printcounter.h"
	#include "parser.h"
	#include "configuration_store.h"
    #include "queue.h"

	#include "Marlin.h"

	#if USE_MARLINSERIAL
  		// Make an exception to use HardwareSerial too
  		#undef HardwareSerial_h
		#include <HardwareSerial.h>
		#define USB_STATUS true
	#else
  		#define USB_STATUS Serial3
	#endif

    nextionlcdelements interface;

    #if ENABLED(NEXTION_TIME)
        #include "TimeLib.h"
    #endif

    const byte MAX_MESSAGE_LENGTH = 255;
    char lcd_status_message[MAX_MESSAGE_LENGTH + 1];
    uint8_t lcd_status_update_delay = 1, // First update one loop delayed
        lcd_status_message_level;

    #if HAS_BUZZER && DISABLED(LCD_USE_I2C_BUZZER)
        #include "buzzer.h"
    #endif

    queue inputQueue;
    nextionlcdelements dispfe;
    char buffer[32];
    bool strEnd = false, strStart = true;

    #define LCD_UPDATE_INTERVAL  500

    millis_t next_lcd_update_ms;

    void lcd_init() 
    {
        bool initState = nexInit();
        interface.setInitStatus(initState);
        SERIAL_ECHO_START();
		SERIAL_ECHOLNPAIR("NEXTION INITIALISE IS ", initState ? "TRUE" : "FALSE");
		SERIAL_EOL();
        lcd_update();
        interface.setStarted();
    }

    void lcd_update() 
    {
        if(!inputQueue.isEmpty()) processBuffer(inputQueue.pop());
        SerialEvent();
        millis_t ms = millis();

	    if (ELAPSED(ms, next_lcd_update_ms))
        {
            char temp[10];

			#if EXTRUDERS > 0
		    	dispfe.setExtruderActual(); //Set current extruder temperature
		    	dispfe.setExtruderTarget(); //Set extruder target temperature
			#endif

			#if HAS_HEATED_BED
		    	dispfe.setBedActual();
		    	dispfe.setBedTarget(temp);
			#endif

		    dispfe.setXPos();
		    dispfe.setYPos();
		    dispfe.setZPos();

			#if FAN_COUNT > 0
		    	dispfe.setFan(temp);
			#endif

			#if defined(PS_ON_PIN)
		    	dispfe.setPower(digitalRead(PS_ON_PIN));
			#endif

            #if defined(CASE_LIGHT_PIN)
		        dispfe.setCaseLight(digitalRead(CASE_LIGHT_PIN));
            #endif

            #if ENABLED(ADVANCED_OK) //or HAS_ABL
		        dispfe.setIsPrinting(planner.movesplanned());
            #endif

		    dispfe.setIsHomed(all_axes_homed());

		    dispfe.resetPageChanged();

            #if ENABLED(NEXTION_TIME)
		        //dispfe.setTime(year(), month(), day(), hour(), minute());
		        dispfe.setTime();
            #endif

		    next_lcd_update_ms = ms + LCD_UPDATE_INTERVAL;
        }
    }

    bool lcd_detected() { return interface.getInitStatus(); }
    void lcd_setalertstatusPGM(const char* message) {}

    bool lcd_hasstatus() { return (lcd_status_message[0] != '\0'); }
    void lcd_setstatus(const char* message, const bool persist) { processMessage(message); }
    void lcd_setstatusPGM(const char* message, const int8_t level) { processMessage(message);}
    void lcd_reset_alert_level() {}
    void lcd_reset_status() {}

    uint8_t lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW;

    void processBuffer(const char* receivedString)
    {
        int receivedByte = strlen(receivedString);
	    int strLength = 0;
	    char subbuff[32] = { 0 };

	    switch (receivedString[0]) {
		#if EXTRUDERS > 0
	    case 'E':
		    strLength = receivedByte - 3;
		    memcpy(subbuff, &receivedString[3], strLength);
		    subbuff[strLength + 1] = '\0';

			dispfe.setExtruderTemperature(atoi(subbuff), ((int)receivedString[1]) - 1);
		    break;
		#endif //END HAS_EXTRUDER
		#if HAS_HEATED_BED
	    case 'A':
		    strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[strLength + 1] = '\0';
			
			dispfe.setBedTemperature(atoi(subbuff));
		    break;
		#endif //END HAS_HEATED_BED
		#if FAN_COUNT > 0
	    case 'F':
		    strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[strLength + 1] = '\0';

		    fanSpeeds[0] = map(atoi(subbuff), 0, 100, 0, 255); //(ceil(atoi(subbuff) * 2.54));
		    break;
		#endif //END FAN_COUNT
	    case 'G': //Send g-code
		    strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[strLength] = '\0';

		    enqueue_and_echo_command(subbuff);
		    break;
	    case 'P':
		    strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[receivedByte - strLength] = '\0';

		    dispfe.setPage(atoi(subbuff));
		    break;
		#if defined(PS_ON_PIN)
	    case 'I': //Power status
		    strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[receivedByte - strLength] = '\0';

		    dispfe.setPower((bool)atoi(subbuff));
		    break;
		#endif //END PS_ON_PIN
		#if defined(CASE_LIGHT_PIN)
	    case 'C': //Case Light
		    strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[receivedByte - strLength] = '\0';

		    dispfe.setCaseLight((bool)atoi(subbuff));
		    break;
		#endif //END CASE_LIGHT_PIN
	    }
    }

    void ProcessPage(char * inputString, uint8_t receivedBytes)
    {
        char subbuff[4];
	    uint8_t strLength = receivedBytes - 2;

	    memcpy(subbuff, &inputString[2], strLength);
	    subbuff[strLength] = 0;

	    dispfe.setPage(atoi(subbuff));

	    SERIAL_ECHO_START();
	    SERIAL_ECHOLNPAIR("Page: ", dispfe.getPage());
	    SERIAL_EOL();
    }

    void processMessage(const char * message)
    {
        SERIAL_ECHO_START();
	    SERIAL_ECHOLNPAIR("Received message: ", message);
	    SERIAL_EOL();

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
				    toCounter;
				    memcpy(subbuff, &message[startPos], toCounter - startPos);
				    subbuff[toCounter - startPos] = '\0';
				    break;
			    }
			    toCounter++;
		    }
		    dispfe.setTLayers(subbuff);
		    return;
	    }
        else if (message[0] == 'E' && message[1] == 'T' && message[2] == 'L')
	    {
		    strlcpy(subbuff, &message[4], 11);
		    dispfe.setTETA(subbuff);
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
			    dispfe.setTPercentage(subbuff);
			    return;
		    }
            dispfe.setMessage(message);
            return;
        }
        
    }

    void SerialEvent() 
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

#endif