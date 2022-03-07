
#if (TEST == 3)

#define BAT_FULL 4200       // напряжение полностью зараженной 
#define BAT_EMPTY 3300      // напряжение свежей батарейки в мв

int voltage;

void setup() {
  Serial.begin(9600);
  Serial.println("TEST == 3");
  
  oled.init(); // инициализировать дисплей
  oled.setContrast(255);   // отправить ему актуальную яркость
}

uint8_t charge = 0; // переменная для заряда 0-10

void loop() {
  //Serial.println(readVccA());
  //Serial.println(readVccB());
  
  oled.clear();
  oled.home();
  
  voltage = readVccB();
  
  //Serial.println(voltage);
  //delay(1000);
  //return;
  
  charge = constrain(map(voltage, BAT_EMPTY, BAT_FULL, 0, 10), 0, 10); // преобразуем мин-макс напряжение батарейки в диапазон
  oled.println(charge);
  
  oled.rect(117, 0, 127, 6, OLED_STROKE);  // тело индикатора
  oled.rect(115, 1, 116, 5, OLED_STROKE);  // полосочка для красоты
  for (uint8_t i = 0; i < charge; i++) { // заполнение индикатора
    oled.line(127 - i, 0, 127 - i, 6);   // полосками
  }
  oled.setCursor(0, 2);
  oled.print(F("readVccB: ")); oled.println(voltage);

  oled.update();
  
//  if (charge >= 10) charge = 0;
//  else charge++;
  
  delay(1000);
  
  
//  oled.print(F("readVccA: "));
//  oled.println(readVccA());
//  oled.print(F("readVccB: "));
//  oled.println(readVccB());
//  oled.update();
//  delay(3000);
  
}



int Summ() {
  return 4 + 6;
}


///*
 // Функция чтения внутреннего опорного напряжения
float readVccA() {  
  byte i;
  float result = 0.0;
  float tmp = 0.0;
  const float vcc_const = 1.091; //  1.0 -- 1.2 константа(const) калибровки напряжения
  
  for (i = 0; i < 5; i++) {
    // Read 1.1V reference against AVcc
    // set the reference to Vcc and the measurement to the internal 1.1V reference - analogReference(internal)то опорное напряжение 1.1 вольт
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
#else
    // works on an Arduino 168 or 328
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

    delay(3); // Wait for Vref to settle - Подождите, пока Vref установится
    ADCSRA |= _BV(ADSC); // Start conversion - Начало преобразований
    while (bit_is_set(ADCSRA, ADSC)); // measuring - измерение

    uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH - сначала нужно прочесть ADCL - это запирает ADCH
    uint8_t high = ADCH; // unlocks both - разблокировать оба

    tmp = (high << 8) | low;
    tmp = (vcc_const * 1023.0) / tmp; // расчёт реального VCC в вольтах, где vcc_const это подобранная константа 1.0 - 1.2
    result = result + tmp;
    delay(5);
  }
  result = result / 5; // расчёт реального Vcc как среднеарифметическое пяти измерений в вольтах
  // Serial.println("result = " + String(result)); // вывод реального напряжения питания МК на выводе Vcc в вольтах
  return result;// возвращает Vcc,прекращает вычисления в функции и возвращает значение (result) из прерванной функции в вызывающую 
  //(readVcc()=result)
}

//*/



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
