#include "nextionlcdelements.h"
#include "printercontrol.h"
#include "../../Configiration_nextion.h"
#include "../libs/numtostr.h"

#if ENABLED(NEXTION_LCD)

	#if ENABLED(SDSUPPORT)
		void nextionlcdelements::update_sd(char files_list[64][27], uint16_t files_less, uint16_t files_count)
		{
			vaLastPos.setValue(files_less);
			vaMax.setValue(files_count-num_of_lines);
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
		float xPos = printercontrol::getCurrentPosition(X_AXIS);
		if (xPos != _x || _pageChanged || !isStarted())
		{
			this->vaX.setValue(xPos);
			_x = xPos;
		}
	}

	void nextionlcdelements::setYPos() 
	{
		float yPos = printercontrol::getCurrentPosition(Y_AXIS);
		if (yPos != _y || _pageChanged || !isStarted())
		{
			this->vaY.setValue(yPos);
			_y = yPos;
		}
	}

	void nextionlcdelements::setZPos()
	{
		float zPos = printercontrol::getCurrentPosition(Z_AXIS);
		if (zPos != _z || _pageChanged || !isStarted())
		{
			this->vaZ.setValue(zPos);
			_z = zPos;
		}
	}

	void nextionlcdelements::setBedTarget(char *temp)
	{
		int16_t inTemp = printercontrol::getDegTargetBed();
		if (inTemp != _bt || _pageChanged || !isStarted())
		{
			this->vaBedT.setValue(inTemp);
			_bt = inTemp;
		}
	}

	void nextionlcdelements::setBedActual()
	{
		uint16_t average = (uint8_t)((_ba + (uint8_t)printercontrol::getDegBed()) / 2);
		if (_ba != average || _pageChanged || !isStarted())
		{
			this->vaBedA.setValue(average);
			_ba = average;
		}
	}

	void nextionlcdelements::setExtruderTarget()
	{
		for(unsigned i = 0; i < EXTRUDERS; i++)
		{
			int16_t inTemp = printercontrol::getDegTargetHotend(i);
			switch (i)
			{
			case 0 :
				if (inTemp != _et || _pageChanged || !isStarted())
				{
					this->vaExtruder1T.setValue(inTemp);
					_et = inTemp;
				}
			 	break;
			#if EXTRUDERS == 2
				case 1:
					if (inTemp != _et1 || _pageChanged || !isStarted())
					{
						this->vaExtruder2T.setValue(inTemp);
						_et1 = inTemp;
					}
					break;
			#endif
			}
		}
	}

	void nextionlcdelements::setExtruderActual()
	{
		for(unsigned i = 0; i < EXTRUDERS; i++)
		{
			switch (i)
			{
			case 0:
				uint16_t average = (uint8_t)((_ea + (uint8_t)printercontrol::getDegHotend(i)) / 2);
				if (_ea != average || _pageChanged || !isStarted())
				{
					this->vaExtruder1A.setValue(average);
					_ea = average;
				}
			 	break;
			#if EXTRUDERS > 1
			case 1:
				average = (uint8_t)((_ea1 + (uint8_t)printercontrol::getDegHotend(i)) / 2);
				if (_ea1 != average || _pageChanged || !isStarted())
			 	{
			 		this->vaExtruder2A.setValue(average);
			 		_ea1 = average;
			 	}
			 	break;
			#endif
			}
		}
	}

	void nextionlcdelements::set_allert_screen(const char* message)
	{
		vaAlert.setValue(1);
		delay(1000);
		vaLastMessage.setText(message);
		vaKillMessage.setText(message);
	}

	void nextionlcdelements::setFan(char *temp)
	{
		for(unsigned i = 0; i < FAN_COUNT; i++)
		{
			int16_t inSpeed = map(printercontrol::getFanSpeed(i), 0, 255, 0, 100);
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

	// void nextionlcdelements::setPower(bool status)
	// {
	// 	if (_power != status || _pageChanged || !isStarted())
	// 	{
	// 		this->_power = status;

	// 		this->vaPower.setValue(_power ? 1 : 0);
	// 		if (_page == 2)
	// 			this->btPower.setValue(_power ? 1 : 0);
	// 		else if (_page == 1)
    //         	status ? this->pPower.setPic(pPower_e_id) : this->pPower.setPic(pEmpty_id);
	// 	}
	// }

	// void nextionlcdelements::setCaseLight(bool status)
	// {
	// 	if (_caseLight != status || _pageChanged || !isStarted())
	// 	{
	// 		this->_caseLight = status;

	// 		this->vaLight.setValue(_caseLight ? 1 : 0);
	// 		if (_page == 2)
	// 			this->btLight.setValue(_caseLight ? 1 : 0);
	// 		else if (_page == 1)
	// 		{
	// 			//uint32_t val = atoi(subbuff);
    //         	status == 1 ? this->pLight.setPic(pLight_e_id) : this->pLight.setPic(pEmpty_id);
	// 		}
	// 	}
	// }

	void nextionlcdelements::setIsPrinting(bool status)
	{
		if (_isPrinting != status || _pageChanged || !isStarted())
		{
			_isPrinting = status;

			if (status == 1)
				vaIsPrinting.setValue(1);
			else
				vaIsPrinting.setValue(0);
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

	// void nextionlcdelements::setTLayers(const char * inStr)
	// {
	// 	this->tLayers.setText(inStr);
	// }

	void nextionlcdelements::setTETA(const char * inStr)
	{
		this->tETA.setText(inStr);
	}

	void nextionlcdelements::setTPercentage(const char * inStr)
	{
		this->tPercentage.setText(inStr);
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