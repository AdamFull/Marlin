#include "nextionlcdelements.h"
#include "printercontrol.h"
#include "../../Configiration_nextion.h"
#include "../libs/numtostr.h"

#if ENABLED(NEXTION_LCD)

	#if ENABLED(SDSUPPORT)
		void nextionlcdelements::update_sd(char files_list[64][27], uint16_t files_less, uint16_t files_count)
		{
			vaLastPos.setValue(files_less);
			vaMax.setValue(files_count <= 6 ? 0 : files_count - 6 - files_less);
			for(unsigned i = 0; i < num_of_lines; i++)
			{
				if(files_count<=num_of_lines)
					list_lines[i].setText(files_list[i+files_less]);
				else if(files_less+i>files_count)
					list_lines[i].setText("");
				{
					list_lines[i].setText(files_list[i+files_less]);
				}
			}
		}
	#endif

	void nextionlcdelements::setXPos()
	{
		uint32_t xPos = (uint32_t)printercontrol::getCurrentPosition(X_AXIS);
		if (xPos != _x || _pageChanged || !isStarted())
		{
			this->vaX.setValue(xPos);
			_x = xPos;
		}
	}

	void nextionlcdelements::setYPos() 
	{
		uint32_t yPos = (uint32_t)printercontrol::getCurrentPosition(Y_AXIS);
		if (yPos != _y || _pageChanged || !isStarted())
		{
			this->vaY.setValue(yPos);
			_y = yPos;
		}
	}

	void nextionlcdelements::setZPos()
	{
		uint32_t zPos = (uint32_t)printercontrol::getCurrentPosition(Z_AXIS);
		if (zPos != _z || _pageChanged || !isStarted())
		{
			this->vaZ.setValue(zPos);
			_z = zPos;
		}
	}

	void nextionlcdelements::setBedTarget(char *temp)
	{
		int32_t inTemp = (int32_t)printercontrol::getDegTargetBed();
		if (inTemp != _bt || _pageChanged || !isStarted())
		{
			if(_page == printing_page)
			{
				this->vaPPBedT.setValue(inTemp);
			}
			else
			{
				this->vaBedT.setValue(inTemp);
			}
			_bt = inTemp;
		}
	}

	void nextionlcdelements::setBedActual()
	{
		uint32_t average = (uint32_t)((_ba + (uint32_t)printercontrol::getDegBed()) / 2);
		if (_ba != average || _pageChanged || !isStarted())
		{
			if(_page == printing_page)
			{
				this->vaPPBedA.setValue(average);
			}
			else
			{
				this->vaBedA.setValue(average);
			}
			_ba = average;
		}
	}

	void nextionlcdelements::setExtruderTarget()
	{
		for(unsigned i = 0; i < EXTRUDERS; i++)
		{
			int32_t inTemp = (int32_t)printercontrol::getDegTargetHotend(i);
			if (inTemp != _et[i] || _pageChanged || !isStarted())
			{
				if(_page == 2) this->vaExtrudersT[i].setValue(inTemp);
				if(_page == 3) this->vaPPExtrudersT[i].setValue(inTemp);
				_et[i] = inTemp;
			}
		}
	}

	void nextionlcdelements::setExtruderActual()
	{
		for(unsigned i = 0; i < EXTRUDERS; i++)
		{
			uint32_t average = (uint32_t)((_ea[i] + (uint32_t)printercontrol::getDegHotend(i)) / 2);
			if (_ea[i] != average || _pageChanged || !isStarted())
			{
				if(_page == printing_page)
				{
					this->vaPPExtrudersA[i].setValue(average);
				}	
				else
				{
					this->vaExtrudersA[i].setValue(average);
				}
				_ea[i] = average;
			}
		}
	}

	void nextionlcdelements::set_allert_screen(PGM_P lcd_error)
	{
		vaAlert.setValue(1);
		vaKillMessage.setText("Error received");
	}

	void nextionlcdelements::set_allert_message(const char* message)
	{
		vaKillMessage.setText(message);
	}

	void nextionlcdelements::setFan(char *temp)
	{
		for(unsigned i = 0; i < FAN_COUNT; i++)
		{
			uint32_t inSpeed = (uint32_t)map(printercontrol::getFanSpeed(i), 0, 255, 0, 100);
			if (inSpeed != _fan || _pageChanged || !isStarted())
			{
				this->vaFan.setValue(inSpeed);
				_fan = inSpeed;
			}
		}
	}

	void nextionlcdelements::setPage(int page)
	{
		if (page != _page)
			_pageChanged = true;
		_page = page;
	}

	int nextionlcdelements::getPage()
	{
		return _page;
	}

	void nextionlcdelements::resetPageChanged()
	{
		_pageChanged = false;
	}

	bool nextionlcdelements::pageChanged()
	{
		return _pageChanged;
	}

	void nextionlcdelements::setIsPrinting(bool status)
	{
		if(_isPrinting != status){
			_isPrinting = status;
			if(_page == 2) vaIsPrinting.setValue(status ? 1 : 0);
			if(_page == 3) vaPPIsPrinting.setValue(status ? 1 : 0);
		}
	}

	void nextionlcdelements::setIsHomed(bool status)
	{
		if (_isHomed != status || _pageChanged || !isStarted())
	 	{
	 		_isHomed = status;
         	status ? this->pHome.setPic(53) : this->pHome.setPic(20);
	 	}
	}

	bool nextionlcdelements::getIsHomed()
	{
	 	return _isHomed;
	}

	void nextionlcdelements::setStarted()
	{
		_startup = true;
	}

	bool nextionlcdelements::isStarted()
	{
		return _startup;
	}


	void nextionlcdelements::setMessage(const char * inStr)
	{
		this->vaLastMessage.setText(inStr);
	}

	void nextionlcdelements::setTPercentage(uint8_t value)
	{
		if(_last_perc != value)
		{
			_last_perc = value;
			this->vaProgress.setValue(value);
		}
	}

	char* nextionlcdelements::XPortMsg(char* mHeader, char* inmsg)
	{
		// retrieve incoming string and assign to variable
		char* msgHeader = new char[strlen(inmsg)+1];
		memcpy(msgHeader,inmsg,strlen(inmsg)+1);
		
		char* retVal = new char[strlen(mHeader)+strlen(msgHeader)+1];
		
		// Assemble the string
		strcat(retVal,mHeader);
		strcat(retVal,msgHeader);

		return retVal;
	}

	void nextionlcdelements::animate()
	{
		
	}

	#if ENABLED(NEXTION_TIME)

	//void PrinterClass::setTime(int year, int month, int day, int hour, int minute)
	void nextionlcdelements::setTime()
	{
		int myMinute = minute();
		if (myMinute != _minutes)
		{
			_minutes = myMinute;

			int myMonth = month();
			int myDay = day();
			int myHour = hour();


			char myBuff[5];

			strcpy(ext, itoa(year(), myBuff, 10));
			strcat(ext, "-");

			if (myMonth < 10)
				strcat(ext, "0");
			strcat(ext, itoa(myMonth, myBuff, 10));
		
		
			strcat(ext, "-");

			if (myDay < 10)
				strcat(ext, "0");
			strcat(ext, itoa(day(), myBuff, 10));


			strcat(ext, " ");

			if (myHour < 10)
				strcat(ext, "0");
			strcat(ext, itoa(hour(), myBuff, 10));

			strcat(ext, ":");

			if (myMinute < 10)
				strcat(ext, "0");
			strcat(ext, itoa(myMinute, myBuff, 10));

			tTime.setText(ext);
		}
	}

	#endif
#endif