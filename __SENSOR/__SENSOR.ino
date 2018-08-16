#define Z_PIN A1

void setup(){
  Serial.begin(9600);
  // 注意，在呼叫analogRead之前必須先呼叫analogReference
}

// 將類比輸入的值轉成g
float toG(float v){
  return v * 6 / 1023 - 3;
}

void loop(){
  int z  = analogRead(Z_PIN);
  
  Serial.print("Z: ");
  Serial.print(z);
  Serial.print("g, Z: ");
  Serial.print(toG(z));
  Serial.println("g");
}
