#include "nextionlcdelements.h"
#include "printercontrol.h"
#include "language.h"

#if ENABLED(NEXTION_LCD)

	void nextionlcdelements::setXPos()
	{
		char* xPos = ftostr52sp(printercontrol::getCurrentPosition(X_AXIS));
		if (strcmp(xPos, _x)!=0 || _pageChanged || !isStarted())
		{
			this->tX.setText(xPos);
			strcpy(_x, xPos);
		}
	}

	void nextionlcdelements::setYPos() 
	{
		char* yPos = ftostr52sp(printercontrol::getCurrentPosition(Y_AXIS));
		if (strcmp(yPos, _y) != 0 || _pageChanged || !isStarted())
		{
			this->tY.setText(yPos);
			strcpy(_y, yPos);
		}
	}

	void nextionlcdelements::setZPos()
	{
		char* zPos = ftostr52sp(printercontrol::getCurrentPosition(Z_AXIS));
		if (strcmp(zPos, _z) != 0 || _pageChanged || !isStarted())
		{
			this->tZ.setText(zPos);
			strcpy(_z, zPos);
		}
	}

	void nextionlcdelements::setBedTarget(char *temp)
	{
		char* inTemp = itoa(printercontrol::getDegTargetBed(), temp, 10);
		if (strcmp(inTemp, _bt) != 0 || _pageChanged || !isStarted())
		{
			this->tBedT.setText(inTemp);
			strcpy(_bt, inTemp);
		}
	}

	void nextionlcdelements::setBedActual()
	{
		uint16_t average = (uint8_t)((_ba + (uint8_t)printercontrol::getDegBed()) / 2);
		if (_ba != average || _pageChanged || !isStarted())
		{
			this->tBedA.setText(itoa(average, ext, 10));
			_ba = average;
		}
	}

	void nextionlcdelements::setExtruderTarget()
	{
		for(unsigned i = 0; i <= EXTRUDERS; i++)
		{
			char* inTemp = itostr3left(printercontrol::getDegTargetHotend(i));
			if (strcmp(inTemp, _et) != 0 || _pageChanged || !isStarted())
			{
				this->tExtruder1T.setText(inTemp);
				strcpy(_et, inTemp);
			}
		}
	}

	void nextionlcdelements::setExtruderActual()
	{
		for(unsigned i = 0; i <= EXTRUDERS; i++)
		{
			uint16_t average = (uint8_t)((_ea + (uint8_t)printercontrol::getDegHotend(i)) / 2);
			if (_ea != average || _pageChanged || !isStarted())
			{
				this->tExtruder1A.setText(itoa(average, ext, 10));
				_ea = average;
			}
		}
	}

	void nextionlcdelements::setFan(char *temp)
	{
		for(unsigned i = 0; i <= FAN_COUNT; i++)
		{
			char* inSpeed = itoa(map(printercontrol::getFanSpeed(i), 0, 255, 0, 100), temp, 10);
			if (strcmp(inSpeed, _fan) != 0 || _pageChanged || !isStarted())
			{
				this->tFan.setText(inSpeed);
				strcpy(_fan, inSpeed);
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

	void nextionlcdelements::setPower(bool status)
	{
		if (_power != status || _pageChanged || !isStarted())
		{
			this->_power = status;

			this->vaPower.setValue(_power ? 1 : 0);
			if (_page == 2)
				this->btPower.setValue(_power ? 1 : 0);
			else if (_page == 1)
            	status ? this->pPower.setPic(pPower_e_id) : this->pPower.setPic(pEmpty_id);
		}
	}

	void nextionlcdelements::setCaseLight(bool status)
	{
		if (_caseLight != status || _pageChanged || !isStarted())
		{
			this->_caseLight = status;

			this->vaLight.setValue(_caseLight ? 1 : 0);
			if (_page == 2)
				this->btLight.setValue(_caseLight ? 1 : 0);
			else if (_page == 1)
			{
				//uint32_t val = atoi(subbuff);
            	status == 1 ? this->pLight.setPic(pLight_e_id) : this->pLight.setPic(pEmpty_id);
			}
		}
	}

	void nextionlcdelements::setIsPrinting(bool status)
	{
		if (_isPrinting != status || _pageChanged || !isStarted())
		{
			_isPrinting = status;

			if (status == 1)
			{
				this->pExtruding.setPic(pExtruding_e_id);
				this->tmSS.disable();
				this->vaCounter.setValue(0);
			}
			else
			{
				this->pExtruding.setPic(pEmpty_id);
				this->tmSS.enable();
				this->vaCounter.setValue(0);
			}
		}
	}

	void nextionlcdelements::setIsHomed(bool status)
	{
		if (_isHomed != status || _pageChanged || !isStarted())
		{
			_isHomed = status;
        	status == 1 ? this->pHome.setPic(pHome_e_id) : this->pHome.setPic(pEmpty_id);
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
		this->tMessage.setText(inStr);
	}

	void nextionlcdelements::setTLayers(const char * inStr)
	{
		this->tLayers.setText(inStr);
	}

	void nextionlcdelements::setTETA(const char * inStr)
	{
		this->tETA.setText(inStr);
	}

	void nextionlcdelements::setTPercentage(const char * inStr)
	{
		this->tPercentage.setText(inStr);
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