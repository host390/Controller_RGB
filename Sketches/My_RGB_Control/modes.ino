
/*
 * ТЗ
 * 1. Надо ввести актив в выбранный цвет
 * 2. Сохранять это в памяти
 * 3. 
 */


//int activeNumb = 0; // активная функция
bool transmissionActive = false;

/*
void returnParams() {
  if (encod.isSingle()) { // вернуться обратно
     flag_value = false;
     oled.invertText(false);
     changePreset();
     break;
   }      
   if (encod.isDouble()) {
     flag_params = false; // закрыть
     flag_value = false;
     oled.invertText(false);
     changePreset();
     break;
   }
}*/

//bool oneTranss = true;

//long timeTransRF; // таймер для отправки одиночных значений

void pushFunction() {
  switch (menuFunction) {
    case 0: // главное меню Тут либо всё офф либо на рандом эффекты пускать или что-то const
      //led.setRGB(0, 0, 0, 0);
      break;
    case 1: // Colors and
    case 2: // Effects
      
      if (flag_params and oneTranss) {
        oneTranss = false;

        Serial.println(F("Отправляю параметры активного: "));
        Serial.print(F("activeNumb: ")); Serial.println((navPos == 1) ? activeNumb + COLOR_PUNCTS : activeNumb);
        Serial.println(F("Значения: ///"));
        //radio.powerUp(); // может там с буффером проблема какя-то хз
        //radio.stopListening();
        timeTransRF = millis();
        while(1) { // это капец но как-то работает 
          if (millis() - timeTransRF >= 600) {
            break;
          }
          writeArrRF[0] = ACTIVENUMB;
          // Если это эффекты, то + COLOR_PUNCTS
          if (navPos == 1) writeArrRF[1] = activeNumb + COLOR_PUNCTS;
          else writeArrRF[1] = activeNumb;
          radio.write(&writeArrRF, 4); // было 2 стало 4 т.к. int 
          
          for (byte t = 0; t < maxParams; t++) { // прохожусь по всем значениям параметров выбранной функции
            writeArrRF[0] = SumNumber + t;
            writeArrRF[1] = vals[SumNumber + t];
            radio.write(&writeArrRF, 4);
          }
        }
        
        /*
        //Serial.print(F("Отправляю фулл: "));
        for (byte t = 0; t < maxParams; t++) {
          //Serial.print(SumNumber + t);
          writeArrRF[0] = SumNumber + t;
          writeArrRF[1] = vals[SumNumber + t];
          radio.write(&writeArrRF, 4);
          Serial.print(writeArrRF[0]); Serial.print(" ");
          Serial.print(writeArrRF[1]); Serial.println(" ");
          //radio.write(&writeArrRF, 4); 
          //for (byte i = 0; i < 3; i++) { // надо по 3 раза блин отправлять все значения
            //writeArrRF[0] = SumNumber + t;
            //writeArrRF[1] = vals[SumNumber + t];
            //radio.write(&writeArrRF, 4);
          //}
          */
          
        //}
        

        
//        vals[10] = activeNumb; // номер этого актива в массиве !!!
//        for (byte i = 0; i < 2; i++) {
//          radio.write(&vals, sizeof(vals));
//        }
//        for (byte i = 0; i < sizeof(vals) / 2; i++) {
//          Serial.print(vals[i]); Serial.print(" "); 
//        }
//        Serial.println();
      }

      if (flag_value) { // перед входом в цикл произвести некоторые преднастройки
        Serial.print(F("activeNumbTrans: "));  Serial.println(activeNumb);
        limitIncr = 255;
        if (navPos == 0) { // Цвета
          if (activeNumb == 3 and stepParams == 0) { // colorWheel Colors
            limitIncr = 1530;
          } else if (activeNumb == 4 and stepParams == 0) { // HEX Colors
            limitIncr = 15;
          }
        } 
        // Определяем максимальный предел для значений
        else if (navPos == 1) { // Эффекты
          if (
            (activeNumb + COLOR_PUNCTS == 5 and stepParams == 0) or // colorWheel_effects Speed
            (activeNumb + COLOR_PUNCTS == 6 and stepParams == 2) or
            (activeNumb + COLOR_PUNCTS == 7 and stepParams == 3) or // FireManual Speed
            (activeNumb + COLOR_PUNCTS == 8 and stepParams == 3) or
            (activeNumb + COLOR_PUNCTS == 9 and stepParams == 3) or
            (activeNumb + COLOR_PUNCTS == 10 and stepParams == 1)
            ) { 
            limitIncr = 1000;
          } else if (activeNumb + COLOR_PUNCTS == 5 and stepParams == 1) { // colorWheel_effects Step
            limitIncr = 500;
          }
          
        }
        Serial.print(F("limitIncr: "));  Serial.println(limitIncr);
      }
      
      while(flag_value) { // если изменяем пункт меню
        
        if (activeNumb == 4) butMatrixTick(); // по приколу
        encod.tick();
        
        if (encod.isSingle()) { // вернуться обратно
           flag_value = false;
           changePreset();
           break;
        }      
        if (encod.isDouble()) {
           flag_params = false; // закрыть параметры
           flag_value = false;

           oled.clear(); // очень тупое решение проблемы с отрисовкой...
           changePreset();
           
           oneTranss = true;



           Serial.println(F("---------СОХРАНЯЮ НОВЫЕ ЗНАЧЕНИЯ---------"));
          
           for (byte g = 0; g < maxParams; g++) { // прохожусь по всем значениям параметров выбранной функции
             Serial.print((SumNumber + g) * 2); Serial.print(" "); Serial.println(vals[SumNumber + g]);
             EEPROM.put((SumNumber + g) * 2, vals[SumNumber + g]); // адрес, значение
           }
           
           // сохраняем активный эффект
           if (navPos == 1) { // Если это эффекты, то + COLOR_PUNCTS
             Serial.print(ACTIVENUMB * 2); Serial.print(" "); Serial.println(activeNumb + COLOR_PUNCTS);
             EEPROM.put(ACTIVENUMB * 2, activeNumb + COLOR_PUNCTS);
             vals[ACTIVENUMB] = activeNumb + COLOR_PUNCTS;
           } else {
             Serial.print(ACTIVENUMB * 2); Serial.print(" "); Serial.println(activeNumb);
             EEPROM.put(ACTIVENUMB * 2, activeNumb);
             vals[ACTIVENUMB] = activeNumb;
           }

           // сохраняем активный экран в 100
           Serial.print("100"); Serial.print(" "); Serial.println(navPos_activ);
           EEPROM.update(100, navPos_activ); // адрес 100 
           //vals[NAVPOS_ACTIV] = navPos_activ;

           Serial.println(F("---------ВСЕ ЗНАЧЕНИЯ vals---------"));
           for (int i = 0; i < sizeof(vals) / 2; i++) {
              Serial.print(vals[i]); Serial.print(","); Serial.print(" ");
              if (i == 3 or i == 6 or i == 9 or i == 11 or i == 13 or i == 16 or i == 19 or i == 23 or i == 27 
                  or i == 31 or i == 33 or i == 34) {
                Serial.println();
              }
            }
            
           break;
        }
        
        // вот прям хз как это сделать??? при клике
        //writeArrRF[0] = stepParams + SumNumber; // пункт меню
        //writeArrRF[1] = vals[stepParams + SumNumber]; // погружаем значение
        
        if (encod.isTurn() || butClick) { // как это оптимизировать???
           butClick = false;
           increment = 0;
           if (encod.isRight()) increment = 1; // incr(&vals[stepParams + SumNumber], 1, 255);
           if (encod.isLeft()) increment = -1;
           if (encod.isFastR()) increment = 10;
           if (encod.isFastL()) increment = -10;
           
           incr(&vals[stepParams + SumNumber], increment, limitIncr);
           
           writeArrRF[0] = stepParams + SumNumber; // пункт меню
           writeArrRF[1] = vals[stepParams + SumNumber]; // погружаем значение
           //Serial.print(F("writeArrRF[0]:")); Serial.print(writeArrRF[0]); Serial.print(" ");
           //Serial.print(F("writeArrRF[1]:")); Serial.println(writeArrRF[1]);
           
           oled.invertText(false); // Кастыльный костыль, но круто работает :)
           oled.setCursor(x_pix , 3 + modeNum + stepParams - indexDown); // x y //oled.setCursor(35 , 2 + modeNum + stepParams); // x y
           oled.print("       "); //oled.print("    ");
           oled.invertText(true);
           oled.setCursor(x_pix , 3 + modeNum + stepParams - indexDown);
           
           if (navPos == 0 and activeNumb == 4 and stepParams == 0) {
             oled.print(vals[12]); // oled.print(colorNames[vals[12]]);
           } else {
             oled.print(vals[stepParams + SumNumber]);
           }
           
           //Serial.println(stepParams + SumNumber);
           //oled.update(); // обновление квадратика 
        //oled.rect(x_pix, (3 + modeNum + stepParams - indexDown) * 8, x_pix + 20, (3 + modeNum + stepParams + 1 - indexDown) * 8, OLED_STROKE);
           // Обновление квадратика
           oled.update(x_pix, (3 + modeNum + stepParams - indexDown) * 8, x_pix + 36, (3 + modeNum + stepParams + 1 - indexDown) * 8); 
           oled.invertText(false);
           
           //Serial.println(indexDown);
        }
        //Serial.print(writeArrRF[0]); Serial.print(" ");
        //Serial.print(writeArrRF[1]); Serial.print(" "); //Serial.println(vals[stepParams + SumNumber]);
        //Serial.println(writeArrRF[2]);
        radio.write(&writeArrRF, 4); // РАДИО отпраляю 1 значения оющего массива 2 байта
        //radio.write(&vals[stepParams + SumNumber], 2); // РАДИО отпраляю 1 значения общего массива 2 байта
        
      }
      /*
      switch (activeNumb) { // colorWheel8 №1 
        
        case 0: // у нас есть активный теперь мы 1 раз отправляем его значения
                
          if (!transmissionActive) { // если ещё не отправили значение активного (возможно нужна будет обратная связть для точности)
            //Serial.println(vals[0]);
            radio.write(&vals, sizeof(vals)); // отправить по радио sizeof(vals) == 4
            transmissionActive = true;
          }
          
          while (flag_value) { // есть зависание в первые пару секунд надо что-то думать, а так что-то работает хы-хы
            //Serial.println("Заходим в цикл"); stepParams == 0 and stepParams == 1
             
             encod.tick();
             
             if (encod.isTurn()) {
               increment = 0;
               if (encod.isRight()) increment = 1; // incr(&vals[stepParams + SumNumber], 1, 255);
               if (encod.isLeft()) increment = -1;
               if (encod.isFastR()) increment = 10;
               if (encod.isFastL()) increment = -10;
               //vals[0] = constrain(vals[0] + increment, 0, 255);
               incr(&vals[stepParams], increment, 255); //она не будет работать хотя???
               
               oled.invertText(false); // Кастыльный костыль, но круто работает :)
               oled.setCursor(35 , 3 + stepParams);
               oled.print("    ");
               oled.invertText(true);
               oled.setCursor(35 , 3 + stepParams);
               oled.print(vals[stepParams]);
               oled.update(35, 24, 80, 40); // обновление квадратика
             }

            //radio.write(&vals, sizeof(vals)); // РАДИО
            //Serial.println(vals[stepParams]); 
             
             
             //Serial.println(vals[0]);
          }
          
          //led.setWheel8(vals[0], vals[1]);
          break;
        default: // если ничего нету
          led.setRGB(0, 0, 0, 0);
      }

    break;
    */
  }
}
