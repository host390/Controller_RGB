// пример с простым шахматным меню
// пины энкодера
#define CLK 3
#define DT 2
#define SW 4

#define PRECISE_ALGORITHM // чтоб всё норм с энкодером было :)
#include <GyverEncoder.h>
Encoder enc(CLK, DT, SW);  // для работы c кнопкой

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // адрес 0x27 или 0x3f

#include "GRGB.h"
GRGB led(COMMON_CATHODE, 9, 10, 11);


#define LINES 2 // количество строк дисплея
#define SETTINGS_AMOUNT 4 // количество настроек
#define FAST_STEP 5 // скорость изменения при быстром повороте

// переменные
//byte val_R = 0;
//byte val_G = 0;
//byte val_B = 0;
//byte val_Braig = 120;

int vals[SETTINGS_AMOUNT];  // массив параметров


const char channel_R[] PROGMEM = "R";
const char channel_G[] PROGMEM = "G";
const char channel_B[] PROGMEM = "B";
const char Brightnes[] PROGMEM = "Brightnes";

const char* const names[] PROGMEM = {
  channel_R, channel_G, channel_B, Brightnes,
};

int16_t arrowPos = 0;  // позиция стрелки

bool chosenPoint = false;

bool last_Arr_Left = false;
bool last_Arr_Right = false;

bool clearNew = false;

bool encodClick = false;

bool printLastZero = false;
bool printTwoZero = false;

int increment = 0;
int arrowRot;
int s = 0;


void setup() {
  Serial.begin(9600);
  enc.setType(TYPE1);
  lcd.init();
  lcd.backlight();
  enc.setFastTimeout(40); // чем больше тем сложнее отличить быстрый поворот от обычного ::::

  vals[3] = 120;
  printGUI_Start();   // выводим интерфейс
}


// умный инкрементатор для настроек
void incr(int* val, int incr, int limit) { // значение, прибавление, предел
  *val += incr;
  if (*val > limit) *val = limit;
  if (*val < 0) *val = 0;
}

void loop() {
  enc.tick();

  
  if (enc.isClick()) { // смена контроля
    chosenPoint = !chosenPoint;
//    encodClick = true;
    printGUI();
  }

  if (enc.isTurn()) {  // при любом повороте
    
    increment = 0;

    if (!chosenPoint) { // если меню не выбрано
      if (enc.isRight()) {
        increment = 1;
        last_Arr_Right = true;
      }
      if (enc.isLeft()) {
        increment = -1;
        last_Arr_Left = true;
      }

//      if (enc.isFastR()) Serial.println("FastR");
//      if (enc.isFastL()) Serial.println("FastL");
      
      incr(&arrowPos, increment, 3);
//      Serial.println(arrowPos);
    } else {
      increment = 0;

      if ((chosenPoint && enc.isRight())) increment = 1;
      if ((chosenPoint && enc.isLeft())) increment = -1;
      
      if (chosenPoint && enc.isFastR()) increment = FAST_STEP;
      if (chosenPoint && enc.isFastL()) increment = -FAST_STEP;

      incr(&vals[arrowPos], increment, 255);
//      vals[arrowPos] += increment;  // меняем параметры
      Serial.println(vals[arrowPos]);

      if (vals[arrowPos] > 9) {
        clearNew = true;
      }

      led.setRGB(vals[0], vals[1], vals[2], vals[3]);

      if (clearNew) {
        if (vals[arrowPos] < 10) {
          printTwoZero = true;
          clearNew = false;
        }
        if (vals[arrowPos] > 40 && vals[arrowPos] < 100) {
          printLastZero = true;
          clearNew = false;
        }
      }
     
    }

    printGUI();
    
    /*
    if (!chosenPoint) {
      
      if (enc1.isRight()) {
        arrowPos++;
        if (arrowPos >= 4) arrowPos = 3;  // ограничиваем позицию курсора
      }
      
      if (enc1.isLeft()) {
        arrowPos--;
        if (arrowPos < 0) arrowPos = 0;  // ограничиваем позицию курсора
      }
      
    } 
    else { // если пункт меню выбран
      
      if (enc1.isRight()) {
        switch (arrowPos) {
          case 0: val_R = examinationValPluse(val_R);
            break;
          case 1: val_G = examinationValPluse(val_G);
            break;
          case 2: val_B = examinationValPluse(val_B);
            break;
          case 3: val_Braig = examinationValPluse(val_Braig);
            break;
        }
      }

      if (enc1.isLeft()) { 
        switch (arrowPos) {
          case 0: val_R = examinationVal(val_R);
            break;
          case 1: val_G = examinationVal(val_G);
            break;
          case 2: val_B = examinationVal(val_B);
            break;
          case 3: val_Braig = examinationVal(val_Braig);
            break;
        }
      }

      if (enc1.isFastR() and chosenPoint) {
        switch (arrowPos) {
          case 0: val_R = constrain(val_R + 6, 0, 255);
            break;
          case 1: val_G = constrain(val_G + 6, 0, 255);
            break;
          case 2: val_B = constrain(val_B + 6, 0, 255);
            break;
          case 3: val_Braig = constrain(val_Braig + 6, 0, 255);
            break;
        }
      }

      if (enc1.isFastL()) {
        switch (arrowPos) {
          case 0: val_R = constrain(val_R - 6, 0, 255);
            break;
          case 1: val_G = constrain(val_G - 6, 0, 255);
            break;
          case 2: val_B = constrain(val_B - 6, 0, 255);
            break;
          case 3: val_Braig = constrain(val_Braig-=6, 0, 255);
            break;
        }
      }

      
    }
    Serial.println(val_R);
    led.setRGB(val_R, val_G, val_B, val_Braig);
    printGUI();   // выводим интерфейс
  */  
  }
}

//const char name1[] PROGMEM = "Speed";
//const char name2[] PROGMEM = "Temp";
//const char name3[] PROGMEM = "Mode";
//
//// объявляем таблицу ссылок
//const char* const names[] PROGMEM = {
//  name1, name2, name3,
//};
//
//#define SETTINGS_AMOUNT 7  // количество настроек
//#define LINES 2       // количество строк дисплея
//int vals[SETTINGS_AMOUNT];  // массив параметров


void printGUI() {
  
  switch (arrowPos) {
    case 0: lcd.setCursor(1, 0);
      break;
    case 1: lcd.setCursor(6, 0);
      break;
    case 2: lcd.setCursor(11, 0);
      break;
    case 3: lcd.setCursor(9, 1);
      break;
  }
  smartArrow(1); // рисую курсор

  if (chosenPoint) {
    lcd.print(vals[arrowPos]);
  }

  if (printTwoZero) {
    switch (arrowPos) {
      case 0: lcd.setCursor(3, 0);
        break;
      case 1: lcd.setCursor(8, 0);
        break;
      case 2: lcd.setCursor(13, 0);
        break;
      case 3: lcd.setCursor(11, 1);
        break;
    }
    lcd.print(F(" "));
    printTwoZero = false;
    
  }

  if (printLastZero) { // если надо убрать последний ненужный нуль...
    switch (arrowPos) {
      case 0: lcd.setCursor(4, 0);
        break;
      case 1: lcd.setCursor(9, 0);
        break;
      case 2: lcd.setCursor(14, 0);
        break;
      case 3: lcd.setCursor(12, 1);
        break;
    }
    lcd.print(F(" "));
    printLastZero = false;
  }
  
  if (last_Arr_Left) { // если крутится в лево 
    switch (arrowPos + 1) {
      case 0: lcd.setCursor(1, 0);
        break;
      case 1: lcd.setCursor(6, 0);
        break;
      case 2: lcd.setCursor(11, 0);
        break;
      case 3: lcd.setCursor(9, 1);
        break;
    }
    lcd.print(F(":"));
    last_Arr_Left = false;
    
  }

  if (last_Arr_Right) { // если крутиться вправо
    switch (arrowPos - 1) {
      case 0: lcd.setCursor(1, 0);
        break;
      case 1: lcd.setCursor(6, 0);
        break;
      case 2: lcd.setCursor(11, 0);
        break;
      case 3: lcd.setCursor(9, 1);
        break;
    }
    lcd.print(F(":"));
    last_Arr_Right = false;
  }
  
//  else { // если крутиться в право
////    Serial.println(arrowPos - 1);
////    arrowRot = arrowPos - 1; // 
////    if (arrowRot <= 0) arrowRot = 0;
//    if (!chosenPoint) { // если это не нажатае на кнопку    
//      switch (arrowPos - 1) {
//        case 0: lcd.setCursor(1, 0);
//          break;
//        case 1: lcd.setCursor(6, 0);
//          break;
//        case 2: lcd.setCursor(11, 0);
//          break;
//        case 3: lcd.setCursor(9, 1);
//          break;
//      }
//      lcd.print(F(":"));
//    }
////    encodClick = false;
//  }

  // Надо находить предыдущую позицию стрелки и ставить туда э:


  
//  lcd.setCursor(0, 0);

  
//  for (int i = 0; i < SETTINGS_AMOUNT; i++) {

//    switch (i) {
//      case 0: lcd.setCursor(0, 0);
//        break;
//      case 1: lcd.setCursor(5, 0);
//        break;
//      case 2: lcd.setCursor(10, 0);
//        break;
//      case 3: lcd.setCursor(0, 1);
//        break;
//    }
    
//    printFromPGM(&names[i]);

//    if (arrowPos == i) { // получаем нужнаю координату стрелки
//      lcd.setCursor(0, 0);
//      smartArrow(1);
//    } else {
//      lcd.print(F(":"));
//    }
////
////    lcd.print(vals[i]);
//  }

}

