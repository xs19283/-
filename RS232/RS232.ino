#include <SoftwareSerial.h>



int TireArray[8];
void mySerialEvent(void);
void TirePrint(void);
int i;
SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  mySerial.begin(9600);

}

void mySerialEvent() {


}
void TirePrint() {
  for (int i = 0; i < 8; i++) {
    Serial.print(TireArray[i]);
    Serial.print("\t");
  }
  Serial.println("");
}

void loop() {
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
   TirePrint();
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
