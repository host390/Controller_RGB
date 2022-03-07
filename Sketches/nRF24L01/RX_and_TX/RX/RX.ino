


#define PIN_R 10
#define PIN_G 9
#define PIN_B 3
#include <GyverPWM.h>
#include <GRGB.h>
GRGB led(COMMON_CATHODE, PIN_R, PIN_G, PIN_B);


//#include <GyverRGB.h>
//GRGB strip(PIN_R, PIN_G, PIN_B);  // куда подключены цвета (R, G, B)


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(7, 8); // CE -> 7 CSN -> 8 RF24 radio(7, 8);

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

int recieved_data[3];  // массив принятых данных

void setup() {
  Serial.begin(9600);         // открываем порт для связи с ПК
  radio.begin();              // активировать модуль
  //radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  //radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  //radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  //radio.setPayloadSize(32);   // размер пакета, в байтах

  radio.openReadingPipe(1, address[0]);   // хотим слушать трубу 0
  radio.setChannel(0x60);     // выбираем канал (в котором нет шумов!)
  radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  
  radio.powerUp();        // начать работу
  radio.startListening(); // начинаем слушать эфир, мы приёмный модуль

  PWM_frequency(3, 18000, FAST_PWM); //PWM_20KHZ_D3(0); // ШИМ 16 кГц на пине D3, заполнение 30 из 255
  PWM_frequency(9, 18000, FAST_PWM); // запустить ШИМ на D10 с частотой 150'000 Гц, режим FAST_PWM
  PWM_frequency(10, 18000, FAST_PWM);
  
//  TCCR2A |= _BV(WGM20); // таймер 2 пины 3 и 11 31.4 кГц
//  TCCR2B = TCCR2B & 0b11111000 | 0x01;
//  TCCR1B &= ~_BV(WGM12);
//  TCCR1B = TCCR1B & 0b11111000 | 0x01;
//  TCCR2A |= _BV(WGM20);
//  TCCR2B = TCCR2B & 0b11111000 | 0x01;

  // Пины D3 и D11 - 31.4 кГц
//  TCCR2B = 0b00000001;  // x1
//  TCCR2A = 0b00000001;  // phase correct

  // Пины D3 и D11 - 8 кГц
//  TCCR2B = 0b00000010;  // x8
//  TCCR2A = 0b00000011;  // fast pwm

  // Пины D3 и D11 - 30 Гц
  //TCCR2B = 0b00000111;  // x1024
  //TCCR2A = 0b00000001;  // phase correct

  //strip.highFrequency(20000);
  
}

void loop() {
  byte pipeNo; // что-то для трубы ???
  
  if (radio.available()) { // слушаем эфир со всех труб зачем этот &pipeNo??? или лучше с while???
    radio.read(&recieved_data, sizeof(recieved_data));  // чиатем входящий сигнал
  }
  Serial.print(recieved_data[0]); Serial.print(" "); Serial.print(recieved_data[1]); Serial.print(" "); Serial.println(recieved_data[2]);
  
//  Serial.print("R: "); Serial.print("G: "); Serial.print("B: ");
//  Serial.print(recieved_data[0]); Serial.print(" ");
//  Serial.print(recieved_data[1]); Serial.print(" ");
//  Serial.println(recieved_data[2]);
//  Serial.println(recieved_data[0]);
//  led.setWheel8(recieved_data[0], recieved_data[1]);
  //Serial.println(recieved_data);
  //led.setWheel8(recieved_data, 100);
  PWM_set(PIN_R, (recieved_data[0] == 0) ? 1: recieved_data[0]);
  PWM_set(PIN_G, (recieved_data[1] == 0) ? 1: recieved_data[1]);
  PWM_set(PIN_B, (recieved_data[2] == 0) ? 1: recieved_data[2]);
  //led.setRGB(recieved_data[0], recieved_data[1], recieved_data[2]);
}
