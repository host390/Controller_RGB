
// Название выводится, спектр виден, всё ок идём в следующую проверку


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


RF24 radio(7, 8); // CE -> 7 CSN -> 8 RF24 radio(7, 8); radio(9, 10)

const uint8_t num_channels = 128; // всего 128 каналов
uint8_t values[num_channels];

void setup(void) {
  Serial.begin(9600);
  printf_begin();
  
  Serial.println("Scanner Air On");

  radio.begin();
  radio.setAutoAck(false);
  radio.startListening();
  radio.printDetails();  
  delay(5000);              

  int i = 0;
  while (i < num_channels) {
    printf("%x", i >> 4);
    ++i;
  }
  Serial.print("\n\r");
  i = 0;
  
  while ( i < num_channels ) {
    printf("%x", i & 0xf);
    ++i;
  }
  printf("\n\r");
}

const int num_reps = 100;

void loop(void) {
  
  // Clear measurement values
  memset(values, 0, sizeof(values));

  // Scan all channels num_reps times
  int rep_counter = num_reps;
  while (rep_counter--) {
    int i = num_channels;
    while (i--) {
      // Select this channel
      radio.setChannel(i);

      // Listen for a little
      radio.startListening();
      delayMicroseconds(512);
      radio.stopListening();

      // Did we get a carrier?
      if ( radio.testCarrier() )
        ++values[i];
    }
  }

  // Print out channel measurements, clamped to a single hex digit
  int i = 0;
  while ( i < num_channels ) {
    printf("%x", min(0xf, values[i] & 0xf));
    ++i;
  }
  printf("\n\r");
}


#ifndef __PRINTF_H__
#define __PRINTF_H__
#ifdef ARDUINO

int serial_putc(char c, FILE *) {
  Serial.write(c);
  return c;
} 

void printf_begin(void) {
  fdevopen(&serial_putc, 0);
}

#else
#error This example is only for use on Arduino.
#endif // ARDUINO
#endif // __PRINTF_H__









/*
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//#include "printf.h" // хз что за биба но это как сериал принт походу
//#include <LibPrintf.h>

RF24 radio(7, 8);  // инициализировать модуль на пинах 9 и 10 Для Уно
//RF24 radio(9, 10); // момент!!!

const uint8_t num_channels = 128; // ???
uint8_t values[num_channels];

void setup() {
  Serial.begin(9600);
  printf_begin();

  radio.begin();
  radio.setAutoAck(false);
  radio.startListening();
  radio.printDetails(); // Вот эта строка напечатает нам что-то, если все правильно соединили.
  delay(5000);          // И посмотрим на это пять секунд.
  radio.stopListening(); // ?????
  
  int i = 0;    // А это напечатает нам заголовки всех 127 каналов
  while ( i < num_channels )  {
    printf("%x", i >> 4);
    ++i;
  }
  printf("\n\r");
  i = 0;
  while ( i < num_channels ) {
    printf("%x", i & 0xf);
    ++i;
  }
  printf("\n\r");
}

const int num_reps = 100;

void loop(void) {

  radio.printDetails(); // Вот эта строка напечатает нам что-то, если все правильно соединили.
  delay(2500); // И посмотрим на это пять секунд.
  
  memset(values, 0, sizeof(values));
  int rep_counter = num_reps;
  while (rep_counter--) {
    int i = num_channels;
    while (i--) {
      radio.setChannel(i);
      radio.startListening();
      delayMicroseconds(128);
      radio.stopListening();
      if ( radio.testCarrier() )
        ++values[i];
    }
  }
  int i = 0;
  while ( i < num_channels ) {
    printf("%x", min(0xf, values[i] & 0xf));
    ++i;
  }
  printf("\n\r");

  
}

int serial_putc( char c, FILE * ) {
  Serial.write( c );
  return c;
}

void printf_begin(void) {
  fdevopen( &serial_putc, 0 );
}


*/
