

#if (TEST == 2)


/*
 * Это вариант измерения с резисторами какиемо-та да да
 * + нет библиотек 
 * - хз
 */

//-------НАСТРОЙКИ ВОЛЬТМЕТРА--------
// r3 и r4 резисторы делителя напряжения
const float r3 = 215.0;  // 220 Ом, (можно пренебречь и принять r3 = 0 Ом)
const float r4 =  2050000.0;  // 2.0~8.0 МОм
float VCC = 0.0; // напряжение питания микроконтроллера Vcc (непосредственно на пине Vcc)
float battery_voltage = 0.0; // напряжение на батареи
float max_battery_voltage = 4.20; // максимальное значения напряжения на батареи при полной зарядке (4,10-4,25 вольт)
float min_battery_voltage = 2.60; // минимальное допустимое рабочее напряжение на батареи (2,60 вольт)
float battery_charge; // остаток заряда батареи в процентах (0~100%)
//float MaxVoltage = 0.0; // максимальное измеряемое напряжения (зависит от номиналов резисторов R3 R4 в цепи делителя)
const float vcc_const = 1.091; //  1.0 -- 1.2 константа(const) калибровки напряжения


#define VoltmetrBatt A6  // вольтметр батареи: A1, значениев программе VoltmetrBatt=15




void setup() {
  Serial.begin(9600);
  Serial.println("TEST == 2");
  
  void(* resetFunc) (void) = 0;//объявляем функцию reset с адресом 0 (для программного ресет)
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  analogReference(DEFAULT); // опорным напряжением считать напряжение питания микроконтроллера Vcc~5 вольт,(режим по умолчанию, можно не задавать)
  //если analogReference(INTERNAL), то опорное равно стабилизированному внутреннему ~ 1,1 вольт
  //VCC = readVcc(); //обращение к функции readVcc()чтения внутреннего опорного напряжения, в нашем случаи напряжение питания Vcc микроконтроллера
  //MaxVoltage = VCC / (r4 / (r3 + r4));//расчёт максимального измеряемого напряжения взависимости от номиналов резисторов R3 R4 в цепи делителя
  //Serial.println("Microcontroller supply voltage at pin Vcc : Vcc = " + String(VCC,3) + " V"); // напряжения питания МК на выводе Vcc в вольтах
  //Serial.println("Maximum measurable voltage: MaxVoltage = " + String(MaxVoltage) + " V"); // максимальное измеряемое напряжение
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
}

void loop() {
  
  VCC = readVcc(); // обращение к функции readVcc()чтения внутреннего опорного напряжения. Переменная VCC = значению напряжение питания Vcc
  Serial.println("Vcc=" + String(VCC, 2) + " V"); // вывод напряжения питания МК на выводе Vcc

  // -------- Расчёт остатка заряда аккумуляторной батареи  ----------
  // считываем точное значение с пина А1 5 раз, где находиться наш вольтметр с делителем напряжения
  //ADC — Analog to Digital Converter : АЦП - аналого-цифровой преобразователь
  battery_voltage = 0.0;  // напряжения на аккумуляторной батареи
  for (byte i = 0; i < 5; i++) {
    Serial.print(analogRead(VoltmetrBatt)); Serial.print(" ");// для контроля вывести эти 5 замеров
    battery_voltage = battery_voltage + analogRead(VoltmetrBatt); // сумма 5 замеров
    delay(10);
  }
  Serial.println(battery_voltage);// для контроля вывести эти 5 замеров
  
  // Расчёт напряжения батареи в вольтах как среднеарифметическое 5 измерений с учётом опорного VCC и резисторов R3 и R4
  battery_voltage = ((battery_voltage / 5) * VCC / 1024) / (r4 / (r3 + r4)); // итоговая формула расчёта напряжения батареи вольт (схема 3)
  // Для схемы 2 нужно подкорректировать формулу, так как (-) батареи, это не минус GND ардуино, относительно которого замеряется напряжение батареи
  //battery_voltage = (1.6 * battery_voltage + 0.568) / 1.72 ; // Для схемы 2
  //Serial.println("Vbatt=" + String(battery_voltage, 3) + " V"); // вывод напряжения на батареи Vbatt
  Serial.println(battery_voltage);// для контроля вывести эти 5 замеров

  // -Интерполяция по графику разряда аккумулятора малым током 0,066-0,120 А, потребляемой нагрузкой 0,280 Вт, напряжение 4,20-2,60 В
  // 1-й, способ расчёта остатка заряда аккумулятора. Интерполяция по графику разряда литиевого аккумулятора
  // отображение остатка заряда в процентах от полной ёмкости! Интерполировано вручную по графику разряда ЛИТИЕВОГО аккумулятора
  battery_charge = battery_voltage * 100;
  if (battery_charge > 410)
    battery_charge = map(battery_charge, max_battery_voltage * 100, 410, 100, 95);
  else if ((battery_charge <= 410) && (battery_charge > 405) )
    battery_charge = map(battery_charge, 410, 405, 95, 76);
  else if ((battery_charge <= 405) && (battery_charge > 360) )
    battery_charge = map(battery_charge, 405, 360, 76, 29);
  else if ((battery_charge <= 360) && (battery_charge > 340) )
    battery_charge = map(battery_charge, 360, 340, 29, 16);
  else if ((battery_charge <= 340) && (battery_charge > 310) )
    battery_charge = map(battery_charge, 340, 310, 16, 5);
  else if (battery_charge <= 310)
    battery_charge = map(battery_charge, 310, min_battery_voltage * 100, 5, 0);
  //Serial.println("battery_charge = " + String(battery_charge, 3) + " %"); // процент уровеня заряда батареи (пока может быть > 100)
  battery_charge = constrain(battery_charge, 0, 100); //заряд батареи определён областью допустимых значений 0~100% constrain
  Serial.println("Battery charge level = " + String(battery_charge, 3) + " %"); // процент уровня заряда батареи
  // -Конец интерполяции по графику (Dunay)-----------------------------------------------------------------------------

  /*
    // ----------Линейная зависимость--------------------------------------------------------------------------------
    // 2-й, менее точный способ расчёта остатка заряда аккумулятора, как линейная зависимость от напряжения на батареи.
    battery_charge = (battery_voltage - min_battery_voltage) * 100 / (max_battery_voltage - min_battery_voltage); // расчёт остатка заряда батареи
    battery_charge = constrain(battery_charge, 0, 100); //заряд батареи определён областью допустимых значений (например 0~100%)
    //Serial.println("Battery charge level = " + String(battery_charge, 3) + " %"); // процент уровеня заряда батареи
    Serial.println("battery_charge = " + String(battery_charge, 3) + " %"); // процент уровеня заряда батареи
    //Serial.println("Battery charge level = " + String(floor(battery_charge),0) + " %");// floor округлил до целого в низ (round до целого, ceil до целого в верх)
    //Функция constrain проверяет допустимые значения и если надо задает новое значение, так чтобы оно была в области допустимых значений
    // конец 1-го способа вычеслений остатка заряда
    // -----------------------------------------------------------------------------------------------------------
  */
  // --------------------Конец расчёта остатка заряда аккумуляторной батареи---------------------------

  delay(5000);
}


// Функция чтения внутреннего опорного напряжения
float readVcc() {// универсальная для всех ардуин
  byte i;
  float result = 0.0;
  float tmp = 0.0;

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
  return result;// возвращает Vcc,прекращает вычисления в функции и возвращает значение (result) из прерванной функции в вызывающую (readVcc()=result)
}


#endif
