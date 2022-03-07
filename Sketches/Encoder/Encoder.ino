


// Что это такое это вот это вот ?(^)?

// volatile
// #if #else #endif
// bitRead(PIND, ENC_A);
// (digitalRead(ENC_B) != lastState) ? -1 : 1; как расшифровать?
// void int0() {}


// Робота с бибилиотекой GyverEncoder
#include <GyverEncoder.h>


//#include "GyverRGB.h"
//GRGB diode(9, 10, 11);

//#include "GRGB.h"
//GRGB led(COMMON_CATHODE, 9, 10, 11);


#define DT 2 // если не так поменять местами
#define CLK 3
#define SW 4

int caunter = 0;

Encoder encod(CLK, DT, SW, TYPE1);  // для работы c кнопкой и сразу выбираем тип

void setup() {
  Serial.begin(9600);

//  pinMode(10, OUTPUT);

  
  encod.setType(TYPE1); // тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип\=
  encod.setFastTimeout(30); // таймаут на скорость isFastR. По умолч. 50
  
}

int8_t increment = 0; // uint8_t, только -128…127

// умный инкрементатор для настроек
void incr(int* val, int8_t incr, int limit) { // как это работает ???
  *val += incr;
  if (*val > limit) *val = limit;
  if (*val < 0) *val = 0;
}



void loop() {
  encod.tick();

  if (encod.isClick()) {
    Serial.println("Click");
  }

  
  if (encod.isTurn()) {
    increment = 0;
    if (encod.isRight()) increment = 1; // incr(&vals[stepParams + SumNumber], 1, 255);
    if (encod.isLeft()) increment = -1;
    if (encod.isFastR()) increment = 10;
    if (encod.isFastL()) increment = -10;
    incr(&caunter, increment, 255);
//    caunter = incr(caunter, increment, 255);
//    caunter = caunter + increment;

    
    Serial.println(caunter);
    
    
//    if (encod.isRight()) {              
//      caunter = constrain(caunter + 1, 0, 255); // Двигаем указатель вниз
//    }
//
//    if (encod.isLeft()) {              
//      caunter = constrain(caunter - 1, 0, 255); // Двигаем указатель вверх
//    }

//    if (encod.isFastR()) {
////      caunter = constrain(caunter + 6, 0, 255);
//      caunter = 1;
//    }
//
//    if (encod.isFastL()) {
//      caunter = -1;
////      caunter = constrain(caunter - 6, 0, 255);
//    }

//    Serial.println(caunter);
//    led.setWheel8(caunter);
//    diode.setRGB(0, caunter, 0);
//    analogWrite(10, caunter);
    
  }
//  if (encod.isRight()) {
//    caunter++;
////    Serial.println("isRight");
//  }
//  if (encod.isLeft()) {
//    caunter--;
////    Serial.println("isLeft");
//  }


  

  

  
//
//  if (encod.isRightH()) {
//    caunter += 5;
////    Serial.println("Right holded"); // если было удержание + поворот
//  }
//  if (encod.isLeftH()) {
//    caunter -= 5;
////    Serial.println("Left holded");
//  }

//  if (encod.isTurn()) {
//    Serial.println(caunter);
//  }

//  if (encod.isPress()) {
//    Serial.println("Press"); // нажатие на кнопку (+ дебаунс)
//  }
//  if (encod.isClick()) {
//    Serial.println("Click"); // отпускание кнопки (+ дебаунс)
//  }
//  if (encod.isRelease()) {
//    Serial.println("Release"); // то же самое, что isClick
//  }
//  if (encod.isHolded()) {
//    Serial.println("Holded"); // если была удержана и энк не поворачивался
//  }
  //if (enc1.isHold()) Serial.println("Hold");         // возвращает состояние кнопки

  
}




// алгоритм с "таблицей", позволяющий увеличить точность энкодера
// в 4 раза, работает максимально чётко даже с плохими энкодерами.
// Для увеличения скорости опроса используйте PCINT и чтение из PINn
/* Почему то прибавляет сразу 2 
#define CLK 3
#define DT 2
long pos = 0;
byte lastState = 0;
const int8_t increment[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
void setup() {
  Serial.begin(9600);
}
void loop() {
  byte state = digitalRead(CLK) | (digitalRead(DT) << 1);
  if (state != lastState) {
    pos += increment[state | (lastState << 2)];
    lastState = state;
    Serial.println(pos);
  }
}
*/

// Тестирование и выведение на графики
/*
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print(digitalRead(2) * 5);
  Serial.print(' ');
  Serial.println(digitalRead(3) * 5);
  delay(10);
}
*/

// Почему ничего не робит ??? аааа :(((
/*
volatile int counter = 0;   // счётчик
volatile bool encFlag = 0;  // флаг поворота
void setup() {
  Serial.begin(9600);
  attachInterrupt(0, encIsr, CHANGE);
  attachInterrupt(1, encIsr, CHANGE);
}
void loop() {
  if (encFlag) {
    Serial.println(counter);
    encFlag = 0;
  }
}
volatile byte reset = 0, last = 0;
void encIsr() {  
  byte state = (PIND & 0b1100) >> 2;  // D2 + D3
  if (reset && state == 0b11) {
    int prevCount = counter;
    if (last == 0b10) counter++;
    else if (last == 0b01) counter--;
    if (prevCount != counter) encFlag = 1;    
    reset = 0;
  }
  if (!state) reset = 1;
  last = state;
}
*/

/*
  Максимально быстрый универсальный код для обработки энкодера
  Работает на перывании (используется одно)
  Тут код построен на digitalRead, что делает его универсальным для всех плат Arduino
*/
/*
#define ENC_A 2       // пин энкодера
#define ENC_B 3       // пин энкодера
#define ENC_TYPE 0    // тип энкодера, 0 или 1
volatile int encCounter;
volatile boolean state0, lastState, turnFlag;
void setup() {
  Serial.begin(9600);
  attachInterrupt(0, int0, CHANGE);
}
void int0() { // ??? что ???
  state0 = digitalRead(ENC_A);
  if (state0 != lastState) {
#if (ENC_TYPE == 1)
    turnFlag = !turnFlag;
    if (turnFlag)
      encCounter += (digitalRead(ENC_B) != lastState) ? -1 : 1; 
#else
    encCounter += (digitalRead(ENC_B) != lastState) ? -1 : 1;
#endif
    lastState = state0;
  }
}

void loop() {
  Serial.println(encCounter);
  delay(100);
}
//*/



/*
 Максимально быстрый универсальный код для обработки энкодера
 Работает на перывании (используется одно)
 Тут код построен на bitRead(PIND..) - только для Arduino NANO! А КАК ТАК ???
*/
/*
#define ENC_A 2       // пин энкодера
#define ENC_B 3       // пин энкодера
#define ENC_TYPE 1    // тип энкодера, 0 или 1

volatile int encCounter;
volatile boolean state0, lastState, turnFlag; // что такое volatile

void setup() {
  Serial.begin(9600);
  attachInterrupt(0, int0, CHANGE); // attachInterrupt ???
}

void int0() {
  state0 = bitRead(PIND, ENC_A); // bitRead ???
  if (state0 != lastState) {
#if (ENC_TYPE == 1)
    turnFlag = !turnFlag;
    if (turnFlag)
      encCounter += (bitRead(PIND, ENC_B) != lastState) ? -1 : 1;
#else
    encCounter += (bitRead(PIND, ENC_B) != lastState) ? -1 : 1;
#endif
    lastState = state0;
  }
}
void loop() {
  Serial.println(encCounter);
  delay(100);
}
*/
