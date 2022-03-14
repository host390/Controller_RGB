/*
   Пример управления RGB светодиодом из набора готовых цветов
   Светодиод/мосфеты подключены к ШИМ пинам!
   Для УНО и НАНО это 3, 5, 6, 9, 10, 11
    _____
   /     \
  |       |
  |       |
  |_______|
   | | | |
   | | | |
   | | | |
     | | |
       |
   B G   R
*/


#define PIN_R 10
#define PIN_G 9
#define PIN_B 3

#include <GyverRGB.h>
GRGB diode(PIN_R, PIN_G, PIN_B);  // куда подключены цвета (R, G, B)

void setup() {

}

void loop() {
  diode.setHEX(WHITE);
  delay(3000);
  diode.setHEX(SILVER);
  delay(3000);
  diode.setHEX(GRAY);
  delay(3000);
  diode.setHEX(BLACK);
  delay(3000);
  diode.setHEX(RED);
  delay(3000);
  diode.setHEX(MAROON);
  delay(3000);
  diode.setHEX(YELLOW);
  delay(3000);
  diode.setHEX(OLIVE);
  delay(3000);
  diode.setHEX(LIME);
  delay(3000);
  diode.setHEX(GREEN);
  delay(3000);
  diode.setHEX(AQUA);
  delay(3000);
  diode.setHEX(TEAL);
  delay(3000);  
  diode.setHEX(BLUE);
  delay(3000);  
  diode.setHEX(NAVY);
  delay(3000);
  diode.setHEX(PINK);
  delay(3000);  
  diode.setHEX(PURPLE);
  delay(3000);  
}
