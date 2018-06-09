#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int rd;
  
  if (mySerial.available()){
    rd=mySerial.read();
    Serial.print(rd);
    Serial.print(" ");
  }
  Serial.println(" ");
}
