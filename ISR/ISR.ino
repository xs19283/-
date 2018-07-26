const int intA = 5000;   // 2500 * 0.1 ms = 250ms

volatile int ggyy = 1;  // 使用這當 Flag 給  ISR 使用 !

const int myTOP = 24;

ISR(TIMER1_COMPA_vect) {
  static unsigned int aaa = 0;
  ++aaa;
  if (aaa == intA) {
    aaa = 0; button( );
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  setMyTimerOne( );
}

void setMyTimerOne( ) {
  cli();  // 禁止中斷
  TCCR1A = 0;
  TCCR1B = (1 << WGM12); // CTC mode; Clear Timer on Compare
  TCCR1B |= (1 << CS10) | (1 << CS11); // Prescaler == 64
  /////////
  OCR1A = myTOP;  // TOP count for CTC, 與 prescaler 有關
  TCNT1 = 0; // counter 歸零
  TIMSK1 |= (1 << OCIE1A);  // enable CTC for TIMER1_COMPA_vect
  sei();  // 允許中斷
}

void button() {
  Serial.println("BBBB");
  ggyy = 1 - ggyy;
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("aaaaa");
  delay(1000);
}
