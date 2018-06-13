const int buttonPin = 2;
const int interruptNumber = 0;
volatile int buttonState; 

// 用來儲存按鈕狀態

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT);             // 把 buttonPin 設置成 INPUT
  attachInterrupt(interruptNumber, buttonStateChanged, FALLING);
}

void buttonStateChanged() {
  buttonState = digitalRead(buttonPin);
   Serial.println("GGGGG");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("YYYYYY");
  delay(250);
}

