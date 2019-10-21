#include "../inc/MarlinConfig.h"
#include "../../Configiration_nextion.h"

#if ENABLED(NEXTION_LCD)

	uint16_t files_count;
    char files_list[64][27];
    uint16_t files_less;
	bool sd_readed;
	bool sd_need_update;

	bool readed = false;

	#if ENABLED(SDSUPPORT)
  		#include "../sd/cardreader.h"
  		//#include "SdFatConfig.h"
	#else
  		#define LONG_FILENAME_LENGTH 0
	#endif

    #include "nextionlcd.h"
	NextionUI ui;
    #include "nextionlcdelements.h"
	#include "printercontrol.h"
	
	#include "../module/planner.h"
	//#include "../module/printcounter.h"
    #include "command_queue.h"

	#include "../Marlin.h"

	/*#if USE_MARLINSERIAL
  		// Make an exception to use HardwareSerial too
  		#undef HardwareSerial_h
		#include <HardwareSerial.h>
		//#include "Arduino.h"
		#define USB_STATUS true
	#else
  		#define USB_STATUS Serial2
	#endif*/

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

    #define LCD_UPDATE_INTERVAL  400

    millis_t next_lcd_update_ms;
	millis_t sd_update_ms;

    void NextionUI::init() 
    {
        bool initState = nexInit();
        dispfe.setInitStatus(initState);
		#if ENABLED(NEXTION_DEBUG)
        	SERIAL_ECHO_START();
			SERIAL_ECHOLNPAIR("NEXTION INITIALISE IS ", initState ? "TRUE" : "FALSE");
			SERIAL_EOL();
		#endif
		dispfe.setMessage("Printer is ready");
		
        update();
        dispfe.setStarted();
    }

    void NextionUI::update() 
    {
        if(!inputQueue.isEmpty()) processBuffer(inputQueue.pop());
        SerialEvent();
        millis_t ms = millis();

	    if (ELAPSED(ms, next_lcd_update_ms))
        {
            char temp[10];

			if(dispfe.getPage() == 2 || dispfe.getPage() == 3)
			{
		    	dispfe.setExtruderActual(); //Set current extruder temperature
		    	dispfe.setExtruderTarget(); //Set extruder target temperature

				#if HAS_HEATED_BED
		    		dispfe.setBedActual();
		    		dispfe.setBedTarget(temp);
				#endif

				#if FAN_COUNT > 0
		    		dispfe.setFan(temp);
				#endif
			}

			if(dispfe.getPage() == 9)
			{
				dispfe.setXPos();
		    	dispfe.setYPos();
		    	dispfe.setZPos();
			}

			//#if defined(PS_ON_PIN)
		    //	dispfe.setPower(digitalRead(PS_ON_PIN));
			//#endif

            //#if ENABLED(CASE_LIGHT_ENABLE)
		    //    dispfe.setCaseLight(digitalRead(CASE_LIGHT_PIN));
            //#endif

            #if ENABLED(ADVANCED_OK) //or HAS_ABL
		        dispfe.setIsPrinting(planner.movesplanned());
            #endif

		    //dispfe.setIsHomed(all_axes_homed());

		    dispfe.resetPageChanged();

            #if ENABLED(NEXTION_TIME)
		        //dispfe.setTime(year(), month(), day(), hour(), minute());
		        dispfe.setTime();
            #endif

			update_sd();

			if(sd_readed && dispfe.getPage() == 5)
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
					if (IS_SD_PRINTING()) progress = card.percentDone();
				#endif
			#endif

		    next_lcd_update_ms = ms + LCD_UPDATE_INTERVAL;
        }
    }

	void NextionUI::update_sd()
	{
		millis_t ms = millis();
		#if ENABLED(SDSUPPORT)
			if (ELAPSED(ms, sd_update_ms))
			{
				//if(dispfe.getPage() != 2 && dispfe.getPage() != 5) card.release();
				if(dispfe.getPage() == 2) card.initsd();
				sd_update_ms = ms + LCD_UPDATE_INTERVAL;
			}
			dispfe.setSDState(card.flag.detected);

				if(card.isDetected())
				{
					if(!sd_readed)
					{
						dispfe.setMessage("SD plugged in.");
						files_count = card.get_num_Files();
						dispfe.setSDFileCount(files_count);
						for(uint16_t i = 0; i<files_count; i++)
						{
							card.getfilename(i);
							strcpy(files_list[i], card.filename);
						}
						sd_readed = true;	
					}
				}
				else
				{
					sd_readed = false;
					files_count = 0;
					memset(files_list[0], 0, 27*64);
				}
		#endif
	}

    bool NextionUI::has_status() { return (lcd_status_message[0] != '\0'); }
    void NextionUI::set_alert_status_P(PGM_P message) { processMessage(message); }
    void NextionUI::set_status_P(const char* message, const int8_t level) { processMessage(message);}
    void NextionUI::reset_status() {}

    void NextionUI::processBuffer(const char* receivedString)
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

			printercontrol::setHotendTemperature(atoi(subbuff), ((int)receivedString[1]) - 1);
			dispfe.setMessage(dispfe.XPortMsg("Hew hotend target: ", subbuff));
			#if ENABLED(NEXTION_DEBUG)
				SERIAL_ECHO_START();
				SERIAL_ECHOLNPAIR("New hotend target: ", subbuff);
				SERIAL_EOL();
				SERIAL_ECHOLNPAIR("Hotend id: ", ((int)receivedString[1]) - 1);
				SERIAL_EOL();
			#endif
		    break;
		#endif //END HAS_EXTRUDER
		#if HAS_HEATED_BED
	    case 'A':
		    strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[strLength + 1] = '\0';
			
			printercontrol::setBedTemperature(atoi(subbuff));
			dispfe.setMessage(dispfe.XPortMsg("New bed target: ", subbuff));
			#if ENABLED(NEXTION_DEBUG)
				SERIAL_ECHO_START();
				SERIAL_ECHOLNPAIR("New bed target: ", subbuff);
				SERIAL_EOL();
			#endif
		    break;
		#endif //END HAS_HEATED_BED
		#if FAN_COUNT > 0
	    case 'F':
		    strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[strLength + 1] = '\0';

			printercontrol::setFanSpeed(subbuff); //(ceil(atoi(subbuff) * 2.54));
			dispfe.setMessage(dispfe.XPortMsg("New fan speed: ", subbuff));
			#if ENABLED(NEXTION_DEBUG)
				SERIAL_ECHO_START();
				SERIAL_ECHOLNPAIR("New fan speed: ", subbuff);
				SERIAL_EOL();
			#endif
		    break;
		#endif //END FAN_COUNT
	    case 'G': //Send g-code
		    strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[strLength] = '\0';
			dispfe.setMessage(dispfe.XPortMsg("G-Code: ", subbuff));
		    queue.enqueue_one_now(subbuff);
		    break;
	    case 'P':
		    strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[receivedByte - strLength] = '\0';

		    dispfe.setPage(atoi(subbuff));
			#if ENABLED(NEXTION_DEBUG)
				SERIAL_ECHO_START();
				SERIAL_ECHOLNPAIR("Page id: ", subbuff);
				SERIAL_EOL();
			#endif
		    break;
		case 'S':
			strLength = receivedByte - 2;
			memcpy(subbuff, &receivedString[2], strLength);
			files_less += atoi(subbuff);
			dispfe.update_sd(files_list, files_less, files_count);
			break;
		#if defined(PS_ON_PIN)
	    case 'I': //Power status
		    strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[receivedByte - strLength] = '\0';

		    //dispfe.setPower((bool)atoi(subbuff));
			#if ENABLED(NEXTION_DEBUG)
				SERIAL_ECHO_START();
				SERIAL_ECHOLNPAIR("New power state: ", subbuff);
				SERIAL_EOL();
			#endif
		    break;
		#endif //END PS_ON_PIN
		#if ENABLED(CASE_LIGHT_ENABLE)
	    case 'C': //Case Light
		    strLength = receivedByte - 2;
		    memcpy(subbuff, &receivedString[2], strLength);
		    subbuff[receivedByte - strLength] = '\0';

		    dispfe.setCaseLight((bool)atoi(subbuff));
			#if ENABLED(NEXTION_DEBUG)
				SERIAL_ECHO_START();
				SERIAL_ECHOLNPAIR("New case light state: ", subbuff);
				SERIAL_EOL();
			#endif
		    break;
		#endif //END CASE_LIGHT_PIN
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
				    toCounter;
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
        else if (message[0] == 'E' && message[1] == 'T' && message[2] == 'L')
	    {
		    strlcpy(subbuff, &message[4], 11);
		    dispfe.setTETA(subbuff);
			#if ENABLED(NEXTION_DEBUG)
				SERIAL_ECHO_START();
				SERIAL_ECHOLNPAIR("Current TETA time: ", subbuff);
				SERIAL_EOL();
			#endif
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
				#if ENABLED(NEXTION_DEBUG)
					SERIAL_ECHO_START();
					SERIAL_ECHOLNPAIR("Percentage: ", subbuff);
					SERIAL_EOL();
				#endif
			    return;
		    }
            dispfe.setMessage(message);
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

#endif