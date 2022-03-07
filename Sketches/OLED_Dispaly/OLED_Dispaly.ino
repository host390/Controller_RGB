

//#include <microWire.h>
#define USE_MICRO_WIRE
#define OLED_SOFT_BUFFER_64     // Буфер на стороне МК

#include <GyverOLED.h>


#define But_UP 8
#define But_OK 9
#define But_DOWN 10


//GyverOLED oled(0x3C); // ???

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
//GyverOLED<SSD1306_128x64, OLED_BUFFER> oled(0x3C); OLED_NO_BUFFER
//GyverOLED oled(0x3C);

#include <GyverButton.h>
GButton up(But_UP); // вверх
GButton down(But_DOWN); // вниз
GButton ok(But_OK); // ок

const uint8_t bitmap_32x32[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0xF0, 0x70, 0x70, 0x30, 0x30, 0x30, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF0, 0x70, 0x30, 0x30, 0x20, 0x00, 0x00,
  0x00, 0x30, 0x78, 0xFC, 0x7F, 0x3F, 0x0F, 0x0F, 0x1F, 0x3C, 0x78, 0xF0, 0xE0, 0xC0, 0x80, 0x80, 0x80, 0x40, 0xE0, 0xF0, 0xF8, 0xFC, 0xFF, 0x7F, 0x33, 0x13, 0x1E, 0x1C, 0x1C, 0x0E, 0x07, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF9, 0xF7, 0xEF, 0x5F, 0x3F, 0x7F, 0xFE, 0xFD, 0xFB, 0xF1, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x1E, 0x33, 0x33, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00,
};

const uint8_t testBit7x7[] PROGMEM = {
//  0x40, 0x60, 0x77, 0x77, 0x77, 0x60, 0x40
// Это полчается битмап 7 на 7
  B1000000,
  B1100000,
  B1110111,
  B1110111,
//  B1110111,
//  B1100000,
//  B1000000
};

//const uint8_t box[] PROGMEM = {
//  
//}


const uint8_t testBit8x8[] PROGMEM = {
// 0xff, 0xff, 0xff, 0x7e, 0x7e, 0x3c, 0x3c, 0x18
// B11111110, B11111111, B11111110,  // высота y
// B01111111, B11111111, B01111111,  // ширина x
//  B1011, 
//  B1011, 
//  B1000, 
//  B1011,
//  B1011

//  B00010000, B00100000, B11001101, B11001101, B11100001, B11001101, B11001101, B00100000, B00010000,
//  B11,       B00,       B00,       B00,       B00,       B00,       B00,       B00,       B00,

  B01000000, B10000000, B00110100, B00110111, B10000111, B00110111, B00110100, B10000000, B01000000,
  B00,       B00,       B11,       B11,       B11,       B11,       B11,       B00,       B00,                                      
  
};


#define POT 0
#define ITEMS 8 // Общее кол во пунктов


int valuePOT = 0;

const uint8_t icons_7x7[][7] PROGMEM = {
  {0x3e, 0x41, 0x55, 0x51, 0x55, 0x41, 0x3e},
  {0x1c, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e}, //9 battery 100%
  {0x1c, 0x3e, 0x22, 0x3e, 0x3e, 0x3e, 0x3e}, //10 battery 75%
  {0x1c, 0x3e, 0x22, 0x22, 0x3e, 0x3e, 0x3e}, //11 battery 50%
  {0x1c, 0x3e, 0x22, 0x22, 0x22, 0x3e, 0x3e}, //12 battery 25%
  {0x1c, 0x3e, 0x22, 0x22, 0x22, 0x22, 0x3e}, //13 battery 0%
};

const uint8_t ptr_bmp[] PROGMEM = {
  0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0xFF, 0xFF, 0x7E, 0x3C, 0x18,
};

