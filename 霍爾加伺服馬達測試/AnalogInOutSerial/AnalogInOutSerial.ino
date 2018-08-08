#include <Servo.h>
Servo myservo;

#define NUMBER 5

float ForceArray [NUMBER];
float SortArray [NUMBER];
const int analogInPin = A0;  // 霍爾感測輸入端
int sensorValue = 0;        // 霍爾感測器數值

void setup() {
  Serial.begin(9600);
  myservo.attach(6);

  ////////////////////////////////////
  ////在程式初始時先放五個元素進陣列
  ////////////////////////////////////
  for (int i = 0; i < NUMBER; i++) {
    ForceArray[i] = analogRead(analogInPin);
  }

  sensorValue = analogRead(analogInPin);
}

//////////////////////////////////////////////////////////////////////涵式部分

////////////////////////////////////
////右移陣列放值進第一元素
////////////////////////////////////
void PutArray () {
  for (int i = NUMBER - 1; i > 0; i--) {
    ForceArray[i] = ForceArray[i - 1];
  }
  ForceArray[0] = analogRead(analogInPin);
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
      myservo.write(60);
      break;
    case 2:
      myservo.write(80);
      break;
    case 3:
      myservo.write(100);
      break;
    case 4:
      myservo.write(125);
      break;
    case 5:
      myservo.write(145);
      break;
    case 6:
      myservo.write(170);
      break;
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
  MotorCmd(6);
  if (avg < 20) {
    CalculateByLoad0 ();
  } else if (avg >= 20) {
    CalculateByLoad1 ();
  }

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
////中位數判斷
////////////////////////////////////
void CalculateByMedian() {

  float median, avg;
  median = SortArray[2];
  Serial.println(median, 1);
  float value = abs(sensorValue - median);
  Serial.println(value);
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
  } else {
    MotorCmd(1);
  }
}

//////////////////////////////////////////////////////////////////////涵式部分結束

void loop() {
  PutArray();
  SortByArray();
  CalculateByMedian();

  delay(100);
}


