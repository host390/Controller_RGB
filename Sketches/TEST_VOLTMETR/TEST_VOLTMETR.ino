

#define TEST 3

#include <EEPROM.h>

#define DT 2
#define CLK 3
#define SW 4
#include <GyverEncoder.h>
Encoder encod(CLK, DT, SW, TYPE1);  // для работы c кнопкой и сразу выбираем тип

/* Дефайны для библиотек обьявляем ПЕРЕД их одключением */
#define OLED_SOFT_BUFFER_64
#include <GyverOLED.h>

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

#if (TEST == 1) // Тестим первый вариант ------------------------------>

#include <directADC.h>

#define VCC_INC 5           // Шаг регулировки напряжения питания 
#define VCC_INT 300         // Период опроса напряжения питания в мс
#define BAT_EMPTY 3300      // Напряжение полностью севшей батарейки в мв
#define BAT_FULL 4200       // напряжение свежей батарейки в мв

#define SYSTEM_DATA_ADDR 0  // По этому адресу храним системные настройки в EEPROM

uint8_t charge = 0; // переменная для заряда 0-10

/* структура для системных данных */
struct {
  uint16_t internal_ref;
} systemData;

uint16_t vcc = 4700;  // переменная для хранения реального напряжения питания

void setup() {
  //Serial.begin(9600);
  adcInit();// Настройка АЦП
  EEPROM.get(SYSTEM_DATA_ADDR, systemData);   // Читаем системные настройки из EEPROM
  
  oled.init(); // инициализировать дисплей
  oled.setContrast(255);   // отправить ему актуальную яркость
  oled.clear();
  oled.update();
  
}

void adcInit() {                  // инициализация ацп
  _ADC.setReference(DEFAULT);     // опорное для ацп - vcc
  _ADC.setInput(ADC_BANDGAP);     // вход для ацп - внутреннее опорное
  _ADC.setPrescaler(ADC_PRESCALER_128); // самый медленный режим ацп
  _ADC.enable(true);              // включаем ацп
  delayMicroseconds(100);         // даем паузу
}

void loop() {

  encod.tick();
  if (encod.isClick()) {
    oled.setScale(1);
    oled.clear();
    oled.home();
    oled.update();
    ServiseMode();
  }

  oled.clear();
  oled.home();
  oled.print(charge);
  oled.setCursor(0, 2);
  oled.setScale(2);
  //oled.print(F("Vcc: ")); oled.println(vcc);
  oled.print(F("Ref: ")); oled.println(systemData.internal_ref);
  oled.print(F("gVcc: ")); oled.println(getVcc());
  oled.print(F("rVcc ")); oled.println(readVccB());
  
  batteryTick();
  batteryPrint();
  
  oled.update();
  delay(1000);

}

void ServiseMode() {
  while(1) {
    encod.tick();
    
    if (encod.isClick()) {
      systemData.internal_ref = getVref(vcc); // рассчитать значение опорного из напряжения питания
      EEPROM.put(SYSTEM_DATA_ADDR, systemData);   // сохраняет настройки
      break;
    }  
      
    if (encod.isTurn()) {
      if (encod.isRight()) {  // нажатием или удержанием устанавливаем vcc
        vcc = constrain(vcc + VCC_INC, 2000, 5500);
      } if (encod.isLeft()) {
        vcc = constrain(vcc - VCC_INC, 2000, 5500);
      }                       
      systemData.internal_ref = getVref(vcc); // рассчитать значение опорного из напряжения питания
      
      //oled.clear(); // вывод значений
      oled.home();
      oled.print(F("Vcc: ")); oled.println(vcc);
      oled.print(F("Ref: ")); oled.println(systemData.internal_ref);
      (F(
         "\n"
         "Press OK to save\n"
       ));
      oled.update();
    }
  }
}

// А чем опорное отличается от внешнего получется?
uint16_t getVref(uint16_t vcc) {    // чтение внуреннего опорного
  uint16_t buf = 0;                 // буфер для усредняющего фильтра
  for (uint8_t i = 0; i < 8; i++) { // цикл усредняющего фильтра
    _ADC.convert();                 // запустить преобразование ацп
    while (_ADC.isBusy());          // дождаться окончания
    buf += _ADC.read();             // прочитать ацп
  }
  buf >>= 3;                        // поделить на кол во итераций
  return ((uint32_t) vcc * buf) >> 10;  // рассчитать опорное
}

uint16_t getVcc() { // чтение напряжения питания
  uint16_t buf = 0;
  for (uint8_t i = 0; i < 8; i++) { // читаем буфера
    _ADC.convert();
    while (_ADC.isBusy());
    buf += _ADC.read();
  }
  buf >>= 3;                      // аналогично сдвигаем буфер
  return (uint32_t)((systemData.internal_ref * 1024UL) / buf); // считаем напряжение питания
}

void batteryTick() {          // опрос батарейки
  charge = constrain(map(getVcc(), BAT_EMPTY, BAT_FULL, 0, 10), 0, 10); // преобразуем мин-макс напряжение батарейки в диапазон 
  //0-10 и ограничиваем в этом диапазоне
}

void batteryPrint() {         // вывод индикатора заряда
  oled.rect(117, 0, 127, 6, OLED_STROKE);  // тело индикатора
  oled.rect(115, 1, 116, 5, OLED_STROKE);  // полосочка для красоты
  for (uint8_t i = 0; i < charge; i++) { // заполнение индикатора
    oled.line(127 - i, 0, 127 - i, 6);   // полосками
  }
}



float my_vcc_const = 1.1;    // начальное значение константы должно быть 1.1
long readVccB() { //функция чтения внутреннего опорного напряжения, универсальная (для всех ардуин)
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both
  long result = (high << 8) | low;

  result = my_vcc_const * 1023 * 1000 / result; // расчёт реального VCC
  return result; // возвращает VCC
}
















#endif 
