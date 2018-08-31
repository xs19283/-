#include <Servo.h>
Servo myservo;

#define NUMBER 5
#define TrixNum 7
#define LED 7
#define BUTTONRED 12
#define BUTTONBULE 13
#define SW 8

float ForceArray [NUMBER];
float SortArray [NUMBER];
const int analogInPin = A4;  // 霍爾感測輸入端
int sensorValue = 0;        // 霍爾感測器數值
int Halls;

float TriaxialArray [NUMBER];
float SortTriaxial [NUMBER];
const int TriaxialPin = A3;  // 三軸感測輸入端
int TriaxialValue = 0;        // 三軸感測器數值
int sumALL;

float XAxisArray [NUMBER];
float SortXAxis [NUMBER];
const int xAxis = A2;
int xAxisValue = 0;
int XData;

int Seg = 1;        //為手動調整伺服馬達變數
int red, bule;      //按鈕變數
int OnOff;          //監控是否自動手動變數

boolean pass = true;  //監控藍芽有沒有連上
int mode;             //監控模式

void setup() {
  ////////////////////////////////////
  ////設定開關跟按鈕
  ////////////////////////////////////
  pinMode(BUTTONRED, INPUT);
  pinMode(BUTTONBULE, INPUT);
  pinMode(SW, INPUT_PULLUP);

  Serial.begin(9600);
  myservo.attach(6);

  pinMode(analogInPin, INPUT);
  pinMode(TriaxialPin, INPUT);
  pinMode(xAxis, INPUT);

  ////////////////////////////////////
  ////在程式初始時先放五個霍爾元素進陣列
  ////////////////////////////////////
  for (int i = 0; i < NUMBER; i++) {
    ForceArray[i] = analogRead(analogInPin);
  }
  sensorValue = analogRead(analogInPin);
  ////////////////////////////////////
  ////在程式初始時先放五個X軸元素進陣列
  ////////////////////////////////////
  for (int i = 0; i < NUMBER; i++) {
    XAxisArray[i] = analogRead(xAxis);
  }
  xAxisValue = analogRead(xAxis);
  ////////////////////////////////////
  ////在程式初始時先放五個Z軸元素進陣列
  ////////////////////////////////////
  for (int i = 0; i < NUMBER; i++) {
    TriaxialArray[i] = analogRead(TriaxialPin);
  }
  TriaxialValue = analogRead(TriaxialPin);
  ////////////////////////////////////
  ////藍芽初始化
  ////////////////////////////////////
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);


  Serial.println("OK");
  ////////////////////////////////////
  ////所有設定已完成 LED亮起
  ////////////////////////////////////
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}


////////////////////////////////////
////右移陣列放值進第一元素
////////////////////////////////////
void PutArray1 () {
  for (int i = NUMBER - 1; i > 0; i--) {
    ForceArray[i] = ForceArray[i - 1];
  }
  ForceArray[0] = analogRead(analogInPin);
}

////////////////////////////////////
////右移陣列放值進第一元素
////////////////////////////////////
void PutArray2 () {
  for (int i = NUMBER - 1; i > 0; i--) {
    TriaxialArray[i] = TriaxialArray[i - 1];
  }
  TriaxialArray[0] = analogRead(TriaxialPin);
}

////////////////////////////////////
////右移陣列放值進第一元素
////////////////////////////////////
void PutArray3 () {
  for (int i = NUMBER - 1; i > 0; i--) {
    XAxisArray[i] = XAxisArray[i - 1];
  }
  XAxisArray[0] = analogRead(xAxis);
}

