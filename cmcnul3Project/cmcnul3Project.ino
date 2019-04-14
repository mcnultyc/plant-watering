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
  lcd.print("00:00:00");
  int tens_place = 0, ones_place = 0;
  int i = 0;
  while(i < 2){
    lcd.setCursor(i, 0);
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      i++;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        if(i == 0 && tens_place + 1 <= 2){
          tens_place++;
          lcd.write('0' + tens_place);
        }
        if(i == 1 && (ones_place + 1) <= 9
                  && (tens_place * 10 + (ones_place + 1)) <= 23){
            ones_place++;
            lcd.write('0' + ones_place);
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        if(i == 0 && tens_place - 1 >= 0){
          tens_place--;
          lcd.write('0' + tens_place);
        }
        if(i == 1 && ones_place - 1 >= 0){
          ones_place--;
          lcd.write('0' + ones_place);
        }
      }
    }
  }
  *hour = tens_place * 10 + ones_place; 
  tens_place = 0;
  ones_place = 0;
  i++;
  while(i < 5){
    lcd.setCursor(i, 0);
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      i++;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        if(i == 3 && tens_place + 1  <= 5){
          tens_place++;
          lcd.write('0' + tens_place);
        }
        if(i == 4 && ones_place + 1 <= 9){
          ones_place++;
          lcd.write('0' + ones_place);
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        if(i == 3 && tens_place - 1 >= 0){
          tens_place--;
          lcd.write('0' + tens_place);
        }
        if(i == 4 && ones_place - 1 >= 0){
          ones_place--;
          lcd.write('0' + ones_place);
        }
      }
    }
  }
  *minutes = tens_place * 10 + ones_place; 
  tens_place = 0;
  ones_place = 0;
  i++; 
  while(i < 8){
    lcd.setCursor(i, 0);
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      i++;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        if(i == 6 && tens_place + 1  <= 5){
          tens_place++;
          lcd.write('0' + tens_place);
        }
        if(i == 7 && ones_place + 1 <= 9){
          ones_place++;
          lcd.write('0' + ones_place);
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        if(i == 6 && tens_place - 1 >= 0){
          tens_place--;
          lcd.write('0' + tens_place);
        }
        if(i == 7 && ones_place - 1 >= 0){
          ones_place--;
          lcd.write('0' + ones_place);
        }
      }
    }
  }
  *seconds = tens_place * 10 + ones_place;
}

void getDateFromLCD(int *month, int *day, int *year){
  if(!month || !day || !year){ return; }
  lcd.clear();
  lcd.setCursor(0,0);
  
  lcd.setCursor(0,1);
  lcd.print("00.00.00");
  int tens_place = 0, ones_place = 0;
  int i = 0;
  while(i < 2){
    lcd.setCursor(i, 1);
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      i++;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        if(i == 0 && tens_place + 1 <= 1){
          tens_place++;
          lcd.write('0' + tens_place);
        }
        if(i == 1 && (ones_place + 1) <= 9
                  && (tens_place * 10 + (ones_place + 1)) <= 12){
            ones_place++;
            lcd.write('0' + ones_place);
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        if(i == 0 && tens_place - 1 >= 0){
          tens_place--;
          lcd.write('0' + tens_place);
        }
        if(i == 1 && ones_place - 1 >= 0){
          ones_place--;
          lcd.write('0' + ones_place);
        }
      }
    }
  }
  *month = tens_place * 10 + ones_place; 
  tens_place = 0;
  ones_place = 0;
  i++;
  while(i < 5){
    lcd.setCursor(i, 1);
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      i++;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        if(i == 3 && tens_place + 1  <= 3){
          tens_place++;
          lcd.write('0' + tens_place);
        }
        if(i == 4 && (tens_place * 10 + (ones_place + 1)) <= 31){
          ones_place++;
          lcd.write('0' + ones_place);
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        if(i == 3 && tens_place - 1 >= 0){
          tens_place--;
          lcd.write('0' + tens_place);
        }
        if(i == 4 && ones_place - 1 >= 0){
          ones_place--;
          lcd.write('0' + ones_place);
        }
      }
    }
  }
  *day = tens_place * 10 + ones_place; 
  tens_place = 0;
  ones_place = 0;
  i++; 
  while(i < 8){
    lcd.setCursor(i, 1);
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      i++;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        if(i == 6 && tens_place + 1  <= 9){
          tens_place++;
          lcd.write('0' + tens_place);
        }
        if(i == 7 && ones_place + 1 <= 9){
          ones_place++;
          lcd.write('0' + ones_place);
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        if(i == 6 && tens_place - 1 >= 0){
          tens_place--;
          lcd.write('0' + tens_place);
        }
        if(i == 7 && ones_place - 1 >= 0){
          ones_place--;
          lcd.write('0' + ones_place);
        }
      }
    }
  }
  *year = tens_place * 10 + ones_place;
}

void getScheduleFromLCD(int *days, int *hours){
  if(!days || !hours){ return; }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter schedule: 
  lcd.setCursor(0,1);
  lcd.print("00:00");
  int tens_place = 0, ones_place = 0;
  int i = 0;
  while(i < 2){
    lcd.setCursor(i, 0);
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      i++;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        if(i == 0 && tens_place + 1 <= 3){
          tens_place++;
          lcd.write('0' + tens_place);
        }
        if(i == 1 && (ones_place + 1) <= 9
                  && (tens_place * 10 + (ones_place + 1)) <= 31){
            ones_place++;
            lcd.write('0' + ones_place);
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        if(i == 0 && tens_place - 1 >= 0){
          tens_place--;
          lcd.write('0' + tens_place);
        }
        if(i == 1 && ones_place - 1 >= 0){
          ones_place--;
          lcd.write('0' + ones_place);
        }
      }
    }
  }
  *days = tens_place * 10 + ones_place; 
  tens_place = 0;
  ones_place = 0;
  i++;
  while(i < 5){
    lcd.setCursor(i, 0);
    if(buttonPressed(enterButtonPin, &enterButtonState)){
      i++;
    }
    else{
      if(buttonPressed(upButtonPin, &upButtonState)){
        if(i == 3 && tens_place + 1  <= 2){
          tens_place++;
          lcd.write('0' + tens_place);
        }
        if(i == 4 && ones_place + 1 <= 9
                  && (tens_place * 10 + (ones_place + 1)) <= 23){
          ones_place++;
          lcd.write('0' + ones_place);
        }
      }
      if(buttonPressed(downButtonPin, &downButtonState)){
        if(i == 3 && tens_place - 1 >= 0){
          tens_place--;
          lcd.write('0' + tens_place);
        }
        if(i == 4 && ones_place - 1 >= 0){
          ones_place--;
          lcd.write('0' + ones_place);
        }
      }
    }
  }
  *hours = tens_place * 10 + ones_place;
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
