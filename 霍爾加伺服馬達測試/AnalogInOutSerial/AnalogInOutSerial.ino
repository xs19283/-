#include <Servo.h>
Servo myservo;

#define NUMBER 5
#define LED 7
#define BUTTONRED 12
#define BUTTONBULE 13
#define SW 8

float HallArray [NUMBER];
float HallSortArray [NUMBER];
const int HallPin = A4;          // 霍爾感測輸入端
int HallStartValue = 0;          // 霍爾感測器數值
int SendHall;

float ZAxisArray [NUMBER];
float ZSortArray [NUMBER];
const int ZPin = A3;             // Z軸感測輸入端
int ZStartValue = 0;             // Z軸感測器數值
int SendZ;

float XAxisArray [NUMBER];
float XSortArray [NUMBER];
const int XPin = A2;
int XStartValue = 0;
int SendX;

int Seg = 1;        //為手動調整伺服馬達變數

int red, bule;      //按鈕變數
int OnOff;          //監控是否自動手動變數

int NowMode;        //監控模式



void setup() {
  ////////////////////////////////////
  ////設定開關跟按鈕
  ////////////////////////////////////
  pinMode(BUTTONRED, INPUT);
  pinMode(BUTTONBULE, INPUT);
  pinMode(SW, INPUT_PULLUP);

  Serial.begin(9600);
  myservo.attach(6);

  pinMode(HallPin, INPUT);
  pinMode(ZPin, INPUT);
  pinMode(XPin, INPUT);

  ////////////////////////////////////
  ////在程式初始時先放五個 Hall&X&Z 元素進陣列
  ////////////////////////////////////
  PutValueArray(HallArray, HallStartValue, HallPin);
  PutValueArray(XAxisArray, XStartValue, XPin);
  PutValueArray(ZAxisArray, ZStartValue, ZPin);
  ////////////////////////////////////
  ////藍芽初始化
  ////////////////////////////////////
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  ////////////////////////////////////
  ////所有設定已完成 LED亮起
  ////////////////////////////////////
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}

void PutValueArray(float Array[], int StartValue, int Pin){
  for (int i = 0; i < NUMBER; i++) {
    Array[i] = analogRead(Pin);
  }
  StartValue = analogRead(Pin);
}

////////////////////////////////////
////右移陣列放值進第一元素
////////////////////////////////////
void ShiftRightArray(float Array[], int Pin) {
  for (int i = NUMBER - 1; i > 0; i--) {
    Array[i] = Array[i - 1];
  }
  Array[0] = analogRead(Pin);
}

////////////////////////////////////
////陣列查看
////////////////////////////////////
void List() {
  for (int i = 0; i < NUMBER; i++) {
    Serial.print(HallArray[i], 1);
    Serial.print("\t");
  }
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
      myservo.write(25);
      break;
    case 3:
      myservo.write(50);
      break;
    case 4:
      myservo.write(80);
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
////標準差加平均值
////////////////////////////////////
void VariancePulsByLoad () {
  float sum1, sum2, ToTal;

  sum1 = pow(HallSortArray[1] - HallSortArray[2], 2);
  sum2 = pow(HallSortArray[3] - HallSortArray[2], 2);
  ToTal = sqrt(sum1 + sum2);
  SendHall = ToTal;
}

////////////////////////////////////
////陣列排序涵式
////////////////////////////////////
void SortByArray(float SortArray[], float Array[]) {
  float temp;

  for (int i = 0; i < NUMBER; i++) {
    SortArray[i] = Array[i];
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
////霍爾中位數判斷
////////////////////////////////////
void CalculateHallByMedian() {

  float Median, avg;
  Median = HallSortArray[2];
  float value = abs(HallStartValue - Median);
  SendHall = value;
  avg = value;


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
  } else if (avg < 20) {
    MotorCmd(1);
  }
  delay(100);
}

////////////////////////////////////
////計算中位數
////////////////////////////////////
void CalculateByMedian(float SortArray[], int StartValue, int Send) {

  float Median, total;
  Median = SortArray[2];
  float CalculateValue = abs(StartValue - Median);
  Send = CalculateValue;

}

////////////////////////////////////
////按鈕控制馬達
////////////////////////////////////
void BlueAndRed() {

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
////藍芽傳值涵式
////////////////////////////////////
void BluetoothSendData() {
  /*Serial.write(85);
    Serial.write(int(SendZ));
    Serial.write(int(SendX));
    Serial.write(int(SendHall));
    Serial.write(NowMode);*/

  Serial.write(85);
  Serial.write(analogRead(XPin));
  Serial.write(analogRead(ZPin));
  Serial.write(analogRead(HallPin));
  Serial.write(1);

}

////////////////////////////////////
////模式選擇
////////////////////////////////////
void NowModeSwitch() {
  if (SendZ < 10) {
    MotorCmd(1);
    NowMode = 1;
  } else if (SendZ >= 10 && SendZ <= 30) {
    CalculateHallByMedian();
    NowMode = 2;
  } else if (SendZ > 30 && SendHall < 10) {
    MotorCmd(6);
    NowMode = 4;
    delay(500);
  } else if (SendX > 560 && SendZ > 5) {
    MotorCmd(2);
    NowMode = 3;
  }
  if (digitalRead(2) == HIGH && SendHall > 10) {
    MotorCmd(2);
    NowMode = 5;
    delay(2000);
  }
}

////////////////////////////////////
////開關判斷
////////////////////////////////////
void SwitchOnOff() {
  OnOff = digitalRead(SW);
  if (OnOff == HIGH) {
    ShiftRightArray(HallArray, HallPin);
    ShiftRightArray(ZAxisArray, ZPin);
    ShiftRightArray(XAxisArray, XPin);
    SortByArray(HallSortArray, HallArray);
    SortByArray(ZSortArray, ZAxisArray);
    SortByArray(XSortArray, XAxisArray);
    //VariancePulsByLoad();
    /*
        CalculateByMedian(ZSortArray, ZStartValue, SendZ);
        CalculateByMedian(XSortArray, XStartValue, SendX);
        NowModeSwitch();
    */

    NowModeSwitch();
    BluetoothSendData();

    delay(50);
  } else {
    BlueAndRed();  //如沒開就是按鍵式手動調整
  }
}

//////////////////////////////////////////////////////////////////////涵式部分結束

void loop() {
  SwitchOnOff();
}
