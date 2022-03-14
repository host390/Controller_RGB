
// умный инкрементатор для настроек
void incr(int* val, int incr, int limit) {
  *val += incr;
  if (*val > limit) *val = limit;
  if (*val < 0) *val = 0;
}

void changePreset() {
  drawInfo();
  if (menuFunction == 0) { // если главное меню
    OLED_Main(); // рисую главное меню
  } else if (menuFunction == 1) { // если функция Colors
    drawSettings();
  } else if (menuFunction == 2) { // если эффекты
    drawSettings();
  }
}

byte x_pix = 0; // количество пикселей необходимое для рисования пунктов

void controlTick() {
  encod.tick();
  if (encod.isSingle()) { // хотим куда-то зайти // isClick
    if (menuFunction == 0) { // если мы сейчас на главном меню
      flag_function = true; // открыть функцию
      // Serial.print(F("flag_function: ")); Serial.println(true);
      startProgram = true;
    } else if (menuFunction == 1) { // если в колоре
      if (!flag_params) { // параметры закрыты
        flag_params = true; // открыть
        stepParams = 0; // обнулить до 1-ого
        flag_value = false;
      } else { // если параметр открыт
        flag_value = !flag_value; // выбираем параметр для редактирования flag_value == true, если открыт
      }
    } else if (menuFunction == 2) { // Эффекты
      if (!flag_params) { // параметры закрыты
        flag_params = true; // открыть
        stepParams = 0; // обнулить до 1-ого
        flag_value = false;
      } else { // если параметр открыт
        flag_value = !flag_value; // выбираем параметр для редактирования flag_value == true, если открыт
      }    
    }
    if (flag_function and navPos == 0) { // если выбрали Цвета
    // Serial.println(F("Зашли в цвета"));
      if (!flag_params) { // ещё не зашли в параметр
        modeNum = 0; // сбрасываем счётчик пунктов до 0
        maxParams = 0;
        SumNumber = 0;
        menuFunction = 1; // переходим в функцию цвета
        // Serial.print(F("menuFunction: ")); Serial.println(menuFunction);
      } else { // зашли в параметр
        activeNumb = modeNum; // теперь это активный элемент
        switch (modeNum) { // здесь я хочу понять какой именно пункт мне раскрывать 
          case 0: // RGB
            maxParams = 4; // какое количчество пунктов
            SumNumber = 0; // с какого начинаются пункты в массиве paramsColor[]
            if (stepParams == 3) x_pix = 90;
            else x_pix = 35;
            break;
          case 1: // HSV
            maxParams = 3; 
            SumNumber = 4;
            if (stepParams == 0) x_pix = 60; // Color
            else x_pix = 90;
            break; 
          case 2: // colorWheel
            maxParams = 2;
            SumNumber = 7;
            if (stepParams == 0) x_pix = 60;
            else x_pix = 90;
            break;  
          case 3: // HEX
            maxParams = 2;
            SumNumber = 9;
            if (stepParams == 0) x_pix = 60;
            else x_pix = 90;
            break;      
          case 4: // Kelvin
            maxParams = 2;
            SumNumber = 11;
            if (stepParams == 0) x_pix = 96;
            else x_pix = 90;
            break;
          default: // если ничего нету
            maxParams = 0;
            SumNumber = 0;
        }
      }
    } else if (flag_function and navPos == 1) { // Эффекты
      // Serial.println(F("Зашли в эффекты"));
      if (!flag_params) { // ещё не зашли в параметр
        maxParams = 0;
        SumNumber = 0;
        menuFunction = 2; // переходим в Эффекты 
        // Serial.print(F("menuFunction: ")); Serial.println(menuFunction);
        modeNum = 0; // сбрасываем счётчик пунктов до 0
        // Serial.print(F("modeNum: ")); Serial.println(modeNum);
      } else { // зашли в параметр
         activeNumb = modeNum; // теперь это активный элемент + COLOR_PUNCTS ?
         // Serial.print(F("activeNu: ")); Serial.println(activeNumb);
         switch (modeNum) { // здесь я хочу понять какой именно пункт мне раскрывать 
          case 0: // colorWheel
            maxParams = 3; // какое количчество пунктов
            SumNumber = 13; 
            if (stepParams == 0) x_pix = 60; // Speed
            else if (stepParams == 1) x_pix = 54;
            else x_pix = 90;
            break;
          case 1: // Fire
            maxParams = 3;
            SumNumber = 16;
            if (stepParams == 2) x_pix = 60;
            else x_pix = 90; // Max_Bright
            break;
          case 2: // FireManual
            if (stepParams == 0 or stepParams == 3) x_pix = 60; // Color Speed
            else x_pix = 90; // Max_Bright
            maxParams = 4;
            SumNumber = 19;
            break;
          case 3: // Strobe
            maxParams = 4;
            SumNumber = 23;
            if (stepParams == 0 or stepParams == 3) x_pix = 60; // Color Speed
            else x_pix = 90; // Saturation Brightness
            break;
          case 4: // FadeTo
            maxParams = 3;
            SumNumber = 27;
            if (stepParams == 0 or stepParams == 1) x_pix = 72; // Color1, Color2
            else x_pix = 60; // Speed
            break;
          case 5: // rainbowBreath
            maxParams = 2;
            SumNumber = 30;
            if (stepParams == 0) x_pix = 60; // Speed
            else if (stepParams == 1) x_pix = 54; // Step
            break; 
         }
      }
    }
    
    if (flag_params) { // and checked_flag если зашли в пункт меню и нужно поставить галочку
      navPos_activ = navPos; // сохраняем значение меню где активная функция
      // Serial.print(F("-> navPos_activ: ")); Serial.println(navPos_activ);
    }
    if (navPos_activ == navPos) { // если меню в котором активный совпадает с открытым
      checked_flag = true; // рисуем галоку на активном
    } else {
      checked_flag = false;
    }
    changePreset(); // отрисоваться
  }


  if (encod.isDouble()) {
    //Serial.println("Я тут");
    if (flag_function and !flag_params) { // если открыта функция и мы не в параметре
      menuFunction = 0; // возвращяемся в главное меню
      flag_function = false;
      startProgram = true; // всё стерерть
    }
    if (flag_params) { // параметры открыт
      flag_params = false; // закрыть
      oneTranss = true;
      oled.clear(); // очень тупое решение проблемы с отрисовкой...
    }
    changePreset();
  }


  if (encod.isTurn()) {
    switch (menuFunction) { // определяем в каком слайде мы сейчас
      case 0: // слайд главного меню
        if (encod.isRight()) incr(&navPos, 1, MENU_ITEMS - 1);
        if (encod.isLeft()) incr(&navPos, -1, MENU_ITEMS - 1);
        // Serial.print(F("navPos: ")); Serial.println(navPos);
        break;
      case 1: // это для Цветов
        if (!flag_params) { // если пункт меню ещё не выбран ...
          if (encod.isRight()) incr(&modeNum, 1, COLOR_PUNCTS - 1); // presetNum +1 огранич 0
          if (encod.isLeft()) incr(&modeNum, -1, COLOR_PUNCTS - 1); // presetNum -1 огранич 0
          // Serial.print(F("modeNum: ")); Serial.println(modeNum);
        } else { // если выбран ...
          if (flag_value) { // если изменяем значение параметра
          } else { // если выбираем параметр для изменения
            if (encod.isRight()) incr(&stepParams, 1, maxParams - 1);
            if (encod.isLeft()) incr(&stepParams, -1, maxParams - 1);
           // Serial.print(F("stepParams: ")); Serial.println(stepParams);
          }
        }
        break;
      case 2: // Эффекты
        if (!flag_params) { // если пункт меню ещё не выбран
          if (encod.isRight()) incr(&modeNum, 1, EFFECTS_PUNCTS - 1); // presetNum +1 огранич 0
          if (encod.isLeft()) incr(&modeNum, -1, EFFECTS_PUNCTS - 1); // presetNum -1 огранич 0
          // Serial.print(F("modeNum: ")); Serial.println(modeNum);
        } else { // если выбран
          if (flag_value) { // если изменяем значение параметра
          } else { // если выбираем параметр для изменения
            if (encod.isRight()) incr(&stepParams, 1, maxParams - 1);
            if (encod.isLeft()) incr(&stepParams, -1, maxParams - 1);
            // Serial.print(F("stepParams: ")); Serial.println(stepParams);
          }
        }
        break;
    }
    changePreset(); // рисуем всё
  }
}

