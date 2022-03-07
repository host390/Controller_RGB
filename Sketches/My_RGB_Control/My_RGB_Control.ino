

/*
 * Тут будет шапка кто создатель там какой год и тд и тп рекламма...
 */

/*
 * Нужно добавить:
 * 1. (Д)Индикатор зарядки.
 * 2. Сохранение значений (что-то написал).
 * 3. Исправить отображение значений (что-то делал).
 * 4. Исправить галочки активных (так вроде робит, но при первом заходе на 2 экран, галочка не отображается).
 * 5. Битмапы на главный (есть, но не особо оптимизированно написано + немного можно дорисовать иконки).
 * 6. (О) Оптимизировать и выводить текст в Color Set 
 * 7. 
 * 
 * 
 * 
 */


#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля
RF24 radio(9, 10); 

#define USE_MICRO_WIRE
#define OLED_SOFT_BUFFER_64     // Буфер на стороне МК
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

#define But_UP 8
#define But_OK 9
#define But_DOWN 10
#include <GyverButton.h>
GButton up(But_UP); // вверх
GButton down(But_DOWN); // вниз
GButton ok(But_OK); // ок

#define DT 2
#define CLK 3
#define SW 4
#include <GyverEncoder.h>
Encoder encod(CLK, DT, SW, TYPE1);  // для работы c кнопкой и сразу выбираем тип


