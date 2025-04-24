#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // inicialização LCD
  lcd.init();
  // Liga a luz de fundo do LCD
  lcd.backlight();
}

void loop() {
  // setar o cursor na primeira linha e primeira coluna
  lcd.setCursor(0, 0);
  // imprimir mensagem no LCD
  lcd.print("Já deu bom...");
  delay(5000);

}
