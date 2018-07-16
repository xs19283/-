
#include <Servo.h>

#include <SoftwareSerial.h>
#include <MsTimer2.h>

SoftwareSerial mySerial(10, 11); // RX, TX
Servo myservo; // 建立Servo物件，控制伺服馬達
//Timer T1;

#define NUMBER 5
#define LED 7
#define BUTTONRED 12
#define BUTTONBULE 13
#define SW 8
#define TireNUMBER 8


float ForceArray [NUMBER];
float SortArray [NUMBER];
int TireArray[TireNUMBER];
int TireCount;
int Seg = 1;
int OnOff;
int red, bule;

const int INTERVAL = 27;
const int TireTime = 27;  //每9ms做一次胎壓
const int AdxlTime = 0;
const int Hx711Time = 0;

void setup() {
  ////////////////////////////////////
  ////設定開關跟按鈕
  ////////////////////////////////////
  pinMode(BUTTONRED, INPUT);
  pinMode(BUTTONBULE, INPUT);
  pinMode(SW, INPUT_PULLUP);

  ////////////////////////////////////
  ////設定Arduino鮑率及胎壓感測器鮑率
  ////////////////////////////////////
  Serial.begin(115200);
  mySerial.begin(9600);

  ////////////////////////////////////
  ////設定馬達腳位
  ////////////////////////////////////
  //myservo.attach(6, 500, 2400); // 修正脈衝寬度範圍
  myservo.attach(6);

  ////////////////////////////////////
  ////設定應變規初始設定
  ////////////////////////////////////
  // HX711.DOUT	- pin #A1
  // HX711.PD_SCK	- pin #A0
  

  ////////////////////////////////////
  ////設定胎壓定時中斷每0.9秒中斷一次
  ////////////////////////////////////
  MsTimer2::set(INTERVAL, TirePressure);
  ////////////////////////////////////
  ////在程式開始時亮燈
  ////////////////////////////////////
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  MsTimer2::start();
}
////////////////////////////////////
////主要控制中斷
////////////////////////////////////
void MainTime() {
  static unsigned int St = 1;
  ++St;
  if (St % TireTime == 0) TirePressure();
}




////////////////////////////////////
////馬達控制函式
////////////////////////////////////
void MotorCmd(int angle) {
  switch (angle) {
    case 1:
      myservo.write(0);
      break;
    case 2:
      myservo.write(20);
      break;
    case 3:
      myservo.write(50);
      break;
    case 4:
      myservo.write(75);
      break;
    case 5:
      myservo.write(100);
      break;
    case 6:
      myservo.write(125);
      break;
  }
}



////////////////////////////////////
////胎壓數值
////////////////////////////////////
void TirePressure() {
  
  int out;
  do{
    out = mySerial.read();
  }while(out != 85);
  TireArray[0] = out;
  for(int i = 1; i < 8; i++){
    TireArray[i] = mySerial.read();
  }
  
  TirePrint();
}

////////////////////////////////////
////胎壓數值查看
////////////////////////////////////
void TirePrint() {
  for (int i = 0; i < TireNUMBER; i++) {
    Serial.print(TireArray[i]);
    Serial.print("\t");
  }
  Serial.println("");
}

////////////////////////////////////
////開關判斷
////////////////////////////////////
void SwitchOnOff() {
  OnOff = digitalRead(SW);
  if (OnOff == HIGH) {
    //PutArray();
    //List();
    //delay(100);
    //SortByArray();
    //CalculateByLoad();      ////用平均值判斷
    //CalculateByVariance();  ////標準差判斷
    //TirePressure();

  } else {
  }
}

////////////////////////////////////
////主要程式迴圈
////////////////////////////////////
void loop() {
  SwitchOnOff();
}
/*
  byte c[1];
  int ctrl;
  if(Serial.available())
  {  //如果有接到資料 讀取字元並放入cmd字串 並轉為整數設定伺服馬達轉向
    Serial.readBytes(c,1);
    ctrl = atoi(c);
    Serial.println(ctrl);
  }

  //SortArray陣列查看
  Serial.print("SortArray \t");
  for (int i=0; i<NUMBER; i++){
    Serial.print("\t");
    Serial.print(SortArray[i], 1);
  }
  Serial.println("\t");

  //TireArray陣列查看
  Serial.print("TireArray \t");
  for (int i=0; i<TireNumber; i++){
    Serial.print("\t");
    Serial.print(TireArray[i], 1);
  }
  Serial.println("\t");
*/

