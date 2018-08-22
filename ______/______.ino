int hellPin = A4;
int staValue;
int rdValue;
int countValue;
void setup() {
  pinMode(hellPin, INPUT);
  Serial.begin(9600);
  staValue = analogRead(hellPin);
}

void loop() {
  rdValue = analogRead(hellPin);
  countValue = abs(rdValue - staValue);
  Serial.println(countValue);
  if(countValue > 0 && countValue<=3){ //25   90     105   110
    Serial.println("0 Cm");
  }else if(countValue > 3 && countValue <=20){
    Serial.println("接近 0.5 Cm");
  }else if(countValue > 20 && countValue <=30){
    Serial.println("0.5 CM");
  }else if(countValue > 30 && countValue <= 85){
    Serial.println("接近 1 CM");
  }else if(countValue > 85 && countValue <= 95){
    Serial.println("1 CM");
  }else if(countValue > 95 && countValue <= 100){
    Serial.println("接近1.5 CM");
  }else if(countValue >100 && countValue <= 107){
    Serial.println("1.5 CM");
  }else if(countValue > 107 && countValue <= 110){
    Serial.println("接近 2 CM");
  }else{
    Serial.println("2 CM");
  }
}
