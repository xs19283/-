#include "HX711.h"
#include <Servo.h>

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
int Seg = 1;
int OnOff;
int red, bule;

void setup() {
  ////////////////////////////////////
  ////設定開關跟按鈕
  ////////////////////////////////////
  pinMode(BUTTONRED, INPUT);
  pinMode(BUTTONBULE, INPUT);
  pinMode(SW, INPUT_PULLUP);

  ////////////////////////////////////
  ////設定Arduino鮑率
  ////////////////////////////////////
  Serial.begin(9600);

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
}

////////////////////////////////////
////右移陣列放值進第一元素
////////////////////////////////////
void PutArray () {
  for (int i = NUMBER - 1; i > 0; i--) {
    ForceArray[i] = ForceArray[i - 1];
  }
  ForceArray[0] = abs(scale.get_units());
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
////開關判斷
////////////////////////////////////
void SwitchOnOff() {
  OnOff = digitalRead(SW);
  if (OnOff == HIGH) {
    PutArray();
    SortByArray();
    //CalculateByLoad();      ////用平均值判斷
    //CalculateByVariance();  ////標準差判斷
    //CalculateByMedian();   ////中位數判斷
    VariancePulsByLoad();   ////平均值與標準差判斷
  } else {
    BuleAndRed();
  }
}

void loop() {
  SwitchOnOff();
}
