



void printBit() {
  
  // x, y, имя, ширина, высота, BITMAP_NORMAL(0)/BITMAP_INVERT(1), BUF_ADD/BUF_SUBTRACT/BUF_REPLACE
//  oled.drawBitmap(1, 20, testBit7x7, 4, 7, BITMAP_NORMAL, BUF_ADD);

//  oled.drawBitmap(30, 20, testBit8x8, 9, 10, BITMAP_NORMAL, BUF_ADD);

  oled.rect(0, 16, 47, 63, OLED_STROKE); // рамка
  oled.rect(2, 18, 45, 61); // битмап

  oled.rect(79, 16, 127, 63, OLED_STROKE); // рамка
  oled.rect(81, 18, 125, 61); // битмап
  
  oled.update(); 

  
}



void printPointer(uint8_t pointer) {
//  oled.setCursor(0, pointer);
//  oled.drawBitmap(0, pointer * 8, ptr_bmp, 10, 8);
  // Указатель в виде прямоугольной рамки со скругленными углами
  if (pointer == 0) {
//    oled.roundRect(0, 22, 33, 51, OLED_STROKE);
    oled.rect(0 + 11, 16, 47 + 11, 63, OLED_STROKE); // рамка
  } else if (pointer == 1) {
    oled.rect(79 - 11, 16, 127 - 11, 63, OLED_STROKE); // рамка
//    oled.roundRect(34, 22, 67, 51, OLED_STROKE);
  } 
//  else if (pointer == 2) {
//    oled.roundRect(68, 22, 101, 51, OLED_STROKE);
//  }
  
}
