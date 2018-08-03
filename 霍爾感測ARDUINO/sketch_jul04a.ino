
int Led = 13;                                    //設定LED接腳為第13孔

int buttonpin = 3;                           //設定線性霍爾磁力感測器接腳為第3孔

int val;                                              //設定變量val

void setup()

{

  pinMode(Led, OUTPUT);              //設定LED為輸出

  pinMode(buttonpin, INPUT);        //設定感測器為輸入
  Serial.begin(9600);
}

void loop()

{

  val = digitalRead(buttonpin);        //將感測器的值讀给val

  if (val == HIGH)                                 //當感測器檢测有信号時，LED 閃爍

  {

    Serial.println("123");

  }

  else

  {

    digitalWrite(Led, LOW);

  }

}
