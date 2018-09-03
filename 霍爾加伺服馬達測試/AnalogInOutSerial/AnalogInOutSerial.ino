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
float angle, angle_dot;

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

/////////////////卡爾曼濾波變數
float P[2][2] = {{ 1, 0 }, { 0, 1 }};
float Pdot[4] = { 0, 0, 0, 0};
float Q_angle = 0.001, Q_gyro = 0.005; //角度数据置信度,角速度数据置信度
float R_angle = 0.5 , C_0 = 1;
float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
float timeChange = 5; //滤波法采样时间间隔毫秒
float dt = timeChange * 0.001; //注意：dt的取值为滤波器采样时间

/////////////////收值變數
int16_t ax, ay, az, gx, gy, gz;

/////////////////角度變數
float Gyro_y; //Y軸陀螺儀數據暫存
float Gyro_x;
float Gyro_z;
float angleAx;
float angle6;
float K1 = 0.05; // 對加速度計取值的權重
float Angle; //一階互補濾波計算出的小車最終傾斜角度
float accelz = 0;
float angle, angle_dot;

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
  Wire.begin();
  mpu.initialize(); 
  delay(100);

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
  delay(1000);
  ////////////////////////////////////
  ////所有設定已完成 LED亮起
  ////////////////////////////////////
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}

////////////////////////////////////
////陣列初始化完放五個元素
////////////////////////////////////
void PutValueArray(float Array[], int StartValue, int Pin) {
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
void VariancePulsByLoad (float SortArray[], int* SendData) {
  float Sum1, Sum2, ToTal;

  Sum1 = pow(SortArray[1] - SortArray[2], 2);
  Sum2 = pow(SortArray[3] - SortArray[2], 2);
  ToTal = sqrt(Sum1 + Sum2);
  *SendData = ToTal;
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

  Serial.write(85);
  Serial.write(SendX);
  Serial.write(SendZ);
  Serial.write(SendHall);
  Serial.write(NowMode);

  /*
    Serial.write(85);
    Serial.write(analogRead(XPin));
    Serial.write(analogRead(ZPin));
    Serial.write(analogRead(HallPin));
    Serial.write(1);
  */
}

////////////////////////////////////
////模式選擇
////////////////////////////////////
void NowModeSwitch() {
  if (SendZ > 40 && SendHall <= 20) {
    MotorCmd(6);
    NowMode = 5;
    delay(2000);
  } else if (SendZ > 5 && SendHall > 10) {
    if (SendHall > 10) {
      MotorCmd(3);
      NowMode = 2;
      delay(1000);
    } else if (SendHall > 50) {
      MotorCmd(4);
      NowMode = 2;
      delay(1000);
    }
  } else if (SendX > 40) {
    MotorCmd(1);
    NowMode = 1;
    delay(2000);
  } else if (SendX <= 10) {
    MotorCmd(1);
    NowMode = 1;
  }
}

////////////////////////////////////
////角度計算
////////////////////////////////////
void Angletest()
{
  //平衡参数
  Angle = atan2(ay , az) * 57.3;           //角度计算公式
  Gyro_x = (gx - 128.1) / 131;              //角度转换
  Kalman_Filter(Angle, Gyro_x);            //卡曼滤波
  //旋转角度Z轴参数
  if (gz > 32768) gz -= 65536;              //强制转换2g  1g
  Gyro_z = -gz / 131;                      //Z轴参数转换
  accelz = az / 16.4;
 
  angleAx = atan2(ax, az) * 180 / PI; //计算与x轴夹角
  Gyro_y = -gy / 131.00; //计算角速度
  //一阶互补滤波
  angle6 = K1 * angleAx + (1 - K1) * (angle6 + Gyro_y * dt);
}

////////////////////////////////////
////卡爾曼濾波計算
////////////////////////////////////
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

////////////////////////////////////
////開關判斷
////////////////////////////////////
void SwitchOnOff() {
  OnOff = digitalRead(SW);
  if (OnOff == HIGH) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Angletest();
    ShiftRightArray(HallArray, HallPin);
    ShiftRightArray(ZAxisArray, ZPin);
    ShiftRightArray(XAxisArray, XPin);
    SortByArray(HallSortArray, HallArray);
    SortByArray(ZSortArray, ZAxisArray);
    SortByArray(XSortArray, XAxisArray);
    VariancePulsByLoad(HallSortArray, &SendHall);
    VariancePulsByLoad(ZSortArray, &SendZ);
    VariancePulsByLoad(XSortArray, &SendX);
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
