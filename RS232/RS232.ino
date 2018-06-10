#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
int TireArray[8];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
   int OutRd,Tire;
    if (mySerial.available()){
    OutRd = mySerial.read();
    if(OutRd == 85){  //如果是讀到85 就開始放值
      TireArray[0]=OutRd;
      for(int i=1; i<8; i++){
          TireArray[i] = mySerial.read();
      }
      delay(17.2);  //測到最好的數值
    }else{  //如果沒讀到就歸零
      /*for(int i=0; i<TireNumber; i++){
          TireArray[i]=0;
      }*/
    }
    }
    for(int i=0; i<8; i++){
      Serial.print(TireArray[i]);
      Serial.print("\t");
    }
    Serial.println(" ");
}
