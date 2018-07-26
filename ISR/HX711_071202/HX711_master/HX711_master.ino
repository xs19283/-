#include <Servo.h>
#include <MsTimer2.h>

Servo myservo; // 建立Servo物件，控制伺服馬達
//Timer T1;

#define NUMBER 5
#define LED 7
#define BUTTONRED 12
#define BUTTONBULE 13
#define SW 8
#define TireNUMBER 8

int TempArray[8];
int TireArray[TireNUMBER];
int TireCount;
int Seg = 1;
int OnOff;
int red, bule;

int flag = 0;
int count = 0;
int a = 1;

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
  Serial1.begin(9600);



  ////////////////////////////////////
  ////在程式開始時亮燈
  ////////////////////////////////////
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  MsTimer2::set(8, TireValue);
  MsTimer2::start();
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

void TireValue() {
  int in1;
  
  TirePrint();
}

////////////////////////////////////
////主要程式迴圈
////////////////////////////////////
void loop() {
  //SwitchOnOff();

}


/*if (flag != 0) {
    for (int i = 0; i < 8; i++) {
      Serial.print(Serial1.read());
      Serial.print("\t");
    }
    Serial.println();
    flag--;
  }*/
