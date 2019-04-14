#include <LiquidCrystal.h>
#include <string.h>
#include <Wire.h>
#include "TimeLib.h"

enum ProgramState { OFF, SET_UP, DISPLAY_TIME };
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
const int redLedPin = 3, greenLedPin = 2;
const int upButtonPin = 13, downButtonPin = 10, enterButtonPin = 9;
int upButtonState = LOW, downButtonState = LOW, enterButtonState = LOW;
int programState = OFF;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void scrollPrint(int r, int c, char *str){
  
}

void setup(){
  Wire.begin();
  pinMode(upButtonPin, INPUT);
  pinMode(downButtonPin, INPUT);
  pinMode(enterButtonPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.blink();
}

int buttonPressed(int buttonPin, int *buttonState){
  if(!buttonState){return -1;}
  int before = digitalRead(buttonPin);
  delay(10);
  int after = digitalRead(buttonPin);
  int state = *buttonState;
  if(before == after){
    if(before == HIGH && *buttonState == LOW){
      *buttonState = before;
      return 1;
    }
    *buttonState = before;
  } 
  return 0;
}

void getTimeFromLCD(int *hour, int *minutes, int *seconds){
  if(!hour || !minutes || !seconds){ return; }
  lcd.clear();
	char buffer[100];
	char *format = "00:00:00";
  lcd.print(format);
	lcd.setCursor(0,0);
	int complete = 0;
	while(!complete){
		char *format = "%02d:00:00";
		if(buttonPressed(enterButtonPin, &enterButtonState)){
			complete = 1;
		}
		else{
			if(buttonPressed(upButtonPin, &upButtonState)){
				if(*hour + 1 <= 23){
					*hour++;
				}
			}
			if(buttonPressed(downButtonPin, &downButtonState)){
				if(*hour - 1 >= 0){
					*hour--;
				}
			}
		}
		snprintf(buffer, 100, format, *hour);
		lcd.print(buffer);
	}
	complete = 0;
	lcd.setCursor(3,0);
	while(!complete){
		char *format = "%02d:%02d:00";
		if(buttonPressed(enterButtonPin, &enterButtonState)){
			complete = 1;
		}
		else{
			if(buttonPressed(upButtonPin, &upButtonState)){
				if(*minutes + 1 <= 59){
					*minutes++;
				}
			}
			if(buttonPressed(downButtonPin, &downButtonState)){
				if(*minutes - 1 >= 0){
					*minutes--;
				}
			}
		}
		snprintf(buffer, 100, format, *hour, *minutes);
		lcd.print(buffer);
	}
	complete = 1;
	lcd.setCursor(6,0);
	while(!complete){
		char *format = "%02d:%02d:%02d";
		if(buttonPressed(enterButtonPin, &enterButtonState)){
			complete = 1;
		}
		else{
			if(buttonPressed(upButtonPin, &upButtonState)){
				if(*seconds + 1 <= 59){
					*seconds++;
				}
			}
			if(buttonPressed(downButtonPin, &downButtonState)){
				if(*seconds - 1 >= 0){
					*seconds--;
				}
			}
		}
		snprintf(buffer, 100, format, *hour, *minutes, *seconds);
		lcd.print(buffer);
	}
}

int validDate(int month, int day, int *year, int *max){
	switch(month){
		case 1: case 3: case 5: case 7:
		case 8: case 10: case 12:{
			if(max){ *max = 31; } 
			if(day >= 1 && day <= 31){
				return 1;
			}			
			break;
		}
		case 4: case 6: case 9: case 11:{
			if(max){ *max = 30; }
			if(day >= 1 && day <= 30){
				return 1;
			}
			break;
		}
		case 2:{
			// check for a leap year
			if(year
				&& (*year%400 == 0 || (*year%4 == 0 && *year%100 != 0))){
				// set leap year max
				if(max){ *max = 29; }
				if(day >= 1 && day <= 29){
					return 1;
				}
			}
			else{
				// without year default is no leap year
				if(day >= 1 && day <= 28){
					if(max){ *max = 28; }
					return 1;
				}
			}
			break;
		}
		default:
			break;
		}
		return 0;
}


void getDateFromLCD(int *month, int *day, int *year){
  if(!month || !day || !year){ return; }
  lcd.clear();
  lcd.setCursor(0,0); 
	char *format = "00.00.00";
	*month = 1;
	*day = 1;
	*year = 18;
  lcd.print(format);
	int complete = 0;
  while(!complete){
		char *format = "%02d.00.00";
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      complete = 1;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
				*month++;
				if(*month > 12){
					*month = 1;
				}
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
				*month--;
				if(*month < 1){
					*month = 12;
				}
      }
    }
		snprintf(buffer, 100, format, *month);
		lcd.print(buffer);
  }
	complete = 0;
	lcd.setCursor(3,0);
	while(!complete){
		char *format = "%02d.%02d.00";
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      complete = 1;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
				*day++;
				if(!validDate(*month, *day, NULL, NULL)){
					*day = 1;
				}
			}
      if(buttonPressed(downButtonPin, &downButtonState)){
				int max;
				*day--;
				if(!validDate(*month, *day, NULL, &max)){
					*day = max;
				}
      }
    }
		snprintf(buffer, 100, format, *month, *day);
		lcd.print(buffer);
	}
	complete = 0;
	lcd.setCursor(6, 0);
	while(!complete){
		char *format = "%02d.%02d.%02d";
		if(buttonPressed(enterButtonPin, &enterButtonState)){
			complete = 1;
		}
		else{
			if(buttonPressed(upButtonPin, &upButtonState)){
				int max;
				*year++;
				if(*year < 0){
					*year = 0;
				}
				else{

				}
				if(!validDate(*month, *day, year, &max)){
					// check if year is inside of range
					if(*year > 2099){
						*year = 2000;
					}
					// check if day is on leap year
					if(*day > max){
						// set day to the 29th on leap year
						*day = max;
					}
				}
				*year -= 2000;
			}
			if(buttonPressed(downButtonPin, &downButtonState)){
			
			}
		}
		snprintf(buffer, 100, format, *month, *day, *year);
		lcd.print(buffer);
	}
	






  *month = tensPlace * 10 + onesPlace; 
  tensPlace = 0;
  onesPlace = 0;
  i++;
  while(i < 5){
    lcd.setCursor(i, 1);
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      i++;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        if(i == 3 && tensPlace + 1  <= 3){
          tensPlace++;
          lcd.write('0' + tensPlace);
        }
        if(i == 4 && (tensPlace * 10 + (onesPlace + 1)) <= 31){
          onesPlace++;
          lcd.write('0' + onesPlace);
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        if(i == 3 && tensPlace - 1 >= 0){
          tensPlace--;
          lcd.write('0' + tensPlace);
        }
        if(i == 4 && onesPlace - 1 >= 0){
          onesPlace--;
          lcd.write('0' + onesPlace);
        }
      }
    }
  }
  *day = tensPlace * 10 + onesPlace; 
  tensPlace = 0;
  onesPlace = 0;
  i++; 
  while(i < 8){
    lcd.setCursor(i, 1);
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      i++;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        if(i == 6 && tensPlace + 1  <= 9){
          tensPlace++;
          lcd.write('0' + tensPlace);
        }
        if(i == 7 && onesPlace + 1 <= 9){
          onesPlace++;
          lcd.write('0' + onesPlace);
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        if(i == 6 && tensPlace - 1 >= 0){
          tensPlace--;
          lcd.write('0' + tensPlace);
        }
        if(i == 7 && onesPlace - 1 >= 0){
          onesPlace--;
          lcd.write('0' + onesPlace);
        }
      }
    }
  }
  *year = tensPlace * 10 + onesPlace;
}

