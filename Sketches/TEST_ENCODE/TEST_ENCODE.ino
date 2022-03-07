
#define DT 2 // если не так поменять местами
#define CLK 3
#define SW 4

// Робота с бибилиотекой GyverEncoder
#include <GyverEncoder.h>

Encoder encod(CLK, DT, SW, TYPE1);  // для работы c кнопкой и сразу выбираем тип

int caunter = 0;
void setup() {
  Serial.begin(9600);
  encod.setFastTimeout(30); // таймаут на скорость isFastR. По умолч. 50
}

int8_t increment = 0; // uint8_t, только -128…127

// умный инкрементатор для настроек
void incr(int* val, int8_t incr, int limit) { // как это работает ???
  *val += incr;
  if (*val > limit) *val = limit;
  if (*val < -255) *val = -255;
}

void loop() {
  encod.tick();
  if (encod.isClick()) {
    Serial.println("Click");
  }
  if (encod.isDouble()) {
    Serial.println("Double");
  }
  if (encod.isTurn()) {
    increment = 0;
    if (encod.isRight()) increment = 1; // incr(&vals[stepParams + SumNumber], 1, 255);
    if (encod.isLeft()) increment = -1;
    if (encod.isFastR()) increment = 10;
    if (encod.isFastL()) increment = -10;
    incr(&caunter, increment, 255);
    Serial.println(caunter);
  }
}
