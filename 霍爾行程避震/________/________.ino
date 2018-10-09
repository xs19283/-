//////////////////////庫//////////////////////
#include <Servo.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "HX711.h"

//////////////////////建構物件//////////////////////
Servo myservo;
MPU6050 mpu;
HX711 scale;

//////////////////////五大模式變數//////////////////////
volatile int NowMode;
int CtrlGoStop;

//////////////////////開關跟按鈕、LED、馬達變數//////////////////////
#define LED1 11
#define LED2 10
#define LED3 5
#define LED4 4
#define LED5 3
#define NUMBER 5
#define LED 7
#define BUTTONRED 12
#define BUTTONBULE 13
#define SW 8
int red, bule;      //按鈕變數
int OnOff;          //監控是否自動手動變數
int Seg = 1;        //為手動調整伺服馬達變數

//////////////////////HX711相關變數及陣列//////////////////////
float Hx711Array[NUMBER];
float Hx711SortArray[NUMBER];
int SendHx711;
int MedienHx711;

//////////////////////霍爾相關變數及陣列//////////////////////
float HallArray[NUMBER];
float HallSortArray[NUMBER];
const int HallPin = A2;
int HallStartValue = 0;
int SendHall;
int MedienHall;

//////////////////////X軸相關變數及陣列//////////////////////
float ZAxisArray [NUMBER];
float ZSortArray [NUMBER];
int ZStartValue = 0;
int SendZ;
int MedienZ;

//////////////////////Z軸相關變數及陣列//////////////////////
float XAxisArray[NUMBER];
float XSortArray[NUMBER];
int XStartValue = 0;
int SendX;
int MedienX;

//////////////////////Y軸相關變數及陣列//////////////////////
float YAxisArray[NUMBER];
float YSortArray[NUMBER];
int YStartValue = 0;
int SendY;

//////////////////////姿態感測收值變數//////////////////////
int16_t ax, ay, az, gx, gy, gz;

//////////////////////角度變數//////////////////////
float Gyro_y; //Y軸陀螺儀數據暫存
float Gyro_x;
float Gyro_z;
float angleAx;
float angle6;
float K1 = 0.05; // 對加速度計取值的權重
float Angle; //一階互補濾波計算出的小車最終傾斜角度
float accelz = 0;
float angle, angle_dot;

//////////////////////卡爾曼濾波變數//////////////////////
float P[2][2] = {{ 1, 0 }, { 0, 1 }};
float Pdot[4] = { 0, 0, 0, 0};
float Q_angle = 0.001, Q_gyro = 0.005; //角度數據基礎,角速度數據基礎
float R_angle = 0.5 , C_0 = 1;
float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
float timeChange = 5; //濾波法採樣時間間隔毫秒
float dt = timeChange * 0.001; //注意：dt的取值為濾波器採樣時間
void Angletest();

//////////////////////外部中斷控制變數//////////////////////
int CtrlInti = 0;
int IntBreak = 2;
int HILO;

void setup() {
  Serial.begin(115200);
  //////////////////////設定開關跟按鈕//////////////////////
  pinMode(BUTTONRED, INPUT);
  pinMode(BUTTONBULE, INPUT);
  pinMode(SW, INPUT_PULLUP);
  //////////////////////設定伺服馬達//////////////////////
  myservo.attach(6);
  //////////////////////設定MPU6050姿態感測//////////////////////
  Wire.begin();
  mpu.initialize();
  delay(100);
  //////////////////////設定應變規//////////////////////
  // HX711.DOUT  - pin #A1
  // HX711.PD_SCK - pin #A0
  scale.begin(A1, A0);
  scale.set_scale(3780.f);
  scale.tare();
  //////////////////////在程式初始時先放五個 Hall&X&Z 元素進陣列//////////////////////
  PutValueArray(HallArray, HallStartValue, HallPin);
  PutValueArray(XAxisArray, "X");
  PutValueArray(ZAxisArray, "Z");
  PutValueArray(YAxisArray, "Y");

  //////////////////////藍芽初始化//////////////////////
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  delay(1000);
  //////////////////////外部中斷設定//////////////////////
  //attachInterrupt(0, InteHall, HIGH); //assign int0
  pinMode(IntBreak, INPUT);
  //////////////////////所有設定已完成 LED亮起//////////////////////
  pinMode(LED1, OUTPUT);   //LED後帶數字為模式LED
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED, OUTPUT);   //此為全部設定完成 亮LED燈
  digitalWrite(LED, HIGH);
}

