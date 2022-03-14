

bool transmissionActive = false;

void pushFunction() {
  switch (menuFunction) {
    case 0:
      break;
    case 1: // Colors and
    case 2: // Effects
      if (flag_params and oneTranss) {
        oneTranss = false;
        // Serial.println(F("Отправляю параметры активного: "));
        // Serial.print(F("activeNumb: ")); Serial.println((navPos == 1) ? activeNumb + COLOR_PUNCTS : activeNumb);
        timeTransRF = millis();
        while(1) {
          if (millis() - timeTransRF >= 600) {
            break;
          }
          writeArrRF[0] = ACTIVENUMB; // номер переменой актив в массиве 
          // Если это эффекты, то + COLOR_PUNCTS
          if (navPos == 1) writeArrRF[1] = activeNumb + COLOR_PUNCTS;
          else writeArrRF[1] = activeNumb;
          radio.write(&writeArrRF, 4);
          for (byte t = 0; t < maxParams; t++) { // прохожусь по всем значениям параметров выбранной функции
            writeArrRF[0] = SumNumber + t;
            writeArrRF[1] = vals[SumNumber + t];
            radio.write(&writeArrRF, 4);
          }
        }
      }

      if (flag_value) { // перед входом в цикл произвести некоторые преднастройки
        // Serial.print(F("activeNumbTrans: ")); Serial.println(activeNumb);
        limitIncr = 255;
        if (navPos == 0) { // Цвета
          if (activeNumb == 3 and stepParams == 0) limitIncr = 15;  // (color_HEX: Colors) 
          // else if (activeNumb == 4 and stepParams == 0) limitIncr = 10000; // (Kelvin: Temperature)
        } else if (navPos == 1) { // Эффекты
          if (
            (activeNumb + COLOR_PUNCTS == 5 and stepParams == 0) or // colorWheel_effects: Speed
            (activeNumb + COLOR_PUNCTS == 6 and stepParams == 2) or // Fire: Speed
            (activeNumb + COLOR_PUNCTS == 7 and stepParams == 3) or // FireManual: Speed
            (activeNumb + COLOR_PUNCTS == 8 and stepParams == 3) // Strobe: Speed
            ) { limitIncr = 1000;
          } else if (activeNumb + COLOR_PUNCTS == 5 and stepParams == 1) { // colorWheel_effects: Step
            limitIncr = 500;
          } else if (
            (activeNumb + COLOR_PUNCTS == 9 and stepParams == 0) or // FadeTo: Сolor1
            (activeNumb + COLOR_PUNCTS == 9 and stepParams == 1)  // FadeTo: Сolor1
            ) { limitIncr = 15; 
          } else if (activeNumb + COLOR_PUNCTS == 9 and stepParams == 2) {
            limitIncr = 3000;
          }
        }
        // Serial.print(F("limitIncr: "));  Serial.println(limitIncr);
      }
      while(flag_value) { // если изменяем пункт меню
        if (activeNumb == 4) butMatrixTick(); // по приколу
        encod.tick();
        if (encod.isSingle()) { // вернуться обратно
           flag_value = false;
           changePreset();
           break;
        }      
        if (encod.isDouble()) { // при 2-ном клике сохраняем значения
           flag_params = false; // закрыть параметры
           flag_value = false;
           oled.clear(); // решение проблемы с отрисовкой
           changePreset();
           oneTranss = true;
           // Serial.println(F("---------СОХРАНЯЮ НОВЫЕ ЗНАЧЕНИЯ---------"));
           for (byte g = 0; g < maxParams; g++) { // прохожусь по всем значениям параметров выбранной функции
             // Serial.print((SumNumber + g) * 2); Serial.print(" "); Serial.println(vals[SumNumber + g]);
             EEPROM.put((SumNumber + g) * 2, vals[SumNumber + g]); // адрес, значение
           }
           // сохраняем активный эффект
           if (navPos == 1) { // Если это эффекты, то + COLOR_PUNCTS
             // Serial.print(ACTIVENUMB * 2); Serial.print(" "); Serial.println(activeNumb + COLOR_PUNCTS);
             EEPROM.put(ACTIVENUMB * 2, activeNumb + COLOR_PUNCTS);
             vals[ACTIVENUMB] = activeNumb + COLOR_PUNCTS;
           } else {
             // Serial.print(ACTIVENUMB * 2); Serial.print(" "); Serial.println(activeNumb);
             EEPROM.put(ACTIVENUMB * 2, activeNumb);
             vals[ACTIVENUMB] = activeNumb;
           }
           // сохраняем активный экран в 100
           // Serial.print("100"); Serial.print(" "); Serial.println(navPos_activ);
           EEPROM.update(100, navPos_activ); // адрес 100 
           // Serial.println(F("---------ВСЕ ЗНАЧЕНИЯ vals---------"));
           break;
        }
        
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

           if (navPos == 0 and activeNumb == 3 and stepParams == 0 ) { // это чтоб можно было рисовать текст
            oled.print(colorValsHEX[vals[9]]);
           } else if (navPos == 1 and activeNumb + COLOR_PUNCTS == 9 and stepParams == 0) {
            oled.print(colorValsHEX[vals[27]]);
           } else if (navPos == 1 and activeNumb + COLOR_PUNCTS == 9 and stepParams == 1) { 
            oled.print(colorValsHEX[vals[28]]);
           } else {
            oled.print(vals[stepParams + SumNumber]);
           }

           // Обновление квадратика
           oled.update(x_pix, (3 + modeNum + stepParams - indexDown) * 8, x_pix + 36, (3 + modeNum + stepParams + 1 - indexDown) * 8); 
           oled.invertText(false);
           //Serial.println(indexDown);
        }
        
        // Serial.print(writeArrRF[0]); Serial.print(" "); Serial.print(writeArrRF[1]); Serial.println(" ");
        radio.write(&writeArrRF, 4); // РАДИО отпраляю 1 значения оющего массива 2 байта
      }
  }
}
