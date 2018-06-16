#include <SoftwareSerial.h>
#include "Timer.h"

SoftwareSerial mySerial(2, 3);
int i;
int TireArray[8];
const int LED1 = 8;                    //connect one LED to this pin (with appropriate current-limiting resistor of course)
const int LED2 = 9;                    //connect another LED to this pin (don't forget the resistor)
const unsigned long PERIOD1 = 1;    //one second
const unsigned long PERIOD2 = 10000;   //ten seconds
Timer t;                               //instantiate the timer object

void TireEvent(void *context) {
  int in1;
  while (mySerial.available() > 0) {
    in1 = mySerial.read();
    if (in1 == 85) {
      TireArray[0] = in1;
    } else {
      TireArray[i] = in1;
    }
    i++;
    if (i == 8) {
      i = 0;
    }
  }
}

void setup(void)
{
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  int tickevent = t.every(1, TireEvent, (void*)0);
  Serial.println(tickevent);
}

void loop(void)
{
  t.update();
  
}

