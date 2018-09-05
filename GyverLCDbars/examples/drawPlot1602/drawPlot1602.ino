/*
  Скетч примера графика на символьном LCD дисплее 1602/2004 итд.
*/

// библиотеки дисплея
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// создаём дисплей
// дисплей 1602. Если не работает, используйте другой адрес
//LiquidCrystal_I2C lcd(0x3f, 16, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// дисплей 2004. Если не работает, используйте другой адрес!
//LiquidCrystal_I2C lcd(0x3f, 20, 4);
//LiquidCrystal_I2C lcd(0x27, 20, 4);

int plot_array[20];         // массив данных для графика

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // инициализация символов для отрисовки. При использовании каких то своих символов,
  // нужно вызывать эту функцию перед отрисовкой графика/полосы!
  initPlot();
}

void loop() {
  // для примера потенциометр подключен к A0
  int value = map(analogRead(0), 0, 1023, 10, 80);

  // drawPlot принимает аргументы (столбец, строка, ширина, высота, мин. значение, макс. значение, величина)
  // строка, столбец: начало графика - нижняя левая точка!
  // значения целочисленные (int) и могут быть отрицательными!
  // для примера отрисовка графика 12х4 занимает 140 миллисекунд
  // график сдвигается на 1 шаг АВТОМАТИЧЕСКИ при вызове функции!
  drawPlot(0, 1, 16, 2, 10, 80, value);

  // также можно вывести минимум и максимум по отображаемому участку графика
/*
  int max_value = -32000;
  int min_value = 32000;
  for (byte i = 0; i < 12; i++) {
    if (plot_array[i] > max_value) max_value = plot_array[i];
    if (plot_array[i] < min_value) min_value = plot_array[i];
  }

  lcd.setCursor(17, 1);
  lcd.print(value); lcd.print(" ");
  lcd.setCursor(13, 0);
  lcd.print("max "); lcd.print(max_value); lcd.print(" ");
  lcd.setCursor(13, 3);
  lcd.print("min "); lcd.print(min_value); lcd.print(" ");
*/
  delay(300);
}

void initPlot() {
  // необходимые символы для работы
  // создано в http://maxpromer.github.io/LCD-Character-Creator/
  byte row8[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row7[8] = {0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row6[8] = {0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row5[8] = {0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row4[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row3[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};
  byte row2[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111};
  byte row1[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};
  lcd.createChar(0, row8);
  lcd.createChar(1, row1);
  lcd.createChar(2, row2);
  lcd.createChar(3, row3);
  lcd.createChar(4, row4);
  lcd.createChar(5, row5);
  lcd.createChar(6, row6);
  lcd.createChar(7, row7);
}

void drawPlot(byte pos, byte row, byte width, byte height, int min_val, int max_val, int fill_val) {
  for (byte i = 0; i < width; i++) {
    plot_array[i] = plot_array[i + 1];
  }
  fill_val = constrain(fill_val, min_val, max_val);
  plot_array[width] = fill_val;

  for (byte i = 0; i < width; i++) {                  // каждый столбец параметров
    byte infill, fract;
    // найти количество целых блоков с учётом минимума и максимума для отображения на графике
    infill = floor((float)(plot_array[i] - min_val) / (max_val - min_val) * height * 10);
    fract = (infill % 10) * 8 / 10;                   // найти количество оставшихся полосок
    infill = infill / 10;

    for (byte n = 0; n < height; n++) {     // для всех строк графика
      if (n < infill && infill > 0) {       // пока мы ниже уровня
        lcd.setCursor(i, (row - n));        // заполняем полными ячейками
        lcd.write(0);
      }
      if (n >= infill) {                    // если достигли уровня
        lcd.setCursor(i, (row - n));
        if (fract > 0) lcd.write(fract);          // заполняем дробные ячейки
        else lcd.write(16);                       // если дробные == 0, заливаем пустой
        for (byte k = n + 1; k < height; k++) {   // всё что сверху заливаем пустыми
          lcd.setCursor(i, (row - k));
          lcd.write(16);
        }
        break;
      }
    }
  }
}
