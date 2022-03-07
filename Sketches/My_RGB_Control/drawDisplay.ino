
const uint8_t ptr_bmp[] PROGMEM = { // Битмап с картинкой стрелочки (Если нужен)
  0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0xFF, 0xFF, 0x7E, 0x3C, 0x18,
};

const uint8_t checked[] PROGMEM = { // Битмап с картинкой стрелочки (Если нужен)
//  0xfe, 0x7f, 0x3f, 0x1f, 0x1f, 0x3f, 0x7f, 0xfe
  0x7e, 0x81, 0x91, 0xa1, 0x91, 0x89, 0x84, 0x72
};


void drawInfo() { // первая строка (пресет и режим)
  if (startProgram) { // если надо прям всё перерисовать
    oled.clear();
    startProgram = false;
  }
//  oled.clear(); // !!!

  oled.home();
  oled.print("           "); // ну тип просто кучка пробелов неа? ^^
  oled.setScale(2); // шрифт 2

  switch (navPos) { // это чтоб поправить все заголовки по центру
    case 0: oled.setCursor(30, 0);
      break;
    case 1: oled.setCursor(20, 0);
      break;
  }
  printFromPGM(navPos, 0); // oled.print(modeNames[navPos]);
  
//  oled.update();
}




const uint8_t colorBit_44x44[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xC0, 0x20, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x01, 0xC1, 0xE1, 0xE1, 0xE1, 0xE1, 0xC1, 0x81, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x82, 0x2C, 0x38, 0x10, 0x18, 0x08, 0x0C, 0x06, 0x02, 0x03, 0x01, 0x01, 0x00, 0xC0, 0x7F, 0x00, 0x00, 0x00, 
  0x1C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3C, 0xFC, 0x7C, 0xF8, 0x78, 0x00, 0x01, 0x03, 0x03, 0x07, 0x01, 0x80, 0x40, 0x80, 0x60, 0x10, 0x08, 0x18, 0x25, 0x53, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xC0, 0xE0, 0xB0, 0x18, 0x0C, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x30, 0x38, 0xFC, 0x78, 0xFC, 0x70, 0x80, 0x60, 0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x80, 0x83, 0x44, 0x2C, 0x18, 0x0C, 0x08, 0x04, 0x03, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x03, 0x0E, 0x18, 0xF0, 0x00, 0x00, 0x00, 0x00, 
  0x03, 0x04, 0x88, 0xD0, 0x60, 0x10, 0x08, 0x04, 0x02, 0x01, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x24, 0xF2, 0xF1, 0xF0, 0xF0, 0xF0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x30, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0x04, 0x04, 0x04, 0x04, 0x06, 0x02, 0x03, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x04, 0x04, 0x0C, 0x09, 0x09, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const uint8_t EffectBit_44x44[] PROGMEM = {
  0x80, 0x80, 0x40, 0x38, 0x04, 0x3C, 0x78, 0xC0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xB8, 0x2E, 0x03, 0x01, 0x01, 0x01, 0x01, 0x06, 0x38, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x06, 0x06, 0x0C, 0x78, 0xC0, 0xF8, 0x7C, 0x0E, 0x06, 0x06, 0x87, 0x73, 0xA8, 0x88, 0x08, 0x08, 0x08, 0x0C, 0x04, 0x04, 0x04, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x04, 0x04, 0x04, 0x08, 0x08, 0x08, 0x88, 0x88, 0xF0, 
  0xC0, 0x20, 0x30, 0x08, 0x18, 0x8C, 0x02, 0xC6, 0x41, 0x21, 0x01, 0x18, 0x00, 0x89, 0x06, 0xC0, 0x8C, 0x50, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xD8, 0x0C, 0x06, 0x03, 0x01, 0x00, 
  0x10, 0x08, 0x84, 0x43, 0x20, 0x21, 0x10, 0x18, 0x00, 0x0C, 0x80, 0x43, 0x20, 0x21, 0x10, 0xF8, 0x87, 0x00, 0x00, 0x80, 0x00, 0x00, 0xC0, 0xC0, 0x60, 0x30, 0x10, 0x10, 0x98, 0xB0, 0x60, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0xFC, 0x00, 0x00, 0x00, 0x00, 
  0x42, 0x21, 0x10, 0x88, 0x88, 0xC4, 0x62, 0x22, 0x31, 0x19, 0x08, 0x0C, 0x06, 0x02, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x61, 0xB0, 0xB0, 0xB0, 0x98, 0x0F, 0x0B, 0x05, 0x0F, 0x18, 0xB1, 0xB1, 0xB1, 0x61, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 
  0x0C, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0F, 0x0A, 0x04, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

int x = 10; // мувинг пиксилей слева и справа

void OLED_Main() {
  
  switch (navPos) { //BITMAP_NORMAL / BITMAP_INVERT
    case 0:
      // Отрисовка первой иконки
      //oled.rect(2 + x, 18, 45 + x, 61, OLED_STROKE); // рамка 44 на 44
      //oled.line(2 + x, 18, 45 + x, 61); // линия диагональная для проверки

      oled.rect(0 + x, 16, 47 + x, 63, OLED_CLEAR); // oled.rect(2 + x, 18, 45 + x, 61, OLED_CLEAR); // стираю битмап
      oled.drawBitmap(2 + x, 18, colorBit_44x44, 44, 44, BITMAP_INVERT); // рисую битмап
      
      // Отрисовка второй иконки
      //oled.rect(82 - x, 18, 125 - x, 61, OLED_STROKE); // рамка 44 на 44

      oled.rect(80 - x, 16, 127 - x, 63, OLED_CLEAR); //oled.rect(82 - x, 18, 125 - x, 61, OLED_CLEAR); // стираю 2 иконку
      oled.drawBitmap(82 - x, 18, EffectBit_44x44, 44, 44, BITMAP_NORMAL);

      // Рисую рамку у 1-ого
      oled.rect(0 + x, 16, 47 + x, 63, OLED_STROKE); // рамка 48 на 48
      //oled.line(0 + x, 16, 47 + x, 63); // проверка
      
      // Стираю рамку у 2 ого
      oled.line(80 - x, 16, 127 - x, 16, OLED_CLEAR); 
      oled.line(80 - x, 63, 127 - x, 63, OLED_CLEAR); 
      oled.line(80 - x, 16, 80 - x, 63, OLED_CLEAR); 
      oled.line(127 - x, 16, 127 - x, 63, OLED_CLEAR);
      //oled.line(82 - x, 18, 125 - x, 61, OLED_CLEAR); // это линия проверки у 2-ого
      break;
    case 1:
      // Отрисовка первой иконки
      //oled.rect(2 + x, 18, 45 + x, 61, OLED_STROKE); // рамка 44 на 44
      //oled.line(2 + x, 18, 45 + x, 61); // проверка
      
      oled.rect(0 + x, 16, 47 + x, 63, OLED_CLEAR); // oled.rect(2 + x, 18, 45 + x, 61, OLED_CLEAR); // стираю битмап
      oled.drawBitmap(2 + x, 18, colorBit_44x44, 44, 44, BITMAP_NORMAL);
      
      // Отрисовка второй иконки
      //oled.rect(82 - x, 18, 125 - x, 61, OLED_STROKE); // битмап 44 на 44
      //oled.line(82 - x, 18, 125 - x, 61); // проверка
      
      oled.rect(80 - x, 16, 127 - x, 63, OLED_CLEAR); // oled.rect(82 - x, 18, 125 - x, 61, OLED_CLEAR); // стираю 2 иконку
      oled.drawBitmap(82 - x, 18, EffectBit_44x44, 44, 44, BITMAP_INVERT);
      
      // Надо стиреть oled.rect(0 + x, 16, 47 + x, 63, OLED_STROKE);
      
      // Стираю рамку у 1 ого
      oled.line(0 + x, 16, 47 + x, 16, OLED_CLEAR); 
      oled.line(0 + x, 63, 47 + x, 63, OLED_CLEAR);
      oled.line(0 + x, 16, 0 + x, 63, OLED_CLEAR);
      oled.line(47 + x, 16, 47 + x, 63, OLED_CLEAR);
      //oled.line(2 + x, 18, 45 + x, 61, OLED_CLEAR); // это просто линия проверки у 1-ого
      
      // Рамка у 2 ого
      oled.rect(80 - x, 16, 127 - x, 63, OLED_STROKE); // рамка 48 на 48 0 -> 45 _ 
      //oled.line(80 - x, 16, 127 - x, 63); // проверка

      break;
  }
  
  
  oled.update();
}

void printFromPGM(int charMap, int numberSlaide) { // а сколько это памяти забивет?
  // тоже по 1 символу очень быстро :) Но только для одного массива, а как для всех ????? Пока самый лучший вариант :=
  uint16_t ptr = pgm_read_word(&(modeNames[charMap]));// получаем адрес modeNames
  if (numberSlaide == 0) { // modeNames
    ptr = pgm_read_word(&(modeNames[charMap]));// получаем адрес modeNames
  } else if (numberSlaide == 1) { // labelsColors
    ptr = pgm_read_word(&(labelsColors[charMap]));// получаем адрес labelsColors
  } else if (numberSlaide == 2) { // сolorsParams
    ptr = pgm_read_word(&(paramsColor[charMap]));// получаем адрес сolorsParams
  }
  while (pgm_read_byte(ptr) != NULL) { // всю строку до нулевого символа
    oled.print(char(pgm_read_byte(ptr))); // выводим в монитор или куда нам надо
    ptr++; // следующий символ
  }
}

void printPointer(int pointer) {
  //  oled.invertText(true);
  // я не понимаю почему это всё ломает ?????
  oled.drawBitmap(0, pointer * 8 + 16, ptr_bmp, 10, 8); // Указатель в виде битмапа (картинки)
//  oled.print(">");
}

void printActiveNum() {

  // может здесь или не здесь ????
  if (navPos_activ == navPos) { // если меню в котором активный совпадает с открытым
    checked_flag = true; // рисуем галоку на активном
  } else {
    checked_flag = false;
  }
  
  if (checked_flag) {
    oled.drawBitmap(90, (activeNumb - indexDown) * 8 + 16, checked, 8, 8);
    Serial.print(F("Отрисовываю Галочку ")); Serial.println(activeNumb - indexDown); 
    //checked_flag = false;
  }
//  oled.drawBitmap(90, ((activeNumb - indexDown < 0) ? 0 : activeNumb - indexDown) * 8 + 16, checked, 8, 8);
//  if (activeNumb < 5 and navPos == 0) { // если у нас активный цвет
//    oled.drawBitmap(90, (activeNumb - indexDown) * 8 + 16, checked, 8, 8); // выводит но я не знаю как поправить баг с выводом на шапку ???
//  } else if (activeNumb > 5 and navPos == 1){ // если активный эффект
//    //oled.drawBitmap(90, (activeNumb - indexDown) * 8 + 16, checked, 8, 8);
//  }
  
}

//bool step_down_flag = true; 
//int indexDown = 0; // индекс опускания меню
//int activeNumb = 0; // активная функция

byte printPuncts; // сколько пунктов надо отрисовать
byte numberStartPrint; // с какого момента начать отрисовывать всё это для цветов, эффектов

// Тут есть куча богов с отрисовкой пунктов когда их мало к примеру
void drawSettings() { // Рисую функцию

  if (navPos == 0) { // сколько пунктов отрисовать
    numberStartPrint = 0;
    if (COLOR_PUNCTS > 6) printPuncts = 6;
    else printPuncts = COLOR_PUNCTS;
  } else if (navPos == 1) {
    
    //modeNum -= COLOR_PUNCTS;
    
    numberStartPrint = COLOR_PUNCTS;
    printPuncts = EFFECTS_PUNCTS;
  }
   
  oled.setScale(1);
  oled.setCursor(0, 2);
  for (uint8_t i = 0; i < printPuncts; i++) { //6 (COLOR_PUNCTS > 6) ? 6 : COLOR_PUNCTS вывод всех пунктов меню // если много пунктов i < 6   - maxParams
    if (modeNum > indexDown + 5) { // если курсор вышел за пределы и 5 это максимальное количество элементов на эране
      indexDown = modeNum - 5;
    }
    // Нужно знать какое количество пунктов вывелось до параметров
    if (flag_params and modeNum + 1 + maxParams > 6 + indexDown) { // если параметр открыт и ...
      //Serial.println("Не все параметры на экране");
      int hidenNumParams = modeNum + 1 + maxParams - 6 - indexDown; // сколько параметров нехватает
      //  Serial.println(hidenNumParams);
      if (hidenNumParams > 0) { // and stepParams == 1
        indexDown += hidenNumParams; // + 1 modeNum - 5 +
      }
    } 
    // Если у нас при закрытии последних параметров остаётся пустое простанство, то убрать его как-то но как ???? maxParams
    else if (flag_params == false and maxParams >= indexDown) { // indexDown == maxParams indexDown == 1 если параметр закрыт и есть пустое простанство 
      indexDown = 0;
      //Serial.println("Все параметры можно поместить в экран");
    }
    
    if (COLOR_PUNCTS > 5) { // если есть пункты за рамкой
      if (!flag_params and indexDown > COLOR_PUNCTS - 6) { // что такое 3 :* 9 - 6 = 3
        indexDown = 3;
      }
    }
    
    if (indexDown > modeNum) { // если меню идёт на вверх
      indexDown--;
    }
    
    

    oled.print("  "); // 
    // Отпределяем пункт где сейчас курсор
    if (constrain(modeNum - indexDown, 0, 6) == i) { // рисуем указатель
      Serial.print(F("Aктивный i: "));  Serial.println(constrain(modeNum - indexDown, 0, 6));
      oled.invertText(true); // надо как-то modeNum зацыклить, что если он > 6, то
      Serial.print(F("indexDown: "));  Serial.println(indexDown);
      Serial.print(F("Параметров открыто:"));  Serial.println(maxParams);
      
    } 
    
    // Рисуем пунткы меню (navPos == 1) ? COLOR_PUNCTS : 0
    printFromPGM(i + indexDown + numberStartPrint, 1); // рисуем пунткы меню // printFromPGM(modeNum > 5 ? i + (modeNum - 5) : i); // 
    oled.invertText(false);

//    if (activeNumb - indexDown == i) { // если это активный момент and activeNumb == modeNum
//      oled.print(" <");
//    }
    
    oled.println("             "); // эти пробелы для стёрки и перенса значений  
    // Теперь нужно вывести все параметры если выбрано
    if (flag_params and modeNum == i + indexDown) { // если 
      for (uint8_t j = 0; j < maxParams; j++) { // выводим каждый параметр
        oled.print("   "); // отступ для стрелочки
        if (!flag_value) { // если выбрали параметр НЕ выбран 
          if (stepParams == j) { // если отрисовываем активный
            oled.invertText(true);
          }
        } 
        // Рисуем параметры выбранного меню
        printFromPGM(j + SumNumber, 2); // 2 это параметры пункта колор так сказать да :7
        oled.invertText(false);
        oled.print(" "); // отступ от параметра к значению
        if (flag_value) { // если выбрали параметр
          if (stepParams == j) { // если отрисовываем активный
            oled.invertText(true);
          }
        }
        
        if (navPos == 0 and activeNumb == 4 and j == 0) { // это чтоб можно было рисовать текст
          oled.print(vals[12]); // oled.print(colorNames[vals[12]]);
        } else {
          oled.print(vals[j + SumNumber]);
          //oled.print(vals[stepParams + SumNumber]);
        }
        
        //oled.print(vals[j + SumNumber]);
        oled.invertText(false);
        oled.print("            "); //  пробелы для убирания текста после значений пунктов
        oled.println();
      }
    }
  }
  printPointer(modeNum - indexDown); // выводим указатель
  printActiveNum(); // отрисовываем галочку над активным
  oled.update();
}
