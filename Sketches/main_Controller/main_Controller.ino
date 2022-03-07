
/*
 * Тут будет шапка кто создатель там какой год и тд и тп рекламма...
 * 
 * 
 * 
 * 
 */

/*
 * Нужно добавить:
 * 1. При включении запускать последнюю функцию.
 * 2. Понять и исправить, маргание на секунду при быстрой работе эффектов
 * 3. ...
 * 
 * 
 */


#define PIN_R 10
#define PIN_G 9
#define PIN_B 3

//#include <GyverPWM.h>
//#include <GRGB.h>
//GRGB led(COMMON_CATHODE, PIN_R, PIN_G, PIN_B);

#include <GyverRGB.h>
GRGB strip(PIN_R, PIN_G, PIN_B);  // куда подключены цвета (R, G, B)


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(7, 8); // CE -> 7 CSN -> 8 RF24 radio(7, 8);


//byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
const uint32_t pipe = 111156789; // адрес рабочей трубы;

int activeNumb = 0; // активный режим

int recieved_data[2];  // массив принятых данных byte recieved_data[2];  // массив принятых данных

//#define ACTIVENUMB 14
//int vals_colors[15] = { // массив "Цветов"
//  10, 10, 10, 10, // R G B Brightness
//  10, 10, 10, // Color Saturation Brightness
//  10, 10, 10, // Color_F Saturation_F Brightness_F
//  10000, 120, // Color_F Saturation_F Brightness_F
//  2, 120, // color_HEX
//  0, // activeNumb
//}; 

//#define ACTIVENUMB 17
//int vals_colors[18] = { // массив "Цветов"
//  10, 10, 10, 10, // R G B Brightness
//  10, 10, 10, // Color Saturation Brightness
//  10, 10, 10, // Color_F Saturation_F Brightness_F
//  1000, 120, // Color_F Saturation_F Brightness_F
//  2, 120, // color_HEX
//
//  1000, 500, 100, // CW_EF
//  
//  0, // activeNumb
//}; 

#define ACTIVENUMB 34 // 34
int vals_colors[35] = { // 35 36 массив "Цветов" 
  100, 100, 100, 100, // R G B Brightness 0-3
  100, 100, 100, // Color Saturation Brightness 4-6
  100, 100, 100, // Color_F Saturation_F Brightness_F 7-9
  40, 10, // colorWheel 10-11
  1530, 255, // color_HEX 12-13
  
  1000, 500, 100, // CW_EF 14-16
  255, 255, 1000,  // Fire 17-19
  255, 255, 255, 1000,  // FireManual 20-23
  255, 255, 255, 1000,  // Strobe 24-27
  255, 255, 255, 1000,  // RndStrobe 28-31
  255, 1000,  // Police 32-33

  0, // activeNumb 34
}; 



const uint32_t colorValsHEX[] = { // эта штука будет на стороне контроллера
  0xFFFFFF, 0xC0C0C0, 0x808080, 0x000000, 0xFF0000, 0x800000,
  0xFFFF00, 0x808000, 0x00FF00, 0x008000, 0x00FFFF, 0x008080,
  0x0000FF, 0x000080, 0xFF00FF, 0x800080,
};

//int val[10] { // массив исходных данных
//  10, 33, 45,
//};

//int ArrTrans[10]; // массив для принятых данных