void getScheduleFromLCD(int *days, int *hours){
  if(!days || !hours){ return; }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter schedule:");
  lcd.setCursor(0,1);
  lcd.print("00:00");
  int tensPlace = 0, onesPlace = 0;
  int i = 0;
  while(i < 2){
    lcd.setCursor(i,1);
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      i++;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        if(i == 0 && tensPlace + 1 <= 3){
          tensPlace++;
          lcd.write('0' + tensPlace);
        }
        if(i == 1 && (onesPlace + 1) <= 9
                  && (tensPlace * 10 + (onesPlace + 1)) <= 31){
            onesPlace++;
            lcd.write('0' + onesPlace);
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        if(i == 0 && tensPlace - 1 >= 0){
          tensPlace--;
          lcd.write('0' + tensPlace);
        }
        if(i == 1 && onesPlace - 1 >= 0){
          onesPlace--;
          lcd.write('0' + onesPlace);
        }
      }
    }
  }
  *days = tensPlace * 10 + onesPlace; 
  tensPlace = 0;
  onesPlace = 0;
  i++;
  while(i < 5){
    lcd.setCursor(i,1);
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      i++;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        if(i == 3 && tensPlace + 1  <= 2){
          tensPlace++;
          lcd.write('0' + tensPlace);
        }
        if(i == 4 && onesPlace + 1 <= 9
                  && (tensPlace * 10 + (onesPlace + 1)) <= 23){
          onesPlace++;
          lcd.write('0' + onesPlace);
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        if(i == 3 && tensPlace - 1 >= 0){
          tensPlace--;
          lcd.write('0' + tensPlace);
        }
        if(i == 4 && onesPlace - 1 >= 0){
          onesPlace--;
          lcd.write('0' + onesPlace);
        }
      }
    }
  }
  *hours = tensPlace * 10 + onesPlace;
}

