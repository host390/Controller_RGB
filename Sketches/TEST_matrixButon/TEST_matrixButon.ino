


#include <Keypad.h> // подключаем библиотеку для управление клавиатурой

const byte ROWS = 4; //число строк у нашей клавиатуры
const byte COLS = 4; //число столбцов у нашей клавиатуры

char hexaKeys[ROWS][COLS] = {// здесь мы располагаем названия наших клавиш, как на клавиатуре, для удобства пользования
  {'0','4','8','C'}, 
  {'1','5','9','D'},
  {'2','6','A','F'},
  {'3','7','B','G'}
};

byte rowPins[ROWS] = {8, 7, 6, 5}; //к каким выводам подключаем управление строками
byte colPins[COLS] = {A0, A1, A2, A3}; //к каким выводам подключаем управление столбцами

//передаем все эти данные библиотеке:
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup(){
  Serial.begin(9600); //запускаем отладочный вывод в монитор порта
}

bool butClick = false; // Кнопка нажата
byte butActiv; // Какая кнопка


void loop(){
  char customKey = customKeypad.getKey(); //записывем нажатый символ
  
  if (customKey) { //если что-то нажато
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
   Serial.print("Нажата Кнопка: "); Serial.println(butActiv);
   
//   Serial.print("Нажато: "); Serial.println(customKey);
//   Serial.print("butActiv: "); Serial.println(butActiv);
   
  }

  
}
