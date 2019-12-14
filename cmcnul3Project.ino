
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

/* This function checks whether a button was pressed
 * and will only return true when pressed, but not
 * while being pressed.
 */
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

/* This function uses an lcd and two buttons as a user interface
 * to get the current time from the user.
 */
void getTimeFromLCD(int *hour, int *minute, int *second){
  if(!hour || !minute || !second){ return; }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("00:00:00");
  *hour = 0;
  *minute = 0;
  *second = 0;
  char buffer[50];
  int complete = 0;
  // get the hours from the user
  while(!complete){
    lcd.setCursor(0,0);
    char *format = "%02d:%02d:%02d";
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      complete = 1;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        (*hour)++;
        // hours wrap back around
        if(*hour > 23){
          *hour = 0;
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        (*hour)--;
        if(*hour < 0){
          *hour = 23;
        }
      }
    }
    snprintf(buffer, 50, format, *hour, *minute, *second);
    lcd.print(buffer);
  }
  complete = 0;
  // get the minutes from the user
  while(!complete){
    lcd.setCursor(3,0);
    // drop the hour portion of the format for current lcd cursor position
    char *format = "%02d:%02d";
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      complete = 1;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        (*minute)++;
        // minutes wrap back around
        if(*minute >= 59){
          *minute = 0;
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        (*minute)--;
        if(*minute < 0){
          *minute = 59;
        }
      }
    }
    snprintf(buffer, 50, format, *minute, *second);
    lcd.print(buffer);
  }
  complete = 0;
  // get seconds from the user
  while(!complete){
    lcd.setCursor(6,0);
    // drop hour and minute portion of format for currrent lcd cursor position
    char *format = "%02d";
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      complete = 1;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        (*second)++;
        // seconds wrap back around
        if(*second > 59){
          *second = 0;
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        (*second)--;
        if(*second < 0){
          *second = 59;
        }
      }
    }
    snprintf(buffer, 50, format, *second);
    lcd.print(buffer);
  }
}