//////////////////////外部中斷涵式//////////////////////
void InteHall() {
  CtrlInti = 1;
  digitalWrite(LED, LOW);
  if (MedienX > 70) {
    MotorCmd(2);
    NowMode = 5;
    delay(2000);
  }

  CtrlInti = 0;
}

//////////////////////霍爾陣列初始化完放五個元素//////////////////////
void PutValueArray(float Array[], int StartValue, int Pin) {
  for (int i = 0; i < NUMBER; i++) {
    Array[i] = analogRead(Pin);
  }
  StartValue = analogRead(Pin);
}

void PutValueArray(float Array[]) {
  for (int i = 0; i < NUMBER; i++) {
    Array[i] = scale.get_units();
  }
  HallStartValue = scale.get_units();
}

//////////////////////姿態X, Z陣列初始化完放五個元素//////////////////////
void PutValueArray(float Array[], String Axis) {
  if (Axis == "X") {
    for (int i = 0; i < NUMBER; i++) {
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      Array[i] = (ay / 16384.0f) * 100;
    }
    XStartValue = (ay / 16384.0f) * 100;
  }
  if (Axis == "Z") {
    for (int i = 0; i < NUMBER; i++) {
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      Array[i] = (az / 16384.0f) * 100;;
    }
    ZStartValue = (az / 16384.0f) * 100;
  }
  if (Axis == "Y") {
    for (int i = 0; i < NUMBER; i++) {
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      Array[i] = (ax / 16384.0f) * 100;;
    }
    ZStartValue = (ax / 16384.0f) * 100;
  }
}

//////////////////////霍爾陣列放值進第一元素//////////////////////
void ShiftRightArray(float Array[], int Pin) {
  for (int i = NUMBER - 1; i > 0; i--) {
    Array[i] = Array[i - 1];
  }
  Array[0] = analogRead(Pin);
}

void ShiftRightArray(float Array[]) {
  for (int i = NUMBER - 1; i > 0; i--) {
    Array[i] = Array[i - 1];
  }
  Array[0] =  scale.get_units();
}

//////////////////////X, Z陣列放值進第一元素//////////////////////
void ShiftRightArray(float Array[], String Axis) {
  for (int i = NUMBER - 1; i > 0; i--) {
    Array[i] = Array[i - 1];
  }
  if (Axis == "X") {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Array[0] = (ay / 16384.0f) * 100;;
  }
  if (Axis == "Z") {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Array[0] = (az / 16384.0f) * 100;;
  }
  if (Axis == "Y") {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Array[0] = (ay / 16384.0f) * 100;;
  }
}

//////////////////////陣列排序//////////////////////
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

//////////////////////標準差計算涵式//////////////////////
void VariancePulsByLoad (float SortArray[], int* SendData) {
  float Sum1, Sum2, ToTal;

  Sum1 = pow(SortArray[1] - SortArray[2], 2);
  Sum2 = pow(SortArray[3] - SortArray[2], 2);
  ToTal = sqrt(Sum1 + Sum2);
  *SendData = ToTal;
}

void LedControl(int mode) {
  switch (mode) {
    case 1:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      break;
    case 2:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      break;
    case 3:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      break;
    case 4:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      break;
    case 5:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      break;
    case 6:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, HIGH);
      break;
  }
}

//////////////////////馬達控制函式//////////////////////
void MotorCmd(int angle) {
  switch (angle) {
    case 1:
      myservo.write(10);
      LedControl(1);
      Seg = 1;
      break;
    case 2:
      myservo.write(35);
      LedControl(2);
      Seg = 2;
      break;
    case 3:
      myservo.write(60);
      LedControl(3);
      Seg = 3;
      break;
    case 4:
      myservo.write(80);
      LedControl(4);
      Seg = 4;
      break;
    case 5:
      myservo.write(110);
      LedControl(5);
      Seg = 5;
      break;
    case 6:
      myservo.write(135);
      LedControl(6);
      Seg = 6;
      break;
  }
}

