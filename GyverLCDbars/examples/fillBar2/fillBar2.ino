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

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // инициализация символов для отрисовки. При использовании каких то своих символов,
  // нужно вызывать эту функцию перед отрисовкой графика/полосы!
  initBar2();
}

void loop() {
  // для примера потенциометр подключен к A0
  int perc = map(analogRead(0), 0, 1023, 0, 100);

  //fillBar2 принимает аргументы (столбец, строка, длина полосы, значение в % (0 - 100) )
  fillBar2(0, 0, 10, perc);
  fillBar2(0, 1, 16, perc);
  delay(50);
}

void initBar2() {
  // необходимые символы для работы
  // создано в http://maxpromer.github.io/LCD-Character-Creator/
  byte right_empty[8] = {0b11111,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b11111};
  byte left_empty[8] = {0b11111,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b11111};
  byte center_empty[8] = {0b11111, 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};
  byte left_full[8] = {0b11111, 0b10000, 0b10111, 0b10111, 0b10111, 0b10111, 0b10000, 0b11111};
  byte right_full[8] = {0b11111, 0b00001, 0b11101, 0b11101, 0b11101, 0b11101, 0b00001, 0b11111};
  byte center_full[8] = {0b11111, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000, 0b11111};
  lcd.createChar(0, left_empty);
  lcd.createChar(1, center_empty);
  lcd.createChar(2, right_empty);
  lcd.createChar(3, left_full);
  lcd.createChar(4, center_full);
  lcd.createChar(5, right_full);
}

void fillBar2(byte start_pos, byte row, byte bar_length, byte fill_percent) {
  byte infill = round((float)bar_length * fill_percent / 100);
  lcd.setCursor(start_pos, row);
  if (infill == 0) lcd.write(0);
  else lcd.write(3);
  for (int n = 1; n < bar_length - 1; n++) {
    if (n < infill) lcd.write(4);
    if (n >= infill) lcd.write(1);
  }
  if (infill == bar_length) lcd.write(5);
  else lcd.write(2);
}
