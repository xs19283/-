
#include <Servo.h>

#include <SoftwareSerial.h>
#include <MsTimer2.h>

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

const int INTERVAL = 1;
const int TireTime = 11;  //每9ms做一次胎壓
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
  Serial.begin(9600);
  //mySerial.begin(9600);

  ////////////////////////////////////
  ////設定馬達腳位
  ////////////////////////////////////
  //myservo.attach(6, 500, 2400); // 修正脈衝寬度範圍
  //myservo.attach(6);

  ////////////////////////////////////
  ////設定應變規初始設定
  ////////////////////////////////////
  // HX711.DOUT	- pin #A1
  // HX711.PD_SCK	- pin #A0
  

  ////////////////////////////////////
  ////設定胎壓定時中斷每0.9秒中斷一次
  ////////////////////////////////////
  MsTimer2::set(INTERVAL, MainTime);
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
////胎壓數值
////////////////////////////////////
void TirePressure() {
  
  int out;
  do{
    out = Serial.read();
    if(out == -1){
      break;
    }else if(out == 85){
      TireArray[0] = out;
    }
  }while(out != 85);
  
  for(int i = 1; i < 8; i++){
    int in = Serial.read();
    if(in != -1){
      TireArray[i] = in; 
    }else{
      break;
    }
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