//////////////////////計算中位數//////////////////////
void CalculateByMedian(float SortArray[], int StartValue, int* Send) {

  float Median, total;
  Median = SortArray[2];
  float CalculateValue = abs(StartValue - Median);
  *Send = CalculateValue;
}

//////////////////////角度計算//////////////////////
void Angletest()
{
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  //平衡參數
  Angle = atan2(ay , az) * 57.3;           //角度計算公式
  Gyro_x = (gx - 128.1) / 131;              //角度轉換
  Kalman_Filter(Angle, Gyro_x);            //卡爾曼濾波
  //旋轉角度Z軸參數
  if (gz > 32768) gz -= 65536;              //強制轉換2G, 1G
  Gyro_z = -gz / 131;                      //Z軸參數轉換
  accelz = az / 16.4;

  angleAx = atan2(ax, az) * 180 / PI; //計算X軸夾角
  Gyro_y = -gy / 131.00; //計算角速度
  //一階互補濾波
  angle6 = K1 * angleAx + (1 - K1) * (angle6 + Gyro_y * dt);
}

//////////////////////卡爾曼濾波計算//////////////////////
void Kalman_Filter(double angle_m, double gyro_m)
{
  angle += (gyro_m - q_bias) * dt;
  angle_err = angle_m - angle;
  Pdot[0] = Q_angle - P[0][1] - P[1][0];
  Pdot[1] = - P[1][1];
  Pdot[2] = - P[1][1];
  Pdot[3] = Q_gyro;
  P[0][0] += Pdot[0] * dt;
  P[0][1] += Pdot[1] * dt;
  P[1][0] += Pdot[2] * dt;
  P[1][1] += Pdot[3] * dt;
  PCt_0 = C_0 * P[0][0];
  PCt_1 = C_0 * P[1][0];
  E = R_angle + C_0 * PCt_0;
  K_0 = PCt_0 / E;
  K_1 = PCt_1 / E;
  t_0 = PCt_0;
  t_1 = C_0 * P[0][1];
  P[0][0] -= K_0 * t_0;
  P[0][1] -= K_0 * t_1;
  P[1][0] -= K_1 * t_0;
  P[1][1] -= K_1 * t_1;
  angle += K_0 * angle_err; //角度
  q_bias += K_1 * angle_err;
  angle_dot = gyro_m - q_bias; //角速度
}

//////////////////////按鈕控制馬達//////////////////////
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

//////////////////////藍芽傳值涵式//////////////////////
void BluetoothSendData() {
  /*
    Serial.write(85);
    Serial.write(SendX);
    Serial.write(SendZ);
    Serial.write(SendHall);
    Serial.write(int(angle));
    Serial.write(NowMode);
  */

  //Serial.print("中位數 Z");
  //Serial.print(MedienZ);
  //Serial.print("標準差 Z");
  //Serial.print(SendZ);
  Serial.print("應變規  ");
  Serial.print(MedienHx711);
  Serial.print("霍爾 ");
  Serial.println(SendHall);

}

