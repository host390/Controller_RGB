/*
	 Скетч контроллера RGB
   Видео на Ютубе: https://
   Исходники на GitHub: https://
	 Автор скетча: EleKtRom4k, 2022
	 За основу взят проект Alexa...
*/


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define CE 7
#define CSN 8

RF24 radio(CE, CSN);

const uint32_t pipe = 111156789; // адрес рабочей трубы

#define PIN_R 10
#define PIN_G 9
#define PIN_B 3

#include <GyverRGB.h>
GRGB strip(PIN_R, PIN_G, PIN_B);  // куда подключены цвета (R, G, B)


const uint32_t colorValsHEX[] = { // 16 ЦВЕТОВ
  0xFF0000, // RED
  0x008000, // GREEN
  0x0000FF, // BLUE
  0xFFFFFF, // WHITE
  0x00FF00, // LIME
  0xFF00FF, // PINK
  0xFFFF00, // YELLOW
  0x800080, // PURPLE
  0x808000, // OLIVE
  0xFF3300, // ORANGE
  0x663300, // BROWN
  0x00ffff, // MINT
  0xFFCC00, // AMBER
  0x663399, // INDIGO
  0xFFCC99, // BEIGE
  0XFF66FF, // FUCUSIA
};

#define ACTIVENUMB 32
int vals_colors[35] = { // массив "Цветов"
  80, 100, 0, 100, // (RGB_Channels) R, G, B, Brightness [0-255], 0, 1, 2, 3,
  200, 230, 100, // (HSV_Colors) Color, Saturation, Brightness [0-255], 4, 5, 6,
  100, 100, // (colorWheel) Color, Brightness [0-255], 7, 8,
  5, 100, // (color_HEX) Color [0-15], Brightness [0-255], 9, 10,
  100, 100, // (Kelvin) Temperature [0-255], Brightness [0-255], 11, 12,

  10, 5, 100, // (ColorWheel) Speed, Step, Brig [0-255], 13, 14, 15,
  30, 200, 360, // (Fire) min_Brig, max_Brig [0-255], Speed [0-1000], 16, 17, 18,
  40, 360, 20, 130, // (FireManual) Color, min_Brig, max_Brig [0-255], Speed [0-1000], 19, 20, 21, 22,
  160, 200, 120, 100, // (Strobe) Color, Saturation, Brightness [0-255], Speed[0-1000], 23, 24, 25, 26,
  12, 7, 1000, // (FadeTo) Color#1, Color#2 [0-15], Time [0-3000], 27, 28, 29,
  10, 40, // (rainbowBreath) Speed [0-1000], Step [0-500], 30, 31,
  0, // activeNumb, 32
};


int activeNumb = 2; // активный режим
int thisBrightness = 150; // общая яркость
long timer = 0;
uint32_t effTimer; // таймер на накт эффекта
uint16_t effPeriod; // сколько раз в микросекунду
int thisColor; // цвета для вывода в эффекте
boolean strobeFlag; // чтоб стробоскоп включался и выключался

void setup() {
  //	Serial.begin(9600);

  radio.begin();  // инициализация
  delay(1000);

  radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.setCRCLength(RF24_CRC_8); // размер контрольной суммы 8 bit или 16 bit
  radio.setPALevel(RF24_PA_MAX); // уровень питания усилителя RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  radio.setChannel(0x6f); // установка канала
  radio.setAutoAck(false); // автоответ false
  radio.openReadingPipe(1, pipe); // открыть трубу на приём
  radio.startListening(); // приём

  // 31.4 кГц нормально, но можно и меньше
  TCCR1B &= ~_BV(WGM12);
  TCCR1B = TCCR1B & 0b11111000 | 0x01;
  TCCR2A |= _BV(WGM20);
  TCCR2B = TCCR2B & 0b11111000 | 0x01;

  strip.setGammaBright(true);
  strip.setDirection(0); // NORMAL / REVERSE - направление ШИМ

  strip.setBrightness(thisBrightness); // включение
  strip.setRGB(0, 0, 255);
  delay(2000);
  strip.setRGB(0, 0, 0);
  delay(1000);
}


int testBrig = 0;
bool revrs_flag = false;
uint32_t prevTime;
int val, rndVal;
float SMOOTH_K = 0.25; // #define SMOOTH_K 0.25 // коэффициент плавности огня

