

/*
//void printPointer(uint8_t pointer) {
//  if (pointer == 0) {
//    //oled.roundRect(0, 22, 33, 51, OLED_STROKE);
//    oled.rect(0 + 11, 16, 47 + 11, 63, OLED_STROKE); // рамка
//  } else if (pointer == 1) {
//    oled.rect(79 - 11, 16, 127 - 11, 63, OLED_STROKE); // рамка
//    //oled.roundRect(34, 22, 67, 51, OLED_STROKE);
//  } 
//
//
//}

// умный инкрементатор для настроек
void incr(int val, int incr, int limit) { // что это за ****** ?????
  val += incr;
  if (val > limit) val = limit;
  if (val < 0) val = 0;
}

uint8_t pointer_last;



uint8_t val_R = 0;
uint8_t val_G = 0;
uint8_t val_B = 10;
//int vals[3] = {100, 100, 100}; // массив параметров цветов R G B

//const char channel_R[] PROGMEM = "R";
//const char channel_G[] PROGMEM = "G";
//const char channel_B[] PROGMEM = "B";
//const char* const names[] PROGMEM = { // массив с названиями каналов цвета
//  channel_R, channel_G, channel_B,
//};

//const char *modeNames[] = { // список названий режимов
//  "RGB", "Цвета"
////  "RGB", "HSV", "Color", "ColorSet", "Kelvin", "ColorW",
////  "Fire", "FireM", "Strobe", "StrobeR", "Police",
//};

//const char *labels[] = {      // список названий лейблов
//  "R", "G", "B",
//};


int8_t arrowPos = 0;  // позиция стрелки

bool chosenPoint = false;

void print_OLED_Color(void) {
  oled.clear();
  oled.home();
  oled.invertText(false);
  oled.setCursor(30, 0);
  oled.print(modeNames[1]);
//  oled.print("Цвета");

//  oled.setCursor(0, 2); oled.print(labels[0]); oled.print(F(":")); oled.print(vals[0]);
//  oled.setCursor(0, 4); oled.print(labels[1]); oled.print(F(":")); oled.print(vals[1]);
//  oled.setCursor(0, 6); oled.print(labels[2]); oled.print(F(":")); oled.print(vals[2]);
//  oled.setCursor(0, 6); oled.print("B: "); oled.println(val_B);

  /*
  if (chosenPoint == false) {
     oled.invertText(true);
     switch (arrowPos) {
     case 0:
       oled.setCursor(0, 2); oled.print("R");
       break;
     case 1: 
       oled.setCursor(0, 4); oled.print("G");
       break;
     case 2:
       oled.setCursor(0, 6); oled.print("B");
       break;
    }
  } else {
    
    switch (arrowPos) {
     case 0:
       oled.setCursor(0, 2); oled.print("R : "); 
       oled.invertText(true); // !!!!
       oled.println(val_R);   
       break;
     case 1: 
       oled.setCursor(0, 4); oled.print("G : "); 
       oled.invertText(true); // !!!!
       oled.println(val_G); 
       break;
     case 2:
       oled.setCursor(0, 6); oled.print("B : "); 
       oled.invertText(true); // !!!!
       oled.println(val_B); 
       break;
    }
  }
  oled.update();
}


void colors(void) {
  print_OLED_Color();
  while (1) {
    
    static uint8_t pointer = 0;
    static bool start_menu = true;
    static bool GoSetParams = false;
    
    ok.tick();
    encod.tick();

    if (encod.isClick()) { // выбрали пункт меню
      chosenPoint = !chosenPoint;
      print_OLED_Color();
    }

    if (encod.isDouble() or ok.isDouble()) {
      //  pointer = 0;
//      Serial.println("Go home");
      moveMainMenu = true;
      return; // return возвращает нас в предыдущее меню
    }

    if (encod.isTurn()) {

      if (chosenPoint == false) { // если НЕ выбран пункт меню
        if (encod.isRight()) {
          arrowPos++;
          if (arrowPos >= 3) arrowPos = 2;  // ограничиваем позицию курсора
        }
      
        if (encod.isLeft()) {
          arrowPos--;
          if (arrowPos < 0) arrowPos = 0;  // ограничиваем позицию курсора
        }
        
      } else { // если выбран пункт меню
        
        if (encod.isRight()) {
          switch (arrowPos) {
            case 0: val_R += 5;
              break;
            case 1: val_G += 5;
              break;
            case 2: val_B += 5;
              break;
          }
        }

        if (encod.isLeft()) {
          switch (arrowPos) {
            case 0: val_R -= 5;
              break;
            case 1: val_G -= 5;
              break;
            case 2: val_B -= 5;
              break;
          }
        }
        
      }

      // применяем на ленту
//      analogWrite(TEST_LED, val_R); // дааа а энкодер то проседает ........
      
      print_OLED_Color();
      
//      Serial.println(arrowPos);
    }

    
    
//    if (encod.isLeft() or up.isClick()) {  
//      if (GoSetParams) {
////        incr(val_R, -1, 255); // интересно сработает ли ???? -0-
//        val_R = constrain(val_R - 1, 0, 255); // Двигаем указатель вниз       
//        Serial.println(val_R);         
//      } else {
//        pointer = constrain(pointer - 1, 0, 3-1); // Двигаем указатель вверх
//        Serial.println(pointer);
//      }
//      
//    }
//
//    if (encod.isRight() or down.isClick()) { 
//      if (GoSetParams) {
////        incr(val_R, 1, 255); // интересно сработает ли ???? -0-
//          val_R = constrain(val_R + 1, 0, 255); // Двигаем указатель вниз     
//        Serial.println(val_R);  
//      } else {
//        pointer = constrain(pointer + 1, 0, 3 - 1); // Двигаем указатель вниз
//        Serial.println(pointer);            
//      }
//    }

    
    
    
  
    // а как сделать так, чтоб всё обновлялось только после прихода нового, это из-за этого всё тормозит??????
//    oled.update();
//    if (происходят изменеья) {
//      oled.update();
//    }
//    if (encod.isTurn()) { // не очень хорошая идея :(
//      oled.update();
//    }

//    if (start_menu) {
//      oled.update();
//      start_menu = false;
////      Serial.println("Я запускаюсь 1 раз");
//    }
////     /// НЕ ЗНАЮ КАК ЭТО ПОТИМИЗИРОВАТЬ НО ОООООООЧЕНЬ ХОСУ УЗНАТЬ ДА :)))))))))))))
//    if (pointer_last != pointer) { // изменение мб есть ещё варианты --
////      Serial.println("Изменение");
//      pointer_last = pointer;
//      oled.update();
//    }
//    oled.update();
    
    
    
    
    
    
  }
}



*/
