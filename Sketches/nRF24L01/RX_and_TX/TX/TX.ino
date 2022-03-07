
#define DT 3
#define CLK 2
#define SW 4
#include <GyverEncoder.h>
Encoder encod(CLK, DT, SW, TYPE1);

#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

int transmit_data;  // массив, хранящий передаваемые данные
//byte latest_data[1];    // массив, хранящий последние переданные данные
//boolean flag; // флажок отправки данных

//byte valPot; // значение с потенциометра


void setup() {
  Serial.begin(9600);         // открываем порт для связи с ПК

  radio.begin();              // активировать модуль
  
  //radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  //radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  //radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  //radio.setPayloadSize(32);   // размер пакета, в байтах
  
  radio.openWritingPipe(address[0]);  // мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);             // выбираем канал (в котором нет шумов!)
  radio.setPALevel (RF24_PA_MAX);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  radio.powerUp();        // начать работу
  radio.stopListening();  // не слушаем радиоэфир, мы передатчик

  encod.setFastTimeout(60);
}

long int timer;
int increment = 0;

void loop() {

  //radio.write(&increment, sizeof(increment));
  
  encod.tick();
  if(encod.isTurn()) {
    increment = 0;
    if (encod.isRight()) increment = 1; // incr(&vals[stepParams + SumNumber], 1, 255);
    if (encod.isLeft()) increment = -1;
    if (encod.isFastR()) increment = 10;
    if (encod.isFastL()) increment = -10;
    transmit_data = constrain(transmit_data + increment, 0, 255);
  }
  
  Serial.println(transmit_data);
  radio.write(&transmit_data, sizeof(transmit_data));
  //Serial.println(transmit_data[0]);
  
  
//  if (millis() - timer >= 5) {
//    timer = millis();
//    transmit_data[0] = map(analogRead(0), 0, 1023, 0, 255);
//  }
//  transmit_data[0] = map(analogRead(0), 0, 1023, 0, 255);
//  radio.write(&transmit_data, sizeof(transmit_data)); // отправить по радио
//  Serial.print("valPot: ");
//  Serial.println(transmit_data[0]);

//  if (flag == 1) {
//    //radio.powerUp();    // включить передатчик
//    radio.write(&transmit_data, sizeof(transmit_data)); // отправить по радио
//    flag = 0;           //опустить флаг
//    //radio.powerDown();  // выключить передатчик
//    Serial.print("valPot: ");
//    Serial.println(transmit_data[0]);
//  }
  
  
  //radio.write(&valPot, sizeof(valPot)); // Как часто можно отправлять данные ???  &valPot ???
  

  
  
}