bool butClick = false; // Кнопка нажата
byte butActiv; // Какая кнопка

void butMatrixTick() {
  char customKey = customKeypad.getKey(); //записывем нажатый символ
  if ((customKey and !flag_params) || (customKey and flag_value)) { //если что-то нажато и ещё не в параметрах
    butClick = true;
    switch(customKey) {
      case '0':
        butActiv = 0;
        break;
      case '1':
        butActiv = 1;
        break;
      case '2':
        butActiv = 2;
        break;
      case '3':
        butActiv = 3;
        break;
      case '4':
        butActiv = 4;
        break;
      case '5':
        butActiv = 5;
        break;
      case '6':
        butActiv = 6;
        break;
      case '7':
        butActiv = 7;
        break;
      case '8':
        butActiv = 8;
        break;
      case '9':
        butActiv = 9;
        break;
      case 'A':
        butActiv = 10;
        break;
      case 'B':
        butActiv = 11;
        break;
      case 'C':
        butActiv = 12;
        break;
      case 'D':
        butActiv = 13;
        break;
      case 'F':
        butActiv = 14;
        break;
      case 'G':
        butActiv = 15;
        break;
    }
    
     // Serial.print(F("Нажата Кнопка: ")); Serial.println(butActiv);
     
     activeNumb = 3;
     vals[9] = butActiv;
     if (!flag_value) {
       changePreset();
       timeTransRF = millis();
       while(1) { // это капец но как-то работает 
         if (millis() - timeTransRF >= 600) {
            break;
         }
         writeArrRF[0] = ACTIVENUMB;
         writeArrRF[1] = activeNumb;
         radio.write(&writeArrRF, 4);
         writeArrRF[0] = 9;
         writeArrRF[1] = vals[9];
         radio.write(&writeArrRF, 4);
       }
     }
  }
}
