
/*
 * ATmega328 (Arduino UNO, Nano, Pro Mini): 1 кБ (0-1023); 1024 ячейки
 * byte 1 байт  
 * int 2 байта 
 * float 4 байта
 * !Все ячейки имеют значение по умолчанию (у нового чипа) 255!
 * Запись одного байта занимает ~3.3 мс (миллисекунды)
   Чтение одного байта занимает ~0.4 мкс (микросекунды)
 */

 /*
  * Почему данных в 1 байте имеено 0-255 ? 
  * Как получается так, что 2 ячейки могут собраться в большее число тип int или floot?
  * Как это вообще работает на уровне физики?
  * Как он складывает эти байты в большие числа?
  * 
  */


#include <avr/eeprom.h> // идёт в комплекте с <EEPROM.h>
#include <EEPROM.h> // есть у всех ...


#define RESERV_ADDR 1010  // номер резервной ячейки
#define RESERV_KEY 50 // ключ первого запуска. 0-254, на выбор

int meArr[] { // мой массив данных типа инт
  100, 233, 1003, 
  23, 99,
};

void setup() {
  Serial.begin(9600);


  // Если в первый раз в программе ...
  if (EEPROM.read(RESERV_ADDR) != RESERV_KEY) { // если значение в резервной ячейки не равно 50
    Serial.println(F("Первый раз в программе"));
    EEPROM.write(RESERV_ADDR, RESERV_KEY); // записали ключ
    EEPROM.put(0, meArr); // отправляю в еерум значенияя заводских настроек
  } else {
    // Будет выводиться это даже есть поменять, т.к. компорт не сразу открывается прикол
    Serial.println(F("Уже был тут"));
  }

  // Читаем все данные с EEPROM i < 1023
  Serial.println(F("---------EEPROM---------"));
  for (int i = 0; i < 20; i++) {
    Serial.print(EEPROM.read(i)); Serial.print(" ");
    if (i != 0 and i % 9 == 0) Serial.println(); // каждые 10 значений переносим строку (% это тип кратно...)
  }
  Serial.println();
  Serial.println(F("---------EEPROM---------"));
  for (int i = 1000; i < 1020; i++) {
    Serial.print(EEPROM.read(i)); Serial.print(" ");
    if (i != 0 and i % 9 == 0) Serial.println(); // каждые 10 значений переносим строку (% это тип кратно...)
  }
  Serial.println();

  
  EEPROM.get(0, meArr); // c какого байта, куда положить ?

  Serial.println(F("Читаю из EEPROM мой массив:"));
  
  for (int i = 0; i < sizeof(meArr) / 2; i++) {
    Serial.print(meArr[i]); Serial.print(","); Serial.print(" ");
    //if (i != 0 and i % 9 == 0) Serial.println(); // каждые 10 значений переносим строку (% это тип кратно...)
  }
  Serial.println();

  meArr[1] = 196; // деффолд 233
  meArr[2] = 44; // деффолд 1003

  EEPROM.put(2, meArr[1]);
  EEPROM.put(4, meArr[2]);

  //EEPROM.get(2, meArr[1]);
  //EEPROM.get(4, meArr[2]);
  
  //EEPROM.get(0, LEDbright); // прочитали яркость из адреса 0
}


void loop() {
  
}


/* 'put/get'
void setup() {
  Serial.begin(9600);
  
  // объявляем переменные, которые будем писать
  float dataF = 3.14;
  int16_t dataI = -634;
  byte dataArray[] = {10, 20, 30, 40};
  
  EEPROM.put(0, dataF); // put это отправить
  EEPROM.put(4, dataI);
  EEPROM.put(6, dataArray);
  
  // объявляем переменные, куда будем читать
  float dataF_read = 0;
  int16_t dataI_read = 0;
  byte dataArray_read[4];
  
  // читаем точно так же, как писали
  EEPROM.get(0, dataF_read); // get это получить
  EEPROM.get(4, dataI_read);
  EEPROM.get(6, dataArray_read);
  
  // проверяем
  Serial.println(dataF_read);
  Serial.println(dataI_read);
  Serial.println(dataArray_read[0]);
  Serial.println(dataArray_read[1]);
  Serial.println(dataArray_read[2]);
  Serial.println(dataArray_read[3]);
  
}

void loop() {}
*/



/* 'Запись байта'
void setup() {
  Serial.begin(9600);
  
  // пишем 200 по адресу 10
  EEPROM.update(10, 200);  
  Serial.println(EEPROM.read(10));  // выведет 200
  Serial.println(EEPROM[10]);       // выведет 200
}

void loop() {}
*/



/* Пример 2 'Массив'
void setup() {
  Serial.begin(9600);
  
  // создаём массив
  float dataF[] = {3.14, 60.25, 9132.5, -654.3};
  
  // записываем по адресу 20, указав размер
  eeprom_write_block((void*)&dataF, 20, sizeof(dataF));
  
  // создаём новую пустой массив такого же типа и размера!
  float dataF_read[4];
  
  // читаем из адреса 20
  eeprom_read_block((void*)&dataF_read, 20, sizeof(dataF_read));
  
  // выведет 3.14 60.25 9132.5 -654.3
  for (byte i = 0; i < 4; i++) {
    Serial.println(dataF_read[i]);
  }
  
}

void loop() {}
*/


/* Пример 2 'Структура'
void setup() {
  Serial.begin(9600);
  
  // Объявляем структуру
  struct MyStruct {
    byte a;
    int b;
    float c;
  };
  
  // Создаём и заполняем структуру
  MyStruct datStru;
  datStru.a = 10;
  datStru.b = 1000;
  datStru.c = 3.14;
  
  // Записываем по адресу 10, указав размер структуры и приведя к void*
  eeprom_write_block((void*)&datStru, 10, sizeof(datStru));
  
  // Cоздаём новую пустую структуру
  MyStruct newDatStru;
  
  // Читаем из адреса 10
  eeprom_read_block((void*)&newDatStru, 10, sizeof(newDatStru));
  
  // Выведет 10 1000 3.14
  Serial.println(newDatStru.a);
  Serial.println(newDatStru.b);
  Serial.println(newDatStru.c);
}
void loop() {}
*/

/* Пример 1
void setup() {
  Serial.begin(9600);
  
  // Объявляем данные разных типов
  byte dataB = 120;
  float dataF = 3.14;
  int16_t dataI = -634;
  
  // Пишем друг за другом
  eeprom_write_byte(0, dataB);  // 1 байт
  eeprom_write_float(1, dataF);  // 4 байта
  eeprom_update_word(5, dataI); // для разнообразия "обновим"
  
  // Объявляем переменные, куда будем читать
  byte dataB_read = 0;
  float dataF_read = 0;
  int16_t dataI_read = 0;
  
  // Читаем
  dataB_read = eeprom_read_byte(0);
  dataF_read = eeprom_read_float(1);
  dataI_read = eeprom_read_word(5);
  
  // Выведет 120 3.14 -634
  Serial.println(dataB_read);
  Serial.println(dataF_read);
  Serial.println(dataI_read);
}

void loop() {}
*/
