#include "HX711.h"
#include <Servo.h>
Servo myservo; // 建立Servo物件，控制伺服馬達
char cmd[9] ;  //接字元字串
byte data[5];
byte compareArray[2];
void compare(byte*);
short sum1,sum2;

// HX711.DOUT	- pin #A1
// HX711.PD_SCK	- pin #A0

HX711 scale(A1, A0);		// parameter "gain" is ommited; the default value 128 is used by the library
void setup() {
  Serial.begin(57600);
  Serial.println("HX711 Demo");
  
  myservo.attach(6, 500, 2400); // 修正脈衝寬度範圍
  myservo.write(90); //初始為90中間位置
  delay(1000);
  
  Serial.setTimeout(10); // 設定為每10毫秒結束一次讀取字元
  

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

  Serial.println("Readings:");
  scale.power_up();
  for(byte b=0;b<5;b++)
  {
    unsigned short bdata=scale.get_units();
    data[b]=bdata;
  }
}

void loop() 
{
  compareArray[1]=compareArray[0];
  byte ctrl;
  
  Serial.print("one reading:\t \t \t \t");
  double bdbefore=abs(scale.get_units());
  Serial.print(bdbefore,1);
  
  Serial.println();
  Serial.println(" ");
  delay(20);
  if(scale.get_units())
  {
    for(byte b=4;b>0;b--)
    {
      data[b]=data[b-1];
    }
    unsigned short bdafter=bdbefore;
    data[0]=bdafter;
  }
  compare(data);
  Serial.print("平均值後差距大小 : ");
  Serial.println(compareArray[0]-compareArray[1]);
  Serial.print("平均值 : ");
  Serial.println(sum1);
  Serial.println(" ");
  for(byte b=0;b<sizeof(data);b++)
  {
      Serial.print(data[b]);
      Serial.print(" ");
  }
  Serial.println(" ");
  if(sum1<50){
    ctrl=3;
  }
  if(sum1>=50&&sum1<100){
    ctrl=4;
  }
  if(sum1>=100){
    ctrl=5;
  }
    
  if(Serial.available())
  {  //如果有接到資料 讀取字元並放入cmd字串 並轉為整數設定伺服馬達轉向
    Serial.readBytes(cmd,9);
    ctrl=atoi(cmd);
  }
  switch(ctrl)
  {
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

void compare(byte* dataA)
{
  sum1=0;
  for(byte b=0;b<5;b++)
  {
    sum1+=dataA[b];
  }
  sum1=sum1/5;
  Serial.println(sum1);
  compareArray[0]=sum1;
  
}