/*
// массивы строк с именами пунктов 
const char i0[] PROGMEM = "   Parameter 0:";  // Каждое имя в своей строке, символ переноса не нужен
const char i1[] PROGMEM = "   Parameter 1:";
const char i2[] PROGMEM = "   Parameter 2:";
const char i3[] PROGMEM = "   Parameter 3:";
const char i4[] PROGMEM = "   Parameter 4:";
const char i5[] PROGMEM = "   Parameter 5:";
const char i6[] PROGMEM = "   Parameter 6:";
const char i7[] PROGMEM = "   Parameter 7:";

const char* const names[] PROGMEM =           // Создаем массив ссылок, обращение к названию пунктов по их номеру
{
  i0, i1, i2, i3,
  i4, i5, i6, i7
};
*/

#define MENU_NUMB 2

void setup() {
  pinMode(POT, INPUT);
  Serial.begin(9600);
  
  oled.init();
  oled.setContrast(255);
  oled.setPower(1);
  Wire.setClock(800000L);   // скорость I2C макс. 800'000 // а сколько по умолчанию ????

  oled.autoPrintln(true);
  oled.clear();   // очистить дисплей (или буфер)
  oled.update();  // обновить. Только для режима с буфером! OLED_BUFFER
  oled.home();
  
  oled.textMode(BUF_ADD);
  // BUF_ADD - наложить текст
  // BUF_SUBTRACT - вычесть текст
  // BUF_REPLACE - заменить (весь прямоугольник буквы)
  
  
//  oled.drawBitmap(0, 0, icons_7x7[0], 7, 7, BITMAP_NORMAL, BUF_ADD); // mode BUF_ADD / BUF_SUBTRACT / BUF_REPLACE
//  oled.drawBitmap(8, 0, icons_7x7[1], 7, 7, BITMAP_NORMAL, BUF_ADD); // mode BUF_ADD / BUF_SUBTRACT / BUF_REPLACE
//  oled.drawBitmap(16, 0, icons_7x7[2], 7, 7, BITMAP_NORMAL, BUF_ADD); // mode BUF_ADD / BUF_SUBTRACT / BUF_REPLACE
//  oled.drawBitmap(24, 0, icons_7x7[3], 7, 7, BITMAP_NORMAL, BUF_ADD); // mode BUF_ADD / BUF_SUBTRACT / BUF_REPLACE
//  oled.drawBitmap(32, 0, icons_7x7[4], 7, 7, BITMAP_NORMAL, BUF_ADD); // mode BUF_ADD / BUF_SUBTRACT / BUF_REPLACE
//  oled.update();
  

  
}



/* пример вложеной функции, которую можно вызвать из под меню */
void func(void) {
  oled.clear();
  oled.home();
  oled.print(F("Press OK to return"));
  oled.update();
  while (1) {
    ok.tick();
    if (ok.isClick()) return; // return возвращает нас в предыдущее меню
  }
}

void setings (void) {
//  oled.clear();
//  oled.home();
//  oled.update();
  
  while (1) {
    oled.clear();
    oled.home();
    oled.println(F("Яркость"));
    oled.print(F("экрана:"));
    valuePOT = map(analogRead(POT), 0, 1023, 0, 255);
    valuePOT = constrain(valuePOT, 0, 255);
    oled.print(valuePOT);
    
    oled.setContrast(valuePOT);
    
//    oled.drawBitmap(0, 12, bitmap_32x32, 32, 32, BITMAP_NORMAL, BUF_ADD);
    oled.update();
    ok.tick();
    if (ok.isClick()) return;
  }
}

/*
// Функция для печати строки из prm
void printMenuItem(uint8_t num) { // Я хрен его знает, что здесь происходит, но выглядит круто :))))))
  char buffer[21];                                // Буфер на полную строку
  uint16_t ptr = pgm_read_word(&(names[num]));    // Получаем указатель на первый символ строки
  uint8_t i = 0;                                  // Переменная - счетчик

  do {                                            // Начало цикла
    buffer[i] = (char)(pgm_read_byte(ptr++));     // Прочитать в буфер один символ из PGM и подвинуть указатель на 1
  } while (buffer[i++] != NULL);                  // Если это не конец строки - вернуться в начало цикла

  oled.println(buffer);                           // Вывод готовой строки с переносом на следующую
}
*/



