#include "HX711.h"
#include <Servo.h>
#include <Timer.h>
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

HX711 scale;
float ForceArray [NUMBER];
float SortArray [NUMBER];
int TireArray[TireNUMBER];
int TireCount;
int Seg = 1;
int OnOff;
int red, bule;

const int INTERVAL = 9;
const int TireTime = 9;  //每9ms做一次胎壓
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
  scale.begin(A1, A0);

  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

  ////////////////////////////////////
  ////設定胎壓定時中斷每0.9秒中斷一次
  ////////////////////////////////////
  MsTimer2::set(INTERVAL, TirePressure);
  ////////////////////////////////////
  ////在程式初始時先放五個元素進陣列
  ////////////////////////////////////
  for (int i = 0; i < NUMBER; i++) {
    ForceArray[i] = abs(scale.get_units());
  }
  ////////////////////////////////////
  ////在程式開始時亮燈
  ////////////////////////////////////
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  scale.power_down();
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
////右移陣列放值進第一元素
////////////////////////////////////
void PutArray () {
  scale.power_up();
  for (int i = NUMBER - 1; i > 0; i--) {
    ForceArray[i] = ForceArray[i - 1];
  }
  ForceArray[0] = abs(scale.get_units());
  scale.power_down();
}

////////////////////////////////////
////陣列查看
////////////////////////////////////
void List() {
  for (int i = 0; i < NUMBER; i++) {
    Serial.print(ForceArray[i], 1);
    Serial.print("\t");
  }
  Serial.print("Average:");
  Serial.println(Average());
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
////平均值
////////////////////////////////////
float Average() {
  float sum = 0;
  for (int i = 0; i < NUMBER; i++) {
    sum += ForceArray[i];
  }
  return abs(sum / NUMBER);
}

////////////////////////////////////
////用平均值判斷
////////////////////////////////////
void CalculateByLoad () {
  float avg = Average();
  if (avg > 100) {
    MotorCmd(6);
  } else if (avg < 100 && avg >= 80) {
    MotorCmd(5);
  } else if (avg < 80 && avg >= 60) {
    MotorCmd(4);
  } else if (avg < 60 && avg >= 40) {
    MotorCmd(3);
  } else if (avg < 40 && avg >= 20) {
    MotorCmd(2);
  } else {
    MotorCmd(1);
  }
}
////////////////////////////////////
////用平均值判斷(0)
////////////////////////////////////
void CalculateByLoad0 () {
  float avg = Average();
  if (avg > 60) {
    MotorCmd(4);
  } else if (avg < 60 && avg >= 30) {
    MotorCmd(3);
  } else if (avg < 30) {
    MotorCmd(2);
  }  else {
    MotorCmd(2);
  }
}
////////////////////////////////////
////用平均值判斷(1)
////////////////////////////////////
void CalculateByLoad1 () {
  float avg = Average();
  if (avg >= 85) {
    MotorCmd(6);
  } else if (avg < 85 && avg >= 60) {
    MotorCmd(5);
  } else {
    MotorCmd(4);
  }
}
////////////////////////////////////
////標準差加平均值
////////////////////////////////////
void VariancePulsByLoad () {
  float sum1, sum2, total, avg;
  sum1 = pow(SortArray[1] - SortArray[2], 2);
  sum2 = pow(SortArray[3] - SortArray[2], 2);
  total = sqrt(sum1 + sum2);
  Serial.println(total);

  avg = total;
  if (avg < 10) {
    CalculateByLoad0 ();
  } else if (avg >= 10) {
    CalculateByLoad1 ();
  }
}
////////////////////////////////////
////標準差判斷
////////////////////////////////////
void CalculateByVariance () {
  float sum1, sum2, total, avg;
  sum1 = pow(SortArray[1] - SortArray[2], 2);
  sum2 = pow(SortArray[3] - SortArray[2], 2);
  total = sqrt(sum1 + sum2);
  Serial.println(total);

  avg = total;
  if (avg > 21) {
    MotorCmd(6);
  } else if (avg < 21 && avg >= 17) {
    MotorCmd(5);
  } else if (avg < 17 && avg >= 13) {
    MotorCmd(4);
  } else if (avg < 13 && avg >= 9) {
    MotorCmd(3);
  } else if (avg < 9 && avg >= 4) {
    MotorCmd(2);
  } else {
    MotorCmd(1);
  }
}

////////////////////////////////////
////中位數判斷
////////////////////////////////////
void CalculateByMedian() {
  float median, avg;
  median = SortArray[2];
  Serial.println(median, 1);

  avg = median;
  if (avg > 110) {
    MotorCmd(6);
  } else if (avg < 110 && avg >= 90) {
    MotorCmd(5);
  } else if (avg < 90 && avg >= 60) {
    MotorCmd(4);
  } else if (avg < 60 && avg >= 40) {
    MotorCmd(3);
  } else if (avg < 40 && avg >= 20) {
    MotorCmd(2);
  } else {
    MotorCmd(1);
  }

}

////////////////////////////////////
////陣列排序
////////////////////////////////////
void SortByArray() {
  float temp;

  for (int i = 0; i < NUMBER; i++) {
    SortArray[i] = ForceArray[i];
  }

  for (int i = 0; i < NUMBER - 1; i++) {
    for (int j = 0; j < NUMBER - 1; j++) {
      if (SortArray[j + 1] < SortArray[j]) {
        temp = SortArray[j];
        SortArray[j] = SortArray[j + 1];
        SortArray[j + 1] = temp;
      }
    }
  }
}

////////////////////////////////////
////按鈕控制馬達
////////////////////////////////////
void BuleAndRed() {

  red = digitalRead(BUTTONRED);
  bule = digitalRead(BUTTONBULE);
  if (red == HIGH && Seg < 6) {
    Seg++;
    MotorCmd(Seg);
    delay(250);
  } else if (bule == HIGH && Seg > 1) {
    Seg--;
    MotorCmd(Seg);
    delay(250);
  }
}

////////////////////////////////////
////胎壓數值
////////////////////////////////////
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
  mySerial.end();
  TirePrint();
  mySerial.begin(9600);
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
    //SortByArray();    1
    //CalculateByLoad();      ////用平均值判斷
    //CalculateByVariance();  ////標準差判斷
    //CalculateByMedian();   ////中位數判斷
    //VariancePulsByLoad();   ////平均值與標準差判斷   1
    //TirePressure();

  } else {
    BuleAndRed();  //如沒開就是按鍵式手動調整
  }
}

////////////////////////////////////
////主要程式迴圈
////////////////////////////////////
void loop() {
  //SwitchOnOff();
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