void setup() {
  Serial.begin(9600); // открываем порт для связи с ПК

//  for (byte i = 0; i < 10; i++) {
//    valrans[i] = val[i];
//    Serial.print(valrans[i]); Serial.print(" ");
//  }
//  
//  //valrans = val;
//  //Serial.print()
//
//  return;
 
  //radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  //radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  //radio.setPayloadSize(32);   // размер пакета, в байтах
  //radio.begin();              // активировать модуль
  
  //radio.openReadingPipe(1, address[0]);   // хотим слушать трубу 0
  //radio.setChannel(0x60);     // выбираем канал (в котором нет шумов!)
  //radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  //radio.setDataRate (RF24_1MBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  //radio.powerUp();        // начать работу
  //radio.startListening(); // начинаем слушать эфир, мы приёмный модуль

  
  radio.begin();  // инициализация
  delay(1000);
  
  radio.setDataRate(RF24_1MBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.setCRCLength(RF24_CRC_8); // размер контрольной суммы 8 bit или 16 bit
  radio.setChannel(0x6f);         // установка канала
  
  radio.setAutoAck(false); // автоответ false
  
  radio.openReadingPipe(1, pipe); // открыть трубу на приём
  radio.startListening();        // приём

  /*
  PWM_frequency(3, 18000, FAST_PWM); //PWM_20KHZ_D3(0); // ШИМ 16 кГц на пине D3, заполнение 30 из 255
  PWM_frequency(9, 18000, FAST_PWM); // запустить ШИМ на D10 с частотой 150'000 Гц, режим FAST_PWM
  PWM_frequency(10, 18000, FAST_PWM);

  //strip.highFrequency(20000);
  PWM_set(PIN_R, 100);
  PWM_set(PIN_G, 20);
  PWM_set(PIN_B, 53);
  delay(2000);
  */
  
  // 31.4 кГц нормально, но можно и меньше
  TCCR1B &= ~_BV(WGM12);
  TCCR1B = TCCR1B & 0b11111000 | 0x01;
  TCCR2A |= _BV(WGM20);
  TCCR2B = TCCR2B & 0b11111000 | 0x01;
  
  strip.setBrightness(100);
  strip.setRGB(0, 110, 50); // установка цвета в пространстве RGB (каждый цвет 0-255)
  delay(2000);
  strip.setBrightness(0);
  strip.setRGB(0, 0, 0);
  
  for (byte i = 0; i < sizeof(vals_colors) / 2; i++) {
    Serial.print(vals_colors[i]); Serial.print(" "); 
  }
  Serial.println();

  
}


byte menuFunction = 0; // каунтер для выбора пункта цыета

bool transAllArr = true;

long timerMonit;





void loop() {
  //byte pipeNo; // что-то для трубы ???
  
  if (radio.available()) {

    radio.read(&recieved_data, 4); // чиатем входящий сигнал
    vals_colors[recieved_data[0]] = recieved_data[1]; // куда -- что
    ///*
    Serial.print(recieved_data[0]); Serial.print(" "); 
    Serial.print(recieved_data[1]); Serial.println();
    //*/
    activeNumb = vals_colors[ACTIVENUMB]; // передаём активный номер

    /*
    if (transAllArr) { // transAllArr если ещё не принимаем 1 значение, то надо принять стопку
      radio.read(&vals_colors, sizeof(vals_colors));
      for (byte i = 0; i < sizeof(vals_colors) / 2; i++) {
        Serial.print(vals_colors[i]); Serial.print(" "); 
      }
      Serial.println();
      activeNumb = vals_colors[7]; // передаём активный номер
      transAllArr = false;
      
    } else {
       radio.read(&recieved_data, 3);  // чиатем входящий сигнал
       vals_colors[recieved_data[0]] = recieved_data[1];
       Serial.print(recieved_data[0]); Serial.print(" "); 
       Serial.print(recieved_data[1]); Serial.print(" "); 
       Serial.print(recieved_data[2]);
       Serial.println();
    }*/

    
  }

  if (millis() - timerMonit >= 10000) {
    timerMonit = millis();
    for (byte i = 0; i < sizeof(vals_colors) / 2; i++) {
      Serial.print(vals_colors[i]); Serial.print(" "); 
    }
    Serial.println();
    Serial.print(F("activeNumb: ")); Serial.println(activeNumb);
  }
  
  effectsTick(); // чекаем эффекты и цвета

//  if (recieved_data[2] == true) {
//    recieved_data[2] = false;
//    transAllArr = true;
//    Serial.println("Вышли из параметров"); 
//  }
  //Serial.print(recieved_data[0]); Serial.print(" "); Serial.print(recieved_data[1]); Serial.print(" "); Serial.println(recieved_data[2]);

  
  // приходят 2 значение 1 номер параметра 2 значение этого параметра
  //recieved_data[0]; recieved_data[1];

  /*
  
  switch (activeNumb) {
    case 0: // RGB

      ///*
      strip.setRGB(vals_colors[0], vals_colors[1], vals_colors[2]); // vals_colors[3]
      strip.setBrightness(vals_colors[3]);
      //
      //led.setRGB(vals_colors[0], vals_colors[1], vals_colors[2], vals_colors[3]);
      
      break;
    case 1: // HSV
      strip.setHSV(vals_colors[4], vals_colors[5], vals_colors[6]);
      break;
      
    case 2: // FAST_HSV
      strip.setHSV_fast(vals_colors[7], vals_colors[8], vals_colors[9]);
      break;

    case 3: // colorWheel

      if (millis() - effTimer >= effPeriod) {
        effTimer = millis();
        effPeriod = vals_colors[10]; // 1530
        thisColor += vals_colors[11]; // 255
        if (thisColor > 1530) thisColor = 0;
        strip.colorWheel(thisColor);
        strip.setBrightness(180);
        //setW(presetSettings[3]);
      }
      /*
      strip.colorWheel(vals_colors[10]);
      strip.setBrightness(vals_colors[11]);
      
      break;

    case 4: // colorWheel 
      strip.setHEX(colorValsHEX[vals_colors[12]]);
      strip.setBrightness(vals_colors[13]);
      break;
  }
  */
  
  //PWM_set(PIN_R, (recieved_data[0] == 0) ? 1: recieved_data[0]);
  //PWM_set(PIN_G, (recieved_data[1] == 0) ? 1: recieved_data[1]);
  //PWM_set(PIN_B, (recieved_data[2] == 0) ? 1: recieved_data[2]);
  //led.setRGB(recieved_data[0], recieved_data[1], recieved_data[2]);


  
}
