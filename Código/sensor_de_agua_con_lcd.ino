#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int nivel=0;
int lvl=0;
void setup()
{
 lcd.begin(16, 2);
 lcd.setCursor(0,0);
 lcd.print("capacidad:");
 lcd.setCursor(12,0);
 lcd.print("%");
}

void loop()
{
 nivel=analogRead(A0);
 lvl=map(nivel,0,500,0,100);
 lcd.setCursor(10,0);
 lcd.print(lvl);
}
/*d4-5
 * d5-4
 * d6-3
 * d7-2
 * a-+
 * k-gnd
 * E-11
 * rw-gnd
 * rs-12
 * v0-potenciometro
 * vdd-+
 * vss--
 */