void fireTick() {
  if (millis() - prevTime > vals_colors[18]) { // скорость 0-1000;
    prevTime = millis();
    rndVal = random(2, 13) * 10;
  }
  val = val * (1 - SMOOTH_K) + rndVal * SMOOTH_K; // значения от 20-120
  strip.colorWheel(val);
  // Serial.print("val: "); Serial.println(val);
  thisBrightness = map(val, 20, 120, vals_colors[16], vals_colors[17]); // мин 0-255, макс 0-255
  strip.setBrightness(thisBrightness);
}

void fireM_tick() {
  if (millis() - prevTime > vals_colors[22]) { // скорость 0-1000;
    rndVal = random(0, 13) * 10;
    prevTime = millis();
  }
  val = val * (1 - SMOOTH_K) + rndVal * SMOOTH_K;
  thisBrightness = map(val, 0, 120, vals_colors[20], vals_colors[21]); // мин - 0-255, макс 0-255
  strip.colorWheel(vals_colors[19] * 6); // цвет 0-255
  strip.setBrightness(thisBrightness);
}

void effectsTick() {
  if (activeNumb <= 4) {
    switch (activeNumb) {
      case 0: // RGB
        strip.setRGB(vals_colors[0], vals_colors[1], vals_colors[2]);
        thisBrightness = vals_colors[3];
        break;
      case 1: // HSV
        strip.setHSV(vals_colors[4], vals_colors[5], vals_colors[6]);
        thisBrightness = 255;
        break;
      case 2: // colorWheel
        strip.colorWheel(vals_colors[7] * 6); // 0-255
        thisBrightness = vals_colors[8];
        break;
      case 3: // HEX
        strip.setHEX(colorValsHEX[vals_colors[9]]);
        thisBrightness = vals_colors[10];
        break;
      case 4: // setKelvin
        strip.setKelvin(vals_colors[11] * 39);
        // Serial.println(vals_colors[11] * 39);
        thisBrightness = vals_colors[12];
        break;
    }
    strip.setBrightness(thisBrightness);
  } else {
    if (millis() - effTimer >= effPeriod) {
      effTimer = millis();
      switch (activeNumb) {
        case 5: // ColorWheel
          effPeriod = vals_colors[13]; // 0 - 1530 скорость изменения
          thisColor += vals_colors[14]; // к цвету прибавляем шаг
          if (thisColor > 1530) thisColor = 0;
          strip.colorWheel(thisColor);
          strip.setBrightness(vals_colors[15]);
          break;
        case 6: // Fire
          effPeriod = 40;
          fireTick();
          break;
        case 7: // FireManual
          effPeriod = 40;
          fireM_tick();
          break;
        case 8: // Strobe
          effPeriod = vals_colors[26]; // скорость вспышек
          strip.setHSV(vals_colors[23], vals_colors[24], vals_colors[25] * strobeFlag);
          strobeFlag = !strobeFlag;
          // strip.setBrightness(255 * strobeFlag); // а нужа ли яркость при HSV ?
          break;
        case 9: // FadeTo
          effPeriod = 40;
          strip.fadeTo(colorValsHEX[vals_colors[27]], vals_colors[29]);
          strip.fadeTo(colorValsHEX[vals_colors[28]], vals_colors[29]);
          break;
        case 10: // rainbowBreath
          effPeriod = vals_colors[30];
          if (revrs_flag == false) {
            testBrig--;
            if (testBrig <= 0) {
              testBrig = 0;
              revrs_flag = true;
              thisColor += vals_colors[31];
              if (thisColor > 1530) thisColor = 0;
              strip.colorWheel(thisColor);
            }
          } else {
            testBrig++;
            if (testBrig >= 255) {
              testBrig = 255;
              revrs_flag = false;
            }
          }
          strip.setBrightness(testBrig);
          break;
      }
    }
  }
}

int recieved_data[2];  // массив принятых данных

void loop() {
  if (radio.available()) {
    radio.read(&recieved_data, 4); // чиатем входящий сигнал
    vals_colors[recieved_data[0]] = recieved_data[1]; // номер в массиве = значение
    // Serial.print(recieved_data[0]); Serial.print(": "); Serial.print(recieved_data[1]); Serial.println();
    activeNumb = vals_colors[ACTIVENUMB]; // передаём активный номер
  }
  effectsTick(); // чекаем эффекты и цвета
}