void loop() {

//  printBit();
//  
//  return;
  
  static int8_t pointer = 0; // как понимать static ????
  
  up.tick();
  down.tick();
  ok.tick();
  
  if (up.isClick()) {              
    pointer = constrain(pointer - 1, 0, MENU_NUMB - 1); // Двигаем указатель вверх
    Serial.println(pointer);
  }

  if (down.isClick()) {              
    pointer = constrain(pointer + 1, 0, MENU_NUMB - 1); // Двигаем указатель вниз
    Serial.println(pointer);
  }


  if (ok.isClick()) {   // Нажатие на ОК - переход в пункт меню
    Serial.println("Click");
    switch (pointer) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
      case 0: func(); break;  // По нажатию на ОК при наведении на 0й пункт вызвать функцию
      case 1: setings(); break;
      case 2: break;
      case 3: break;
      case 4: break;
      case 5: break;
      case 6: break;
      case 7: break;
    }
  }
  
  
  /* меню */
  oled.clear();           // Очищаем буфер
  oled.home();            // Курсор в левый верхний угол
  oled.setCursor(0, 0);
  oled.setScale(2);

  if (pointer == 0) {
    oled.print("   Цвета");
  } else if (pointer == 1) {
    oled.print("  Эффекты");
  } 
//  else if (pointer == 2) {
//    oled.print("  Эффекты");
//  }

  // 11 это сещение на 11 пикселей :))))
  oled.rect(2 + 11, 18, 45 + 11, 61); // битмап
  oled.rect(81 - 11, 18, 125 - 11, 61); // битмап
  
  
//  oled.roundRect(0, 16, 127, 63, OLED_STROKE); // рамочка для меню
  
//  oled.setCursor(0, 2);
//  oled.drawBitmap(1, 20, bitmap_32x32, 32, 32, BITMAP_NORMAL, BUF_ADD);
//  oled.drawBitmap(35, 20, bitmap_32x32, 32, 32, BITMAP_NORMAL, BUF_ADD);
//  oled.drawBitmap(69, 20, bitmap_32x32, 32, 32, BITMAP_NORMAL, BUF_ADD);
//  oled.drawBitmap(103, 20, bitmap_32x32, 32, 32, BITMAP_NORMAL, BUF_ADD);

/*
  for (uint8_t i = 0; i < 8; i++) { // Цикл, выводящий 8 пунктов на дисплей
    printMenuItem(i);               // Выводим пункты по номерам
  }
*/

//  for (int i = 0; i < 8; i++) { // ОГРОМНЫЙ МИНУС В ТОМ что нельзя задавать разные имена ха-ха
//    oled.setCursor(0, i);
//    oled.print(F("  Parameter "));
//    oled.print(i);
//    oled.print(F(":"));
//  }

  printPointer(pointer);

//  oled.print( Так не выводится и я хз почему -\\- ???
//    "0:\n"   // Не забываем про '\n' - символ переноса строки
//    "1:\n"
//    "2:\n"
//    "3:\n"
//    "4:\n"
//    "5:\n"
//    "6:\n"
//    "7:\n"
//  );

   
//  printPointer(pointer);  // Вывод указателя
  oled.update();  







  
  /*
  valuePOT = map(analogRead(POT), 0, 1023, 0, 255);
  valuePOT = constrain(valuePOT, 0, 255);

//  if (valuePOT < 999) { // если значение стало < 999, то стереть 1 раз
//    oled.clear();
//    oled.home();
//  }
  
  Serial.print("Сигнал: ");
  Serial.println(valuePOT);
  
  oled.clear();
  oled.home();
  oled.print("Сигнал: ");
  oled.print(valuePOT);
  

  oled.drawBitmap(0, 12, bitmap_32x32, 32, 32, BITMAP_NORMAL, BUF_ADD); // mode BUF_ADD / BUF_SUBTRACT / BUF_REPLACE
//  delay(300);

  oled.update();

  */
}
