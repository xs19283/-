#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);  //Rx Tx

volatile int flag = 1;
const int myTOP = 224;
int TireArray[8];

ISR(TIMER1_COMPA_vect)
{
  TirePressure();
  flag = 1 - flag;
}

void TirePressure() {
  int out;
  do {
    out = mySerial.read();
    Serial.println(out);
  } while (out != 85);
  TireArray[0] = out;
  for (int i = 1; i < 8; i++) {
    TireArray[i] = mySerial.read();
    delayMicroseconds(200);
  }
  TirePrint();
}

void TirePrint()
{
 for(int i=0;i<8;i++)
 {
   Serial.print(TireArray[i]);
   Serial.print("\t");
 }
 Serial.println();
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  cli();  //禁止中斷
  TCCR1A = 0;
  TCCR1B = (1 << WGM12);
  TCCR1B |= (1 << CS10) | (1 << CS11);

  OCR1A = myTOP;
  TCNT1 = 0;
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

void loop()
{
  // put your main code here, to run repeatedly:

}