int validDate(int month, int day, int year, int *max){
  switch(month){
    case 1: case 3: case 5: case 7:
    case 8: case 10: case 12:{
      // set the max day of the month
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
      if(year%400 == 0 || (year%4 == 0 && year%100 != 0)){
        // set leap year max
        if(max){ *max = 29; }
        if(day >= 1 && day <= 29){
          return 1;
        }
      }
      else{
        // default is no leap year
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

/* This functions uses an lcd and two buttons as a user interface
 * to get the current date from the user.
 */
void getDateFromLCD(int *month, int *day, int *year){
  if(!month || !day || !year){ return; }
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("00.00.00");
  // set default date to this year
  *month = 4;
  *day = 15;
  *year = 18;
  char buffer[50];
  int complete = 0;
  // get the year from the user
  while(!complete){
    lcd.setCursor(0,0);
    char *format = "%02d.%02d.%02d";
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      complete = 1;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        (*year)++;
        // years wrap back around to 0 - 99
        if(*year > 99){
          *year = 0;
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        (*year)--;
        if(*year < 0){
          *year = 99;
        }
      }
    }
    snprintf(buffer, 50, format, *year, *month, *day);
    lcd.print(buffer);
  }
  complete = 0;
  // get the month from the user
  while(!complete){
    lcd.setCursor(3,0);
    // year portion of format dropped for new lcd cursor position
    char *format = "%02d.%02d";
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      complete = 1;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        (*month)++;
        // months wrap back around
        if(*month > 12){
          *month = 1;
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        (*month)--;
        if(*month < 1){
          *month = 12;
        }
      }
    }
    snprintf(buffer, 50, format, *month, *day);
    lcd.print(buffer);
  }
  complete = 0;
  // get the day from the user
  while(!complete){
    lcd.setCursor(6, 0);
    // year and month dropped from format
    char *format = "%02d";
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      complete = 1;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        (*day)++;
        // days wrap back around to 1
        if(!validDate(*month, *day, *year + 2000, NULL)){
          *day = 1;
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        int max;
        (*day)--;
        // days wrap back around to the max day for the given
        // year and month (includes leap years)
        if(!validDate(*month, *day, *year + 2000, &max)){
          *day = max;
        }
      }
    }
    snprintf(buffer, 50, format, *day);
    lcd.print(buffer);
  }
}

/* This function uses an lcd and two buttons to get the 
 * watering schedule from the user.
 */
void getScheduleFromLCD(int *days, int *hours, int *minutes){
  if(!days || !hours){ return; }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("00::00:00");
  *days = 0;
  *hours = 0;
  *minutes = 0;
  char buffer[50];
  int complete = 0;
  // get the days from the user
  while(!complete){
    lcd.setCursor(0,0);
    char *format = "%02d::%02d:%02d";
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      complete = 1;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        (*days)++;
        // days wrap back around 0 - 99
        if(*days > 99){
          *days = 0;
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        (*days)--;
        if(*days < 0){
          *days = 99;
        }
      }
    }
    snprintf(buffer, 50, format, *days, *hours, *minutes);
    lcd.print(buffer);
  }
  complete = 0;
  while(!complete){
    lcd.setCursor(4, 0);
    // drop the days portion of the format for current lcd cursor position
    char *format = "%02d:%02d";
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      complete = 1;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        (*hours)++;
        // hours wrap back around 0 - 99
        if(*hours > 99){
          *hours = 0;
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        (*hours)--;
        if(*hours < 0){
          *hours = 99;
        }
      }
    }
    snprintf(buffer, 50, format, *hours, *minutes);
    lcd.print(buffer);
  }
  complete = 0;
  while (!complete){
    lcd.setCursor(7, 0);
    // drop days and hours portion of format for current lcd position
    char *format = "%02d";
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      complete = 1;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        (*minutes)++;
        // minutes wrap back around 0 - 59
        if(*minutes > 59){
          *minutes = 0;
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        (*minutes)--;
        if(*minutes < 0){
          *minutes = 59;
        }
      }
    }
    snprintf(buffer, 50, format, *minutes);
    lcd.print(buffer);
  }
}

/* This function uses an lcd and two buttons as a user interface
 * to get the uv threshold from the user. This uv sensor threshold
 * is the point at which the uv lights are activated. If no value
 * is set then a default threshold is used.
 */
void getUVThreshFromLCD(unsigned *uv_thresh, int uv_max){
  if(!uv_thresh){ return; }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("00");
  *uv_thresh = 0;
  char buffer[50];
  int complete = 0;
  // get the uv sensor threshold from user
  while(!complete){
    lcd.setCursor(0,0);
    char *format = "%02d";
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      complete = 1;
    }
    if(buttonPressed(upButtonPin, &upButtonState)){
      (*uv_thresh)++;
      // uv threshold wraps back around 0 - uv max
      if(*uv_thresh > uv_max){
        *uv_thresh = 0;
      }
    }
    if(buttonPressed(downButtonPin, &downButtonState)){
      (*uv_thresh)--;
      if(*uv_thresh < 0){
        *uv_thresh = uv_max;
      }  
    }
    snprintf(buffer, 50, format, *uv_thresh);
    lcd.print(buffer);
  }
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
    char buffer[50];
    unsigned month, day, year;
    unsigned hour, minute, second;
    unsigned uv_thresh;
    // get the date and time from the user
    getDateFromLCD(&month, &day, &year);
    getTimeFromLCD(&hour, &minute, &second);
    setTime(hour, minute, second, day, month, year);
    unsigned days, hours, minutes;
    // get the watering schedule (in hours) from the user
    getScheduleFromLCD(&days, &hours, &minutes);
    hours = days * 24 + hours + minutes / 60;
    // get the uv threshold from the user
    getUVThreshFromLCD(&uv_thresh, 30); 
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
