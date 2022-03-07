



int thisBrightness; // общая яркость

uint32_t effTimer; // таймер на накт эффекта
uint16_t effPeriod; // сколько раз в микросекунду

int thisColor; // цвета для вывода в эффекте

boolean strobeFlag; // чтоб стробоскоп включался и выключался


byte policeState = 0;



void effectsTick() {
  if (activeNumb < 5) { // если статические режимы
    
    switch (activeNumb) {
      case 0: // RGB
        strip.setRGB(vals_colors[0], vals_colors[1], vals_colors[2]);
        thisBrightness = vals_colors[3];
        break;  
      case 1: // HSV
        strip.setHSV(vals_colors[4], vals_colors[5], vals_colors[6]);
        thisBrightness = 255;
        break;
      case 2: // FAST_HSV
        strip.setHSV_fast(vals_colors[7], vals_colors[8], vals_colors[9]);
        thisBrightness = 255;
        break;
      case 3: // colorWheel
        strip.colorWheel(vals_colors[10]);
        thisBrightness = vals_colors[11];
        break;
      case 4: // HEX (бит - ???)
        strip.setHEX(colorValsHEX[vals_colors[12]]);
        thisBrightness = vals_colors[13];
        break;
//      case 5: // Kelvin (В разработке)
//        strip.setKelvin(vals_colors[]);
//        thisBrightness = vals_colors[];
//        break;
    }
    
    strip.setBrightness(thisBrightness);
    
  } else { // если динамические эффекты
    if (millis() - effTimer >= effPeriod) { // по таймеру
      effTimer = millis();
      switch (activeNumb) {
        case 5: // ColorWheel 5
          effPeriod = vals_colors[14]; // 0 - 1530 скорость изменения
          thisColor += vals_colors[15]; // к цвету прибавляем шаг
          if (thisColor > 1530) thisColor = 0;
          strip.colorWheel(thisColor);
          strip.setBrightness(vals_colors[16]);
          break;
        case 6: // 6 Fire  
          // макс яркость 0-255 vals_colors[17]
          // мин яркость 0-255
          // скорость 0-1000
          effPeriod = 40;
          fireTick();
          break;
        case 7: // 7 FireManual
          // цвет 0-255
          // скорость 0-1000
          // макс яркость 0-255
          // мин яркость 0-255
          effPeriod = 40;
          fireM_tick();
          break;
        case 8: // 8 Strobe
          // Цвет 0-255
          // Насышенность 0-255
          // Яркость 0-255
          // Скорость 0-1000
          effPeriod = vals_colors[27]; // скорость вспышек
          strip.setHSV(vals_colors[24], vals_colors[25], vals_colors[26]);
          strobeFlag = !strobeFlag;
          strip.setBrightness(255 * strobeFlag);
          break;
        case 9: // 9 RndStrobe
          // Цвет 0-255
          // Насышенность 0-255
          // Яркость 0-255
          // Скорость 0-1000
          effPeriod = random(vals_colors[31]); // а как рандом работает ???
          strip.setHSV(vals_colors[28], vals_colors[29], vals_colors[30]);
          strobeFlag = !strobeFlag;
          strip.setBrightness(255 * strobeFlag);
          break;
        case 10: // 10 Police
          // Яркость 0-255
          // Скорость 0-1000
          if (++policeState >= 8) policeState = 0;
          switch (policeState) {
            case 0: strip.setRGB(0, 0, 0);
              break;
            case 1: strip.setRGB(255, 0, 0);
              break;
            case 2: strip.setRGB(0, 0, 0);
              break;
            case 3: strip.setRGB(255, 0, 0);
              break;
            case 4: strip.setRGB(0, 0, 0);
              break;
            case 5: strip.setRGB(0, 0, 255);
              break;
            case 6: strip.setRGB(0, 0, 0);
              break;
            case 7: strip.setRGB(0, 0, 255);
              break;
          }
          effPeriod = vals_colors[33];
          strip.setBrightness(vals_colors[32]);
          break;
      }
    }
  }

  
}


uint32_t prevTime;
int val, rndVal;

float SMOOTH_K = 0.25; // #define SMOOTH_K 0.25 // коэффициент плавности огня

void fireTick() {
  if (millis() - prevTime > vals_colors[19]) { // скорость 0-1000;
    rndVal = random(2, 13) * 10;
    prevTime = millis();
  }
  val = val * (1 - SMOOTH_K) + rndVal * SMOOTH_K;
  
  strip.colorWheel(val);
  
  thisBrightness = map(val, 20, 120, vals_colors[18], vals_colors[17]); // мин - 0-255, макс 0-255
  strip.setBrightness(thisBrightness);
}


void fireM_tick() {
  if (millis() - prevTime > vals_colors[21]) { // скорость 0-1000;
    rndVal = random(0, 13) * 10;
    prevTime = millis();
  }
  val = val * (1 - SMOOTH_K) + rndVal * SMOOTH_K;
  thisBrightness = map(val, 0, 120, vals_colors[23], vals_colors[22]); // мин - 0-255, макс 0-255
  strip.colorWheel(vals_colors[20] * 6); // цвет 0-255
  strip.setBrightness(thisBrightness);
}