////////////////////////////////////
////陣列查看
////////////////////////////////////
void List() {
  for (int i = 0; i < NUMBER; i++) {
    Serial.print(ForceArray[i], 1);
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
  sum1 = pow(SortArray[1] - SortArray[2], 2);
  sum2 = pow(SortArray[3] - SortArray[2], 2);
  ToTal = sqrt(sum1 + sum2);

  Halls = ToTal;

  /*
    MotorCmd(6);
    if (avg < 20) {
    CalculateByLoad0 ();
    } else if (avg >= 20) {
    CalculateByLoad1 ();
    }
  */
}

////////////////////////////////////
////用平均值判斷(0)
////////////////////////////////////

void CalculateByLoad0 () {
  float avg = Average();
  float value = abs(sensorValue - avg);
  Serial.print("判斷0 : ");
  Serial.println(value);
  if (value > 50) {
    MotorCmd(4);
  } else if (value < 50 && value >= 35) {
    MotorCmd(3);
  } else if (value < 35 && value >= 20) {
    MotorCmd(2);
  }  else {
    MotorCmd(1);
    sensorValue = analogRead(analogInPin);
  }
}

////////////////////////////////////
////用平均值判斷(1)
////////////////////////////////////

void CalculateByLoad1 () {
  float avg = Average();
  float value = abs(sensorValue - avg);
  Serial.print("判斷1 : ");
  Serial.println(value);
  if (avg >= 85) {
    MotorCmd(6);
  } else if (avg < 85 && avg >= 50) {
    MotorCmd(5);
  } else {
    MotorCmd(4);
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
////霍爾陣列排序
////////////////////////////////////
void SortByArray1() {
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
////三軸陣列排序
////////////////////////////////////
void SortByArray2() {
  float temp;

  for (int i = 0; i < NUMBER; i++) {
    SortTriaxial[i] = TriaxialArray[i];
  }

  for (int i = 0; i < NUMBER - 1; i++) {
    for (int j = 0; j < NUMBER - 1; j++) {
      if (SortTriaxial[j + 1] < SortTriaxial[j]) {
        temp = SortTriaxial[j];
        SortTriaxial[j] = SortTriaxial[j + 1];
        SortTriaxial[j + 1] = temp;
      }
    }
  }
}

////////////////////////////////////
////三軸陣列排序
////////////////////////////////////
void SortByArray3() {
  float temp;

  for (int i = 0; i < NUMBER; i++) {
    SortXAxis[i] = XAxisArray[i];
  }

  for (int i = 0; i < NUMBER - 1; i++) {
    for (int j = 0; j < NUMBER - 1; j++) {
      if (SortXAxis[j + 1] < SortXAxis[j]) {
        temp = SortXAxis[j];
        SortXAxis[j] = SortXAxis[j + 1];
        SortXAxis[j + 1] = temp;
      }
    }
  }
}

////////////////////////////////////
////中位數判斷
////////////////////////////////////
void CalculateByMedian1() {

  float median, total, avg;
  median = SortArray[2];
  float value = abs(sensorValue - median);/*
  Serial.print("霍爾:");
  Serial.println(value);*/
  Halls = value;
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
////中位數判斷
////////////////////////////////////
void CalculateByMedian2() {

  float median, total;
  median = SortTriaxial[2];
  float Triavalue = abs(TriaxialValue - median);
  sumALL = Triavalue;

}

////////////////////////////////////
////中位數判斷
////////////////////////////////////
void CalculateByMedian3() {

  float median, total;
  median = SortXAxis[2];
  float Xvalue = abs(xAxisValue - median);
  XData = median;

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
void BlueToothSendData() {
  /*Serial.write(85);
  Serial.write(int(sumALL));
  Serial.write(int(XData));
  Serial.write(int(Halls));
  Serial.write(mode);*/
  Serial.write(85);
  Serial.write(analogRead(xAxis));
  Serial.write(analogRead(TriaxialPin));
  Serial.write(analogRead(analogInPin));
  Serial.write(1);
  
}

////////////////////////////////////
////開關判斷
////////////////////////////////////
void ModeSwitch() {
  if (sumALL < 10) {
    MotorCmd(1);
    mode = 1;
  } else if (sumALL >= 10 && sumALL <= 30) {
    CalculateByMedian1();
    mode = 2;
  } else if (sumALL > 30 && Halls < 10) {
    MotorCmd(6);
    mode = 4;
    delay(500);
  } else if (XData > 560 && sumALL > 5) {
    MotorCmd(2);
    mode = 3;
  }
  if (digitalRead(2) == HIGH && Halls > 10) {
    MotorCmd(2);
    mode = 5;
    delay(2000);
  }
}

////////////////////////////////////
////開關判斷
////////////////////////////////////
void SwitchOnOff() {
  OnOff = digitalRead(SW);
  if (OnOff == HIGH) {
    PutArray1();
    PutArray2();
    PutArray3();
    SortByArray1();
    SortByArray2();
    SortByArray3();
    //VariancePulsByLoad();
    MotorCmd(1);
    /*
    CalculateByMedian2();
    CalculateByMedian3();
    ModeSwitch();
    */
    
    BlueToothSendData();
    
    delay(50);
  } else {
    BuleAndRed();  //如沒開就是按鍵式手動調整
  }
}

//////////////////////////////////////////////////////////////////////涵式部分結束

void loop() {
  SwitchOnOff();
}
