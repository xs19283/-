#include <MsTimer2.h>

const int INTERVAL = 1;  //0.001秒 = 1ms
const int PrintA = 1000;

void TimeSet(){
  static unsigned int Ts = 0;
  ++Ts;
  if(Ts % PrintA == 0) button();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  MsTimer2::set(INTERVAL, TimeSet);
  MsTimer2::start();
}

void button(){
  Serial.println("BBBBBB");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("aaaaa");
  delay(500);
}

