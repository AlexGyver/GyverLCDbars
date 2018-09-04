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
}

void loop() {
  // для примера потенциометр подключен к A0
  int perc = map(analogRead(0), 0, 1023, 0, 100);

  //fillBar0 принимает аргументы (столбец, строка, длина полосы, значение в % (0 - 100) )
  fillBar0(0, 0, 10, perc);
  fillBar0(0, 1, 16, perc);
  delay(50);
}

void fillBar0(byte start_pos, byte row, byte bar_length, byte fill_percent) {
  byte infill = round((float)bar_length * fill_percent / 100);
  lcd.setCursor(start_pos, row);
  if (infill == 0) lcd.write(16);
  else lcd.write(255);
  for (int n = 1; n < bar_length - 1; n++) {
    if (n < infill) lcd.write(255);
    if (n >= infill) lcd.write(16);
  }
  if (infill == bar_length) lcd.write(255);
  else lcd.write(16);
}
