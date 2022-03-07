

#include <GRGB.h>
GRGB led(COMMON_CATHODE, 9, 10, 11);


void setup() {
  led.setRGB(0, 0, 0);
  Serial.begin(9600);
}

void loop() {
//  int H = map(analogRead(0), 0, 1023, 0, 255);
//  Serial.println(H);
  
  led.setBrightness(125); // установить ОБЩУЮ яркость

  
  led.setCRT(true); // включить гамма-коррекцию (по умолч. выключено)
  //Что это такое??? 
  
  //led.setRGB(123, 2, 30); //led.setRGB(255, 0, 0, 120); // + "локальная" яркость [0-255]
  //led.setWheel8(H); //led.setWheel8(120, 255); // + "локальная" яркость [0-255]
  //led.setHSV(123, 200, 250); // установить цвет HSV (цвет, насыщенность, яркость) [0-255]
  //led.setHSVfast(H, 200, 250); // установить цвет HSV быстрый алгоритм (цвет, насыщенность, яркость) [0-255]
  // установить цвет из цветового колеса [0-1530]
  //led.setWheel(H); //led.setWheel(800, 120); // + "локальная" яркость [0-255]
  // установить цветовую температуру [1000-40000] Кельвинов
  //led.setKelvin(H); //led.setKelvin(5500, 255); // + "локальная" яркость [0-255]
  // установить HEX цвет (24 бита)
  //led.setHEX(0x00FF00); //led.setHEX(0x00FF00, 120); // + "локальная" яркость [0-255]
  // установить HEX цвет (16 бит)
//  led.setHEX16(0xF81F); //led.setHEX16(0xF81F, 120); // + "локальная" яркость [0-255]
  
  // плавно мигаем яркостью
  static uint32_t tmr;
  if (millis() - tmr >= 50) {
    tmr = millis();
    static int8_t dir = 1;
    static int val = 0;
    val += dir;
    if (val == 255 || val == 0) dir = -dir;   // разворачиваем
    led.setWheel8(val);
  }
  
}