#include <Keypad.h> // подключаем библиотеку для управление клавиатурой
const byte ROWS = 4; //число строк у нашей клавиатуры
const byte COLS = 4; //число столбцов у нашей клавиатуры
char hexaKeys[ROWS][COLS] = {// здесь мы располагаем названия наших клавиш, как на клавиатуре, для удобства пользования
  {'0','4','8','C'}, 
  {'1','5','9','D'},
  {'2','6','A','F'},
  {'3','7','B','G'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //к каким выводам подключаем управление строками
byte colPins[COLS] = {A0, A1, A2, A3}; //к каким выводам подключаем управление столбцами
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


#include <avr/eeprom.h>
#include <EEPROM.h>

#define RESERV_ADDR 1010  // номер резервной ячейки
#define RESERV_KEY 60 // ключ первого запуска


// Радио
//byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
const uint32_t pipe = 111156789; // адрес рабочей трубы;


// Переменные
bool moveMainMenu = false;

// Флаги
bool flag_function = false; // выбран ...
bool flag_params = false; // выбран функция
bool flag_value = false; // выбран параметр функции
bool oneTranss = true; // для отправки значений 1 раз при вхоже в параметре выборе активного



int maxParams = 0; // максимальное количество параметро для данного пункта
int SumNumber = 0; // чтоб найти нужный текст в labelsColors

int stepParams = 0; // каунтер для параметров

/* Шагание по меню */
int navPos = 0; // номер пункта главного меню

byte navPos_activ; // номер пункта меню где находится активный элемент

bool checked_flag = false; // для отрисовки галочки активного пункта в нужном окне

int16_t modeNum = 0; // номер режима
int activeNumb = 0; // активный режим
byte menuFunction = 0; // счётчик для функций меню
int16_t presetNum = 0; // номер, но для чего ?_? ?_?
int16_t modeParams = 0; // номера параметров режимов

/* Отрисовка */
int indexDown = 0; // индекс опускания меню


// byte writeArrRF[2];
int writeArrRF[2]; // массив для отправки по радио 3ий это для завершения передачи 1 значчения

int limitIncr; // лимит для инкремента

bool openOption = false;
bool openPatams = false;
bool startProgram = true;
long timeTransRF; // таймер для отправки одиночных значений



#define MENU_ITEMS 2 // Общее коли-во пунктов меню
//const char *modeNames[] = { // список названий режимов
//  "Цвета", "Эффекты",
//};

const char Colors [] PROGMEM = "Цвета";
const char Effects [] PROGMEM = "Эффекты";

const char* const modeNames[MENU_ITEMS] PROGMEM = {
  Colors, Effects,
};


// Цвета
const char RGB[] PROGMEM = "RGB_Channels";
const char HSV[] PROGMEM = "HSV_Colors";
const char FHSV[] PROGMEM = "FHSV_Colors";
const char CW[] PROGMEM = "colorWheel";
const char HEXC[] PROGMEM = "HEX_Colors";

// Эффекты
const char CW_EF[] PROGMEM = "colorWheel";
const char Fire[] PROGMEM = "Fire";
const char FireManual[] PROGMEM = "FireManual";
const char Strobe[] PROGMEM = "Strobe";
const char RndStrobe[] PROGMEM = "RndStrobe";
const char Police[] PROGMEM = "Police";


#define EFFECTS_PUNCTS 6 // Пункты Эффектов

// COLOR_PUNCTS + EFFECTS_PUNCTS можно записать в 1, а можно и в разных как лучше???
#define COLOR_PUNCTS 5 // Пункты Цветов

const char* const labelsColors[COLOR_PUNCTS + EFFECTS_PUNCTS] PROGMEM = {
  RGB, HSV, FHSV, CW, HEXC,
  CW_EF, Fire, FireManual, Strobe, RndStrobe, Police,    
};

//#define EFFECTS_PUNCTS 1 // Пункты Эффектов
//const char* const labelsEffects[EFFECTS_PUNCTS] PROGMEM = {
//  CW6, // CW7, CW8, CW9,
//};


const char channel_R[] PROGMEM = "R:";
const char channel_G[] PROGMEM = "G:";
const char channel_B[] PROGMEM = "B:";
const char Brightness_RGB[] PROGMEM = "Brightness:";

const char color_HSV[] PROGMEM = "Color:";
const char saturation_HSV[] PROGMEM = "Saturation:";
const char Brightness_HSV[] PROGMEM = "Brightness:";

const char color_FHSV[] PROGMEM = "Color:";
const char saturation_FHSV[] PROGMEM = "Saturation:";
const char Brightness_FHSV[] PROGMEM = "Brightness:";

const char colorW[] PROGMEM = "Color:";
const char colorW_Brightness[] PROGMEM = "Brightness:";

const char HEX_Color[] PROGMEM = "Color:";
const char HEXC_Brightness[] PROGMEM = "Brightness:";


const char CW_EF_Speed[] PROGMEM = "Speed:";
const char CW_EF_Step[] PROGMEM = "Step:";
const char CW_EF_Brightness[] PROGMEM = "Brightness:";

/*
 CW_EF, Fire, FireManual, Strobe, RndStrobe, Police,    
  
  Fire, // макс яркость 0-255
        // мин яркость 0-255
        // скорость 0-1000
        
  FireManual, // цвет 0-255
              // скорость 0-1000
              // макс яркость 0-255
              // мин яркость 0-255
          
  Strobe, // Цвет 0-255
          // Насышенность 0-255
          // Яркость 0-255
          // Скорость 0-1000
  
  RndStrobe, // Цвет 0-255
             // Насышенность 0-255
             // Яркость 0-255
             // Скорость 0-1000
  
  Police, // Яркость 0-255
          // Скорость 0-1000
*/

const char Fire_max[] PROGMEM = "Max_Bright:";
const char Fire_min[] PROGMEM = "Min_Bright:";
const char Fire_speed[] PROGMEM = "Speed:";

const char FireManual_col[] PROGMEM = "Color:";
const char FireManual_max[] PROGMEM = "Max_Bright:";
const char FireManual_min[] PROGMEM = "Min_Bright:";
const char FireManual_speed[] PROGMEM = "Speed:";

const char Strobe_col[] PROGMEM = "Color:";
const char Strobe_sat[] PROGMEM = "Saturation:";
const char Strobe_brig[] PROGMEM = "Brightness:";
const char Strobe_speed[] PROGMEM = "Speed:";

const char RndStrobe_col[] PROGMEM = "Color:"; // Color:
const char RndStrobe_sat[] PROGMEM = "Saturation:";
const char RndStrobe_brig[] PROGMEM = "Brightness:";
const char RndStrobe_speed[] PROGMEM = "Speed:";

const char Police_brig[] PROGMEM = "Brightness:";
const char Police_speed[] PROGMEM = "Speed:";

//const char *colorNames[] = { // всего 16 значений как и кнопок нужно оптимизировать вывод
//  "WHITE",
//  "SILVER",
//  "GRAY",
//  "BLACK",
//  "RED",
//  "MAROON",
//  "YELLOW",
//  "OLIVE",
//  "LIME",
//  "GREEN",
//  "AQUA",
//  "TEAL",
//  "BLUE",
//  "NAVY",
//  "PINK",
//  "PURPLE",
//};


const char* const paramsColor[] PROGMEM = {
   channel_R, channel_G, channel_B, Brightness_RGB,
   color_HSV, saturation_HSV, Brightness_HSV,
   color_FHSV, saturation_FHSV, Brightness_FHSV,
   colorW, colorW_Brightness,
   HEX_Color, HEXC_Brightness, // HEX

   CW_EF_Speed, CW_EF_Step, CW_EF_Brightness, // CW_EF
   Fire_max, Fire_min, Fire_speed, // Fire
   FireManual_col, FireManual_max, FireManual_min, FireManual_speed, // FireManual 
   Strobe_col, Strobe_sat, Strobe_brig, Strobe_speed, // Strobe
   RndStrobe_col, RndStrobe_sat, RndStrobe_brig, RndStrobe_speed, // RndStrobe
   Police_brig, Police_speed, // Police
};

int increment = 0; // для изменения значений vals

#define ACTIVENUMB 34 // 34
//#define NAVPOS_ACTIV 36 // 34
int vals[35] = { // массив "Цветов" 
  100, 100, 100, 100, // R G B Brightness 0-3
  100, 100, 100, // Color Saturation Brightness 4-6
  100, 100, 100, // Color_F Saturation_F Brightness_F 7-9
  40, 10, // colorWheel 10-11
  2, 120, // color_HEX 12-13
  
  1000, 500, 100, // CW_EF 14-16
  250, 250, 1000,  // Fire 17-19
  250, 250, 250, 1000,  // FireManual 20-23
  250, 250, 250, 1000,  // Strobe 24-27
  250, 250, 250, 1000,  // RndStrobe 28-31 28-32
  250, 1000,  // Police 32-33

  0, // activeNumb 34
  //0, // navPos_activ 35
}; 

void setup() {
  Serial.begin(9600);
  //pinMode(TEST_LED, OUTPUT);

  // Радио
  
  //radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  //radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  //radio.setPayloadSize(32);   // размер пакета, в байтах
  //radio.begin();              // активировать модуль
  //radio.setAutoAck(false);
  //radio.openWritingPipe(address[0]);  // мы - труба 0, открываем канал для передачи данных
  //radio.setChannel(0x60);             // выбираем канал (в котором нет шумов!)
  //radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  //radio.setDataRate (RF24_1MBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  //radio.powerUp();        // начать работу
  //radio.stopListening();  // не слушаем радиоэфир, мы передатчик

  radio.begin(); // инициализация
  delay(2000);

  
  radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.setCRCLength(RF24_CRC_8); // размер контрольной суммы 8 bit или 16 bit
  radio.setPALevel(RF24_PA_MAX); // уровень питания усилителя RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  radio.setChannel(0x6f);         // установка канала
  
  radio.setAutoAck(false); // автоответ false
  
  radio.powerUp();               // включение или пониженное потребление powerDown - powerUp
  radio.stopListening();  //радиоэфир не слушаем, только передача
  radio.openWritingPipe(pipe);   // открыть трубу на отправку
  

  oled.init();
  oled.setContrast(255);
  oled.setPower(1);
  Wire.setClock(800000L); // скорость I2C макс. 800'000 400000L // а сколько по умолчанию ????
  oled.clear();// очистить дисплей (или буфер)
  oled.update(); // обновить. Только для режима с буфером! OLED_BUFFER
  oled.home();

  //encod.setFastTimeout(60); // таймаут на скорость isFastR. По умолч. 50 Вот как сделать так, чтоб он нормально отрабатывал????
  encod.setFastTimeout(100);
  
  up.setTickMode(AUTO);
  down.setTickMode(AUTO);
  ok.setTickMode(AUTO);

  //writeArrRF[2] = false; // значение которрое для выхода из параметров
  
  changePreset(); // отрисовывем всё

  // Первый старт программы
  if (EEPROM.read(RESERV_ADDR) != RESERV_KEY) { // если значение в резервной ячейки не равно 50
    EEPROM.write(RESERV_ADDR, RESERV_KEY); // записали ключ
    EEPROM.put(0, vals); // отправляю в еерум значенияя заводских настроек
  }

  // Читаем все данные с EEPROM i < 1023
  Serial.println(F("---------EEPROM---------"));
  for (int i = 0; i < sizeof(vals); i++) {
    Serial.print(EEPROM.read(i)); Serial.print(" ");
    if (i == 7 or i == 13 or i == 19 or i == 23 or i == 27 or i == 33 or i == 39 or i == 47 or i == 55 or i == 63 or i == 67) {
      Serial.println();
    }
  }
  Serial.println();
  /*
  Serial.println(F("---------EEPROM---------"));
  for (int i = 1000; i < 1020; i++) {
    Serial.print(EEPROM.read(i)); Serial.print(" ");
    if (i != 0 and i % 9 == 0) Serial.println(); // каждые 10 значений переносим строку (% это тип кратно...)
  }
  Serial.println();
  */
  EEPROM.get(0, vals); // читаем значения vals

  activeNumb = vals[ACTIVENUMB];
  Serial.print(F("activeNumb(Начальный): ")); Serial.println(activeNumb);
  
  navPos_activ = EEPROM[100]; // В 100 ЗНАЧЕНИЕ СОХРАНИЛИ
  Serial.print(F("navPos_activ(Начальный): ")); Serial.println(navPos_activ);
  
  Serial.println(F("---------ВСЕ ЗНАЧЕНИЯ vals---------"));
  for (int i = 0; i < sizeof(vals) / 2; i++) {
    Serial.print(vals[i]); Serial.print(","); Serial.print(" ");
    if (i == 3 or i == 6 or i == 9 or i == 11 or i == 13 or i == 16 or i == 19 or i == 23 or i == 27 or i == 31 or i == 33 or i == 34) {
      Serial.println();
    }
  }

  

  
}



void loop() {
//  encod.tick();
//  if (encod.isClick()) Serial.println("Click");
//  if (encod.isSingle()) Serial.println("Single");
//  if (encod.isDouble()) Serial.println("Double");
//  return;
  
  controlTick(); // управление
  butMatrixTick(); // просмотр кнопок
  pushFunction(); // отправка данных
}