void getUVThreshFromLCD(unsigned *uv_thresh){
  if(!uv_thresh){ return; }
  lcd.clear();
  lcd.print("00");
  lcd.setCursor(0,0);
  lcd.blink();
  int value = 0;
  while(1){
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      break;
    }
    if(buttonPressed(upButtonPin, &upButtonState)){
      if(value + 1 <= 1023){
        value++;
      }
    }
    if(buttonPressed(downButtonPin, &downButtonState)){
      if(value - 1 >= 0){
        value--;
      }
    }
  }
  *uv_thresh = value;
}

void broadcast(byte *data, unsigned bytes){
   // broadcast data to both devices 1, and 2
   Wire.beginTransmission(2);
   Wire.write(data, bytes);
   Wire.endTransmission();
   Wire.beginTransmission(1);
   Wire.write(data, bytes);
   Wire.endTransmission();
}

void loop(){;
  if(programState == OFF){
    digitalWrite(redLedPin, HIGH);
    if(buttonPressed(upButtonPin, &upButtonState)){
      digitalWrite(redLedPin, LOW);
      programState = SET_UP;
    }
    else if(buttonPressed(downButtonPin, &downButtonState)){
      digitalWrite(redLedPin, LOW);
      programState = SET_UP;
    }
    else if(buttonPressed(enterButtonPin, &enterButtonState)){
      digitalWrite(redLedPin, LOW);
      programState = SET_UP;
    }
  }
  else if(programState == SET_UP){
    digitalWrite(greenLedPin, HIGH);
    char buffer[100];
    unsigned month, day, year;
    unsigned hour, minutes, seconds;
    unsigned uv_thresh;
    // get the date and time from the user
    getDateFromLCD(&month, &day, &year);
    getTimeFromLCD(&hour, &minutes, &seconds);
    setTime(hour, minutes, seconds, day, month, year);
    unsigned days, hours;
    // get the watering schedule (in hours) from the user
    getScheduleFromLCD(&days, &hours);
    hours = days * 24 + hours;
    // get the uv threshold from the user
    getUVThreshFromLCD(&uv_thresh); 
    time_t time = now();
    // broadcast the current time to receivers
    byte *data = (byte*)&time;
    broadcast(data, sizeof(time_t));
    // send uv threshold to device 2
    data = (byte*)&uv_thresh; 
    Wire.beginTransmission(2);
    Wire.write(data, sizeof(unsigned));
    Wire.endTransmission();
    // send watering schedule (in hours) to device 1
    data = (byte*)&hours;
    Wire.beginTransmission(1);
    Wire.write(data, sizeof(unsigned));
    Wire.endTransmission();
    programState = DISPLAY_TIME;
  }
  else if(programState == DISPLAY_TIME){
    lcd.noBlink();
    char buffer[50];
    char *format = "%02d:%02d:%02d";
    snprintf(buffer, 50, format, hour(), minute(), second());
    lcd.print(buffer);
    delay(200);
    lcd.clear();
  }
}