//////////////////////模式選擇//////////////////////
void NowModeSwitch() {
  HILO = digitalRead(IntBreak);

  if (HILO == HIGH) {
    InteHall();
  } else  if (SendZ >= 120 && CtrlInti == 0 && SendHx711 <= 5) {
    digitalWrite(LED, HIGH);
    MotorCmd(6);
    NowMode = 4;
    delay(1500);
  } else if (angle > 50 && CtrlInti == 0) {
    digitalWrite(LED, HIGH);
    MotorCmd(2);
    NowMode = 3;
  } else if (SendX <= 3 && SendZ <= 3) {
    digitalWrite(LED, HIGH);
    MotorCmd(1);
    NowMode = 1;
  } else if (abs(MedienHx711) >= 30 && SendHall >= 1 && CtrlInti == 0) {
    digitalWrite(LED, HIGH);
    switch (Seg) {
      case 1:
        if (SendZ >= 50) {
          MotorCmd(4);
          NowMode = 2;
          break;
        } else if (SendZ >= 25) {
          MotorCmd(3);
          NowMode = 2;
          break;
        } else if (SendZ >= 5) {
          MotorCmd(2);
          NowMode = 2;
          break;
        } else if (SendZ >= 0 && SendZ < 5) {
          MotorCmd(1);
          NowMode = 2;
          break;
        }
      case 2:
        if (SendHall >= 12) {
          MotorCmd(4);
          NowMode = 2;
          break;
        } else if (SendHall >= 8) {
          MotorCmd(3);
          NowMode = 2;
          break;
        } else if (SendHall >= 4) {
          MotorCmd(2);
          NowMode = 2;
          break;
        } else if (SendHall >= 0) {
          MotorCmd(1);
          NowMode = 2;
          break;
        }
      case 3:
        if (SendHall >= 15) {
          MotorCmd(5);
          NowMode = 2;
          break;
        } else if (SendHall >= 10) {
          MotorCmd(4);
          NowMode = 2;
          break;
        } else if (SendHall >= 5) {
          MotorCmd(3);
          NowMode = 2;
          break;
        } else if (SendHall >= 0) {
          MotorCmd(2);
          NowMode = 2;
          break;
        }
      case 4:
        if (SendHall >= 18) {
          MotorCmd(6);
          NowMode = 2;
          break;
        } else if (SendHall >= 12) {
          MotorCmd(5);
          NowMode = 2;
          break;
        } else if (SendHall >= 6) {
          MotorCmd(4);
          NowMode = 2;
          break;
        } else if (SendHall >= 0) {
          MotorCmd(3);
          NowMode = 2;
          break;
        }
      case 5:
        if (SendHall >= 20) {
          MotorCmd(6);
          NowMode = 2;
          break;
        } else if (SendHall >= 14) {
          MotorCmd(5);
          NowMode = 2;
          break;
        } else if (SendHall >= 7) {
          MotorCmd(4);
          NowMode = 2;
          break;
        } else if (SendHall >= 0) {
          MotorCmd(3);
          NowMode = 2;
          break;
        }
      case 6:
        if (SendHall >= 20) {
          MotorCmd(6);
          NowMode = 2;
          break;
        } else if (SendHall >= 12) {
          MotorCmd(5);
          NowMode = 2;
          break;
        } else if (SendHall >= 5) {
          MotorCmd(4);
          NowMode = 2;
          break;
        } else if (SendHall >= 0) {
          MotorCmd(3);
          NowMode = 2;
          break;
        }
    }
  }
}

//////////////////////手動自動開關判斷//////////////////////
void SwitchOnOff() {
  OnOff = digitalRead(SW);
  if (OnOff == HIGH) {
    Angletest();
    ShiftRightArray(Hx711Array);
    ShiftRightArray(XAxisArray, "X");
    ShiftRightArray(ZAxisArray, "Z");
    //ShiftRightArray(YAxisArray, "Y");
    ShiftRightArray(HallArray, HallPin);
    SortByArray(Hx711SortArray, Hx711Array);
    SortByArray(XSortArray, XAxisArray);
    SortByArray(ZSortArray, ZAxisArray);
    //SortByArray(YSortArray, YAxisArray);
    SortByArray(HallSortArray, HallArray);
    VariancePulsByLoad(Hx711SortArray, &SendHx711);
    VariancePulsByLoad(XSortArray, &SendX);
    VariancePulsByLoad(ZSortArray, &SendZ);
    VariancePulsByLoad(HallSortArray, &SendHall);
    //CalculateByMedian(XSortArray, XStartValue, &MedienX);
    //CalculateByMedian(ZSortArray, ZStartValue, &MedienZ);
    //CalculateByMedian(HallSortArray, HallStartValue, &MedienHall);
    Angletest();
    MedienHx711 = Hx711SortArray[2];
    MedienX = XSortArray[2];
    //MedienZ = ZSortArray[2];
    NowModeSwitch();
    BluetoothSendData();
  } else {
    BlueAndRed();
  }
}

void loop() {
  SwitchOnOff();
}
