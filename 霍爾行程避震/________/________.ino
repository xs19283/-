//////////////////////庫//////////////////////
#include<Servo.h>
#include "I2Cdev.h"
#include "MPU6050.h"

//////////////////////實例化物件//////////////////////
Servo myservo;
MPU6050 mpu;

//////////////////////五大模式變數//////////////////////
int NowMode;

//////////////////////開關跟按鈕、LED、馬達變數//////////////////////
#define NUMBER 5
#define LED 7
#define BUTTONRED 12
#define BUTTONBULE 13
#define SW 8
int red, bule;      //按鈕變數
int OnOff;          //監控是否自動手動變數
int Seg = 1;        //為手動調整伺服馬達變數

//////////////////////霍爾相關變數及陣列//////////////////////
float HallArray[NUMBER];
float HallSortArray[NUMBER];
const int HallPin = A2;
int HallStartValue = 0;
int SendHall;

//////////////////////X軸相關變數及陣列//////////////////////
float ZAxisArray [NUMBER];
float ZSortArray [NUMBER];
int SendZ;

//////////////////////Z軸相關變數及陣列//////////////////////
float XAxisArray[NUMBER];
float XSortArray[NUMBER];
int SendX;

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
float Q_angle = 0.001, Q_gyro = 0.005; //角度数据置信度,角速度数据置信度
float R_angle = 0.5 , C_0 = 1;
float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
float timeChange = 5; //滤波法采样时间间隔毫秒
float dt = timeChange * 0.001; //注意：dt的取值为滤波器采样时间

//////////////////////外部中斷控制變數//////////////////////
int CtrlInti = 0;

void setup() {
  Serial.begin(9600);
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
  //////////////////////在程式初始時先放五個 Hall&X&Z 元素進陣列//////////////////////
  PutValueArray(HallArray, HallStartValue, HallPin);
  PutValueArray(XAxisArray, "X");
  PutValueArray(ZAxisArray, "Z");
  //////////////////////藍芽初始化//////////////////////
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  delay(1000);
  //////////////////////外部中斷設定//////////////////////
  attachInterrupt(0, InteHall, HIGH); //assign int0
  //////////////////////所有設定已完成 LED亮起//////////////////////
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}

//////////////////////外部中斷涵式//////////////////////
void InteHall() {
  CtrlInti = 1;
  if (SendX > 1000) {
    MotorCmd(2);
    NowMode = 5;
    delay(1000);
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

//////////////////////姿態X, Z陣列初始化完放五個元素//////////////////////
void PutValueArray(float Array[], String Axis) {
  if (Axis == "X") {
    for (int i = 0; i < NUMBER; i++) {
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      Array[i] = ax;
    }
  }
  if (Axis == "Z") {
    for (int i = 0; i < NUMBER; i++) {
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      Array[i] = az;
    }
  }
}

//////////////////////霍爾陣列放值進第一元素//////////////////////
void ShiftRightArray(float Array[], int Pin) {
  for (int i = NUMBER - 1; i > 0; i--) {
    Array[i] = Array[i - 1];
  }
  Array[0] = analogRead(Pin);
}

//////////////////////X, Z陣列放值進第一元素//////////////////////
void ShiftRightArray(float Array[], String Axis) {
  for (int i = NUMBER - 1; i > 0; i--) {
    Array[i] = Array[i - 1];
  }
  if (Axis == "X") {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Array[0] = ax;
  }
  if (Axis == "Z") {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Array[0] = az;
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

//////////////////////馬達控制函式//////////////////////
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

//////////////////////角度計算//////////////////////
void Angletest()
{
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
  int AccX=(ay/16384.0f)*100;
  int AccZ=(az/16384.0f)*100;
  
    Serial.write(85);
    Serial.write(AccX);
    Serial.write(AccZ);
    Serial.write(SendHall);
    Serial.write(int(angle));
    Serial.write(NowMode);
  
  /*
    Serial.write(85);
    Serial.write(analogRead(XPin));
    Serial.write(analogRead(ZPin));
    Serial.write(analogRead(HallPin));
    Serial.write(1);
  */
}

//////////////////////模式選擇//////////////////////
void NowModeSwitch() {
  if (SendZ > 500 && SendHall <= 4 && CtrlInti == 0) {
    MotorCmd(6);
    NowMode = 4;
    delay(1000);
  } else if (angle > 43 && SendHall >= 5 && CtrlInti == 0) {
    MotorCmd(2);
    NowMode = 3;
    delay(1000);
  } else if (SendZ > 400 && SendHall > 10 && CtrlInti == 0) {
    if (SendHall > 10) {
      MotorCmd(3);
      NowMode = 2;
      delay(500);
    } else if (SendHall > 30) {
      MotorCmd(4);
      NowMode = 2;
      delay(500);
    } else if (SendHall > 50) {
      MotorCmd(5);
      NowMode = 2;
      delay(500);
    }
  } else if (SendX > 1000 && CtrlInti == 0) {
    MotorCmd(1);
    NowMode = 1;
    delay(2000);
  } else if (SendX <= 1000 && CtrlInti == 0) {
    MotorCmd(1);
    NowMode = 1;
    delay(500);
  }
}

//////////////////////手動自動開關判斷//////////////////////
void SwitchOnOff() {
  OnOff = digitalRead(SW);
  if (OnOff == HIGH) {
    ShiftRightArray(XAxisArray, "X");
    ShiftRightArray(ZAxisArray, "Z");
    ShiftRightArray(HallArray, HallPin);
    SortByArray(XSortArray, XAxisArray);
    SortByArray(ZSortArray, ZAxisArray);
    SortByArray(HallSortArray, HallArray);
    VariancePulsByLoad(XSortArray, &SendX);
    VariancePulsByLoad(ZSortArray, &SendZ);
    VariancePulsByLoad(HallSortArray, &SendHall);
    Angletest();
   // NowModeSwitch();
    BluetoothSendData();
  } else {
    BlueAndRed();
  }
}

void loop() {
  SwitchOnOff();
}
