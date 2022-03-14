/*
  Скетч пульта RGB 
  Видео на Ютубе: https://
  Исходники на GitHub: https://
  Автор: EleKtRom4k, 2022
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define CE 9
#define CSN 10

RF24 radio(CE, CSN);

#define USE_MICRO_WIRE
#define OLED_SOFT_BUFFER_64 // Буфер на стороне МК
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

#define DT 2
#define CLK 3
#define SW 4
#include <GyverEncoder.h>
Encoder encod(CLK, DT, SW, TYPE1); // для работы c кнопкой и сразу выбираем тип

#include <Keypad.h> // подключаем библиотеку для управление клавиатурой
const byte ROWS = 4; //число строк у нашей клавиатуры
const byte COLS = 4; //число столбцов у нашей клавиатуры
char hexaKeys[ROWS][COLS] = {// здесь мы располагаем названия наших клавиш, как на клавиатуре, для удобства пользования
  {'0', '4', '8', 'C'},
  {'1', '5', '9', 'D'},
  {'2', '6', 'A', 'F'},
  {'3', '7', 'B', 'G'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //к каким выводам подключаем управление строками
byte colPins[COLS] = {A0, A1, A2, A3}; //к каким выводам подключаем управление столбцами
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

#include <avr/eeprom.h>
#include <EEPROM.h>

#define RESERV_ADDR 1010 // номер резервной ячейки
#define RESERV_KEY 60 // ключ первого запуска

const uint32_t pipe = 111156789; // адрес рабочей трубы;
int writeArrRF[2]; // массив для отправки по радио 3ий это для завершения передачи 1 значчения

bool moveMainMenu = false;
bool flag_function = false; // выбран ...
bool flag_params = false; // выбран функция
bool flag_value = false; // выбран параметр функции
bool oneTranss = true; // для отправки значений 1 раз при вхоже в параметре выборе активного
int maxParams = 0; // максимальное количество параметро для данного пункта
int SumNumber = 0; // чтоб найти нужный текст в labelsColors
int stepParams = 0; // каунтер для параметров
int navPos = 0; // номер пункта главного меню
byte navPos_activ; // номер пункта меню где находится активный элемент
bool checked_flag = false; // для отрисовки галочки активного пункта в нужном окне
int16_t modeNum = 0; // номер режима
int activeNumb = 0; // активный режим
byte menuFunction = 0; // счётчик для функций меню
int16_t presetNum = 0; // номер, но для чего ?_? ?_?
int16_t modeParams = 0; // номера параметров режимов
int indexDown = 0; // индекс опускания меню
int limitIncr; // лимит для инкремента
bool openOption = false;
bool openPatams = false;
bool startProgram = true;
long timeTransRF; // таймер для отправки одиночных значений


#define MENU_ITEMS 2 // Общее коли-во пунктов меню

const char Colors [] PROGMEM = "Цвета";
const char Effects [] PROGMEM = "Эффекты";

const char* const modeNames[MENU_ITEMS] PROGMEM = {
  Colors, Effects,
};

// Цвета
const char RGB[] PROGMEM = "RGB_Channels";
const char HSV[] PROGMEM = "HSV_Colors";
const char CW[] PROGMEM = "СolorWheel";
const char HEXC[] PROGMEM = "HEX_Colors";
const char KELV[] PROGMEM = "Kelvin";

// Эффекты
const char CW_EF[] PROGMEM = "colorWheel";
const char Fire[] PROGMEM = "Fire";
const char FireManual[] PROGMEM = "FireManual";
const char Strobe[] PROGMEM = "Strobe";
const char FadeTo[] PROGMEM = "FadeTo";
const char rainbowBreath[] PROGMEM = "rainbowBreat";

#define EFFECTS_PUNCTS 6 // Пункты Эффектов
#define COLOR_PUNCTS 5 // Пункты Цветов

const char* const labelsColors[COLOR_PUNCTS + EFFECTS_PUNCTS] PROGMEM = {
  RGB, HSV, CW, HEXC, KELV,
  CW_EF, Fire, FireManual, Strobe, FadeTo, rainbowBreath,
};

const char *colorValsHEX[] = {
  "RED", "GREEN", "BLUE", "WHILE", "LIME", "PINK",
  "YELLOW", "PURPLE", "OLIVE", "ORANGE", "BROWN",
  "MINT", "AMBER", "INDIGO", "BEIGE", "FUCUS",
};

// Цвета
const char channel_R[] PROGMEM = "R:";
const char channel_G[] PROGMEM = "G:";
const char channel_B[] PROGMEM = "B:";
const char Brightness_RGB[] PROGMEM = "Brightness:";

const char color_HSV[] PROGMEM = "Color:";
const char saturation_HSV[] PROGMEM = "Saturation:";
const char Brightness_HSV[] PROGMEM = "Brightness:";

const char colorW[] PROGMEM = "Color:";
const char colorW_Brightness[] PROGMEM = "Brightness:";

const char HEX_Color[] PROGMEM = "Color:";
const char HEXC_Brightness[] PROGMEM = "Brightness:";

const char KELV_Temp[] PROGMEM = "Temperature:";
const char KELV_Brightness[] PROGMEM = "Brightness:";

// Эффекты
const char CW_EF_Speed[] PROGMEM = "Speed:";
const char CW_EF_Step[] PROGMEM = "Step:";
const char CW_EF_Brightness[] PROGMEM = "Brightness:";

const char Fire_max[] PROGMEM = "Min_Bright:";
const char Fire_min[] PROGMEM = "Max_Bright:";
const char Fire_speed[] PROGMEM = "Speed:";

const char FireManual_col[] PROGMEM = "Color:";
const char FireManual_max[] PROGMEM = "Min_Bright:";
const char FireManual_min[] PROGMEM = "Max_Bright:";
const char FireManual_speed[] PROGMEM = "Speed:";

const char Strobe_col[] PROGMEM = "Color:";
const char Strobe_sat[] PROGMEM = "Saturation:";
const char Strobe_brig[] PROGMEM = "Brightness:";
const char Strobe_speed[] PROGMEM = "Speed:";

const char FadeTo_color1[] PROGMEM = "Color#1:";
const char FadeTo_color2[] PROGMEM = "Color#2:";
const char FadeTo_speed[] PROGMEM = "Speed:";

const char rainbowBreath_speed[] PROGMEM = "Speed:";
const char rainbowBreath_step[] PROGMEM = "Step:";


const char* const paramsColor[] PROGMEM = {
  channel_R, channel_G, channel_B, Brightness_RGB,
  color_HSV, saturation_HSV, Brightness_HSV,
  colorW, colorW_Brightness,
  HEX_Color, HEXC_Brightness,
  KELV_Temp, KELV_Brightness,

  CW_EF_Speed, CW_EF_Step, CW_EF_Brightness, // CW_EF
  Fire_max, Fire_min, Fire_speed, // Fire
  FireManual_col, FireManual_max, FireManual_min, FireManual_speed, // FireManual
  Strobe_col, Strobe_sat, Strobe_brig, Strobe_speed, // Strobe
  FadeTo_color1, FadeTo_color2, FadeTo_speed, // FadeTo
  rainbowBreath_speed, rainbowBreath_step, // rainbowBreath
};

int increment = 0; // для изменения значений vals

#define ACTIVENUMB 32

int vals[35] = { // массив всех значений
  80, 100, 0, 100, // (RGB_Channels) R, G, B, Brightness [0-255], 0, 1, 2, 3,
  200, 230, 100, // (HSV_Colors) Color, Saturation, Brightness [0-255], 4, 5, 6,
  100, 100, // (colorWheel) Color, Brightness [0-255], 7, 8,
  1, 100, // (color_HEX) Color [0-15], Brightness [0-255], 9, 10,
  10, 200, // (Kelvin) Temperature , Brightness [0-255], 11, 12,

  10, 5, 100, // (ColorWheel) Speed, Step, Brig, 13, 14, 15,
  30, 200, 360, // (Fire) min_Brig, max_Brig, time_change, 16, 17, 18,
  40, 360, 20, 130, // (FireManual) Color, Speed, min_Brig, max_Brig, 19, 20, 21, 22,
  160, 200, 120, 100, // (Strobe) Color, Saturation, Brightness, Speed, 23, 24, 25, 26,
  12, 7, 1000, // (FadeTo) Color#1, Color#2, Time, 27, 28, 29,
  10, 40, // (rainbowBreath) Speed, Step, 30, 31,

  0, // activeNumb 32
};

void setup() {
  // Serial.begin(9600);

  radio.begin(); // инициализация
  delay(1000);
  radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.setCRCLength(RF24_CRC_8); // размер контрольной суммы 8 bit или 16 bit
  radio.setPALevel(RF24_PA_MAX); // уровень питания усилителя RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  radio.setChannel(0x6f);         // установка канала
  radio.setAutoAck(false); // автоответ false
  radio.powerUp(); // включение
  radio.stopListening(); // радиоэфир не слушаем, только передача
  radio.openWritingPipe(pipe); // открыть трубу на отправку

  oled.init();
  oled.setContrast(255);
  oled.setPower(1);
  Wire.setClock(800000L); // скорость I2C макс.
  oled.clear(); // очистить дисплей (или буфер)
  oled.update(); // обновить. Только для режима с буфером! OLED_BUFFER
  oled.home();

  encod.setFastTimeout(100);

  //writeArrRF[2] = false; // значение которрое для выхода из параметров

  changePreset(); // отрисовывем всё

  // Первый старт программы
  if (EEPROM.read(RESERV_ADDR) != RESERV_KEY) { // если значение в резервной ячейки не равно 50
    EEPROM.write(RESERV_ADDR, RESERV_KEY); // записали ключ
    EEPROM.put(0, vals); // отправляю в еерум значенияя заводских настроек
  }
  EEPROM.get(0, vals); // читаем значения vals
  activeNumb = vals[ACTIVENUMB];
  // Serial.print(F("activeNumb(Начальный): ")); Serial.println(activeNumb);

  navPos_activ = EEPROM[100]; // В 100 ЗНАЧЕНИЕ СОХРАНИЛИ
  // Serial.print(F("navPos_activ(Начальный): ")); Serial.println(navPos_activ);
}

void loop() {
  controlTick(); // управление
  butMatrixTick(); // просмотр кнопок
  pushFunction(); // отправка данных
}
