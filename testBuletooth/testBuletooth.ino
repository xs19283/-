#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // RX | TX
boolean pass = true;
void setup()
{
  pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(9, HIGH);
  Serial.begin(9600);
  Serial.println("Enter AT commands:");
  BTSerial.begin(9600);  // HC-05 default speed in AT command more
  
  do{
    if (BTSerial.available()){
      byte Check = BTSerial.read();
      if(Check == 79){
        Serial.write(Check);
        pass = false;
      }else{
        Serial.println("沒有連上");
      }
    }
  }while(pass);
  
}
void loop()
{
  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTSerial.available())
    Serial.write(BTSerial.read());
  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available()){
    BTSerial.write(85);
    BTSerial.write(20 );
    BTSerial.write(50 );
    BTSerial.write(65 );
  }
}
