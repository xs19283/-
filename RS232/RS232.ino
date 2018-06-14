#include <SoftwareSerial.h>
#include <TimedAction.h>

#define TireTime 19.2
int TireArray[8];
void mySerialEvent(void);
void TirePrint(void);

SoftwareSerial mySerial(0, 1); // RX, TX
TimedAction mySerialAction = TimedAction(TireTime, mySerialEvent);
TimedAction mySerialprint = TimedAction(TireTime, TirePrint);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  mySerial.begin(9600);

}

void mySerialEvent() {
  int i;
  if ((TireArray[0] = mySerial.read()) == 85) {
    for (i = 1; i < 8; i++) {
      TireArray[i] = mySerial.read();
    }
  }
  
}

void TirePrint() {
  for (int i = 0; i < 8; i++) {
    Serial.print(TireArray[i]);
    Serial.print("\t");
  }
  Serial.println("");
}

void loop() {
  mySerialAction.check();
  mySerialprint.check();
}

/*int OutRd;
    if (mySerial.available()){
    OutRd = mySerial.read();
    if(OutRd == 85){  //如果是讀到85 就開始放值
      TireArray[0]=OutRd;
      for(int i=1; i<8; i++){
          TireArray[i] = mySerial.read();
      }
      delay(10.2);  //測到最好的數值
    }else{  //如果沒讀到就歸零
      for(int i=0; i<TireNUMBER; i++){
          TireArray[i]=0;
      }
    }
    }
    for(int i=0; i<TireNUMBER; i++){
      Serial.print(TireArray[i]);
      Serial.print("\t");
    }
    Serial.println(" "); */