void printGUI_Start() {
  lcd.clear();
  lcd.setCursor(0, 0); printFromPGM(&names[0]); lcd.print(vals[0]);
  lcd.setCursor(5, 0); printFromPGM(&names[1]); lcd.print(vals[2]);
  lcd.setCursor(10, 0); printFromPGM(&names[2]); lcd.print(vals[2]);
  lcd.setCursor(0, 1); printFromPGM(&names[3]); lcd.print(vals[3]);

  for (int i = 0; i < SETTINGS_AMOUNT; i++) {

    switch (i) {
      case 0: lcd.setCursor(0, 0);
        break;
      case 1: lcd.setCursor(5, 0);
        break;
      case 2: lcd.setCursor(10, 0);
        break;
      case 3: lcd.setCursor(0, 1);
        break;
    }
    
    printFromPGM(&names[i]);

    if (arrowPos == i) {
      smartArrow(1);
    } else {
      lcd.print(F(":"));
    }

    lcd.print(vals[i]);
  }

  // если курсор находится на выбранной строке
//  smartArrow(1);  // рисуем стрелку или пробел
  
  // выводим имя и значение пункта меню
//  printFromPGM(&names[0]);
//  printFromPGM(&names[LINES * screenPos + i]);
//  lcd.print(F(": "));
//  lcd.print(vals[0]);
//  lcd.print(F("  ")); // пробелы для очистки

  /*
     for (byte i = 0; i < LINES; i++) {  // для всех строк
    lcd.setCursor(0, i);              // курсор в начало

    // если курсор находится на выбранной строке
    smartArrow(arrowPos == LINES * screenPos + i);  // рисуем стрелку или пробел

    // если пункты меню закончились, покидаем цикл for
    if (LINES * screenPos + i == SETTINGS_AMOUNT) break;

    // выводим имя и значение пункта меню
    printFromPGM(&names[LINES * screenPos + i]);
    lcd.print(F(": "));
    lcd.print(vals[LINES * screenPos + i]);
    lcd.print(F("  ")); // пробелы для очистки
  }
  */
}

void smartArrow(bool state) {  // рисует стрелку, галку или пробел
  lcd.write(state ? (chosenPoint ? 62 : 126) : 32); // оооочень крутая штука
}

// очень хитрая функция для печати из PROGMEM
void printFromPGM(int charMap) {
  uint16_t ptr = pgm_read_word(charMap);    // получаем адрес из таблицы ссылок
  while (pgm_read_byte(ptr) != NULL) {      // всю строку до нулевого символа
    lcd.print(char(pgm_read_byte(ptr)));    // выводим в монитор или куда нам надо
    ptr++;                                  // следующий символ
  }
}

/*


uint8_t examinationValPluse(uint8_t val) {
  if (val >= 255) val = 255;
  else val++;
  return val;
}

uint8_t examinationVal(uint8_t val) {
  if (val <= 0) {
    val = 0;
  } else {
    val--;
  }
  if (val == 9 or val == 99) {
    lcd.clear();
  }
  return val;
}

void printGUI() {
  lcd.setCursor(0, 0); lcd.print("R:"); lcd.print(val_R);
  lcd.setCursor(5, 0); lcd.print("G:"); lcd.print(val_G);
  lcd.setCursor(10, 0); lcd.print("B:"); lcd.print(val_B);
  lcd.setCursor(0, 1); lcd.print("Brightness:"); lcd.print(val_Braig);
  // выводим стрелку
  switch (arrowPos) {
    case 0: lcd.setCursor(1, 0);
      break;
    case 1: lcd.setCursor(6, 0);
      break;
    case 2: lcd.setCursor(11, 0);
      break;
    case 3: lcd.setCursor(10, 1);
      break;
  }
  lcd.write(126); // вывести стрелку
}
*/
