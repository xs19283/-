#include <Arduino_FreeRTOS.h>

volatile int count1 = 0;
volatile int count2 = 0;
volatile int total = 0;

int TireArray[8];
int TempArray[8];
void TirePrint() {
  for (int i = 0; i < 8; i++) {
    Serial.print(TireArray[i]);
    Serial.print("\t");
  }
  Serial.println("");
}

static void vCount1Task_1(void *pvParameters) {
  int total = 0;
  byte temp = 0;
  for (;;) {
    if (Serial.peek() == 85) {
      TempArray[0] = Serial.read();
      for (int i = 1; i < 8; i++) {
        TempArray[i] = Serial.read();
      }
      total = TempArray[0] + TempArray[1] + TempArray[2] + TempArray[3] + TempArray[4] + TempArray[5] + TempArray[6];
      temp = (byte)(~total + 1);
      if (temp == TempArray[7]) {
        for (int i = 0; i < 8; i++) {
          TireArray[i] = TempArray[i];
        }
        TirePrint();
      } else {
        vTaskDelay((10L * configTICK_RATE_HZ) / 1000L);
      }
    } else {
      Serial.read();
      vTaskDelay((10L * configTICK_RATE_HZ) / 1000L);
    }
    TirePrint();
  }
}
static void vCountTask_2(void *pvParameters) {
  for (;;) {
    count2++;
    Serial.print("count2 : ");
    Serial.println(count2);
    vTaskDelay((1000L * configTICK_RATE_HZ) / 1000L);
  }
}

static void vTotalTask_3(void *pvParameters) {
  for (;;) {
    total = count1 + count2;
    Serial.print("Total : ");
    Serial.println(total);
    vTaskDelay((1500L * configTICK_RATE_HZ) / 1000L);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {}

  xTaskCreate(vCount1Task_1,
              "Task1",
              configMINIMAL_STACK_SIZE + 64,
              NULL,
              tskIDLE_PRIORITY + 3,
              NULL);

  xTaskCreate(vCountTask_2,
              "Task2",
              configMINIMAL_STACK_SIZE + 50,
              NULL,
              tskIDLE_PRIORITY + 2,
              NULL);

  xTaskCreate(vTotalTask_3,
              "Task3",
              configMINIMAL_STACK_SIZE + 50,
              NULL,
              tskIDLE_PRIORITY + 1,
              NULL);

  vTaskStartScheduler();

  while (1);
}
void loop() {
  // put your main code here, to run repeatedly:

}
