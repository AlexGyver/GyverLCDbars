/*
  Скетч примера полосы загрузки на символьном LCD дисплее 1602/2004 итд.
*/

// библиотеки дисплея
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// создаём дисплей
// дисплей 1602. Если не работает, используйте другой адрес
LiquidCrystal_I2C lcd(0x3f, 16, 2);
//LiquidCrystal_I2C lcd(0x27, 16, 2);

// дисплей 2004. Если не работает, используйте другой адрес!
//LiquidCrystal_I2C lcd(0x3f, 20, 4);
//LiquidCrystal_I2C lcd(0x27, 20, 4);

boolean change_flag = true;     // флаг для 3 типа полосы

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // инициализация символов для отрисовки. При использовании каких то своих символов,
  // нужно вызывать эту функцию перед отрисовкой графика/полосы!
  initBar3();
}

void loop() {
  // для примера потенциометр подключен к A0
  int perc = map(analogRead(0), 0, 1023, 0, 100);

  //fillBar3 принимает аргументы (столбец, строка, длина полосы, значение в % (0 - 100) )
  fillBar3(0, 0, 10, perc);
  fillBar3(0, 1, 16, perc);
  delay(50);
}

void initBar3() {
  // необходимые символы для работы
  // создано в http://maxpromer.github.io/LCD-Character-Creator/
  byte right_empty[8] = {0b11111,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b11111};
  byte left_empty[8] = {0b11111,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b11111};
  byte center_empty[8] = {0b11111, 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};
  byte bar2[] = {0b11111, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11111};
  byte bar3[] = {  B11111,  B11100,  B11100,  B11100,  B11100,  B11100,  B11100,  B11111};
  byte bar4[] = {  B11111,  B11110,  B11110,  B11110,  B11110,  B11110,  B11110,  B11111};
  lcd.createChar(0, left_empty);
  lcd.createChar(1, center_empty);
  lcd.createChar(2, right_empty);
  lcd.createChar(3, bar2);
  lcd.createChar(4, bar3);
  lcd.createChar(5, bar4);
}

void fillBar3(byte start_pos, byte row, byte bar_length, byte fill_percent) {
  byte infill = bar_length * fill_percent / 10;
  byte fract = infill % 10;
  infill = infill / 10;
  // change_flag - true слева, false справа
  if (infill < bar_length - 1) {
    if (!change_flag) {
      change_flag = true;
      byte bar2[] = {0b11111, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11111};
      byte bar3[] = {  B11111,  B11100,  B11100,  B11100,  B11100,  B11100,  B11100,  B11111};
      byte bar4[] = {  B11111,  B11110,  B11110,  B11110,  B11110,  B11110,  B11110,  B11111};
      lcd.createChar(3, bar2);
      lcd.createChar(4, bar3);
      lcd.createChar(5, bar4);
    }
  } else {
    if (change_flag) {
      change_flag = false;
      byte leftbar1[] = {  B11111,  B10001,  B10001,  B10001,  B10001,  B10001,  B10001,  B11111};
      byte leftbar2[] = {  B11111,  B11001,  B11001,  B11001,  B11001,  B11001,  B11001,  B11111};
      byte leftbar3[] = {  B11111,  B11101,  B11101,  B11101,  B11101,  B11101,  B11101,  B11111};
      lcd.createChar(3, leftbar1);
      lcd.createChar(4, leftbar2);
      lcd.createChar(5, leftbar3);
    }
  }
  lcd.setCursor(start_pos, row);
  if (infill == 0) {
    if (fract >= 0 && fract < 2) lcd.write(0);
    else if (fract >= 2 && fract < 4) lcd.write(0);
    else if (fract >= 4 && fract < 6) lcd.write(3);
    else if (fract >= 6 && fract < 8) lcd.write(4);
    else if (fract >= 8) lcd.write(5);
  }
  else lcd.write(255);
  for (int n = 1; n < bar_length - 1; n++) {
    if (n < infill) lcd.write(255);
    if (n == infill) {
      if (fract >= 0 && fract < 2) lcd.write(1);
      else if (fract >= 2 && fract < 4) lcd.write(0);
      else if (fract >= 4 && fract < 6) lcd.write(3);
      else if (fract >= 6 && fract < 8) lcd.write(4);
      else if (fract >= 8) lcd.write(5);
    }
    if (n > infill) lcd.write(1);
  }
  if (infill == bar_length - 1) {
    if (fract >= 0 && fract < 2) lcd.write(2);
    else if (fract >= 2 && fract < 4) lcd.write(3);
    else if (fract >= 4 && fract < 6) lcd.write(4);
    else if (fract >= 6 && fract < 8) lcd.write(5);
    else if (fract >= 8) lcd.write(255);
  }
  else if (infill == bar_length) lcd.write(255);
  else lcd.write(2);
}
