#include "HX711.h"
#include <Servo.h>
Servo myservo; // 建立Servo物件，控制伺服馬達

#define NUMBER 5

HX711 scale;
float ForceArray [NUMBER];


void setup() {
  Serial.begin(38400);
  myservo.attach(6, 500, 2400); // 修正脈衝寬度範圍
  Serial.setTimeout(10);
  Serial.println("Initializing the scale");
  // parameter "gain" is ommited; the default value 128 is used by the library
  // HX711.DOUT	- pin #A1
  // HX711.PD_SCK	- pin #A0
  scale.begin(A1, A0);
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());			// print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));  	// print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided
						// by the SCALE parameter (not set yet)

  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
						// by the SCALE parameter set with set_scale
  for (int i=0; i<NUMBER; i++){
    ForceArray[i] = abs(scale.get_units());
  }
}

void PutArray (){
  for (int i=NUMBER-1; i>0; i--){
    ForceArray[i] = ForceArray[i-1];
  }
  ForceArray[0] = abs(scale.get_units());
}

void List(){
  for (int i=0; i<NUMBER; i++){
    Serial.print(ForceArray[i], 1);    
    Serial.print("\t");    
  }  
  Serial.print("Average:");
  Serial.println(Average());     
}

void MotorCmd(int angle){  
  switch(angle)  {
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


float Average(){
  float avg=0;
  for (int i=0; i<NUMBER; i++){
    avg += ForceArray[i];
  }  
  return avg/NUMBER;
}

void CalculateByLoad () {
  float avg = Average();
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

void CalculateByVariance () {
  float max,min;
  
  
}
void loop() {  
  int c;
  PutArray();
  List();
  //delay(100);
  CalculateByVariance();
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

*/

