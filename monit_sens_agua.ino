#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa display I2C no endereço 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Defini os pinos como sensores
#define Sensor1 13 /* Define sensor1 como pino 16 */
#define Sensor2 12 /* Define sensor2 como pino 2 */
#define Sensor3 14 /* Define sensor3 como pino 4 */
#define Sensor4 27 /* Define sensor2 como pino 15 */

int sensor1 = 13; /* Alto */
int sensor2 = 12; /* Meio alto */
int sensor3 = 14; /* Meio baixo */
int sensor4 = 27; /* Baixo */

// int estado_inicial = 0;

void setup() {  
  // Serial.begin(115200);
  Serial.begin(9600);

  // Define pinos como entradas
  pinMode(Sensor1, INPUT);
  pinMode(Sensor2, INPUT);
  pinMode(Sensor3, INPUT);
  pinMode(Sensor4, INPUT);

  // lcd.begin();
  lcd.init();
  // lcd.backlight();
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Monitor init...");
  delay(10000);
  lcd.clear();
}

void loop() {
  int estado_sensor1 = digitalRead(sensor1);
  int estado_sensor2 = digitalRead(sensor2);
  int estado_sensor3 = digitalRead(sensor3);
  int estado_sensor4 = digitalRead(sensor4);

  // Serial.print("Estado do sensor 1: ");
  // Serial.println(estado_sensor1);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sensor 1:");
  lcd.setCursor(0,1);
  lcd.print("Estado: ");
  lcd.setCursor(11,1);
  lcd.print(estado_sensor1);
  delay(3000);

  // Serial.print("Estado do sensor 2: ");
  // Serial.println(estado_sensor2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sensor 2:");
  lcd.setCursor(0,1);
  lcd.print("Estado: ");
  lcd.setCursor(11,1);
  lcd.print(estado_sensor2);
  delay(3000);
  
  // Serial.print("Estado do sensor 3: ");
  // Serial.println(estado_sensor3);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sensor 3:");
  lcd.setCursor(0,1);
  lcd.print("Estado: ");
  lcd.setCursor(11,1);
  lcd.print(estado_sensor3);
  delay(3000);
  
  // Serial.print("Estado do sensor 4: ");
  // Serial.println(estado_sensor4);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sensor 4:");
  lcd.setCursor(0,1);
  lcd.print("Estado: ");
  lcd.setCursor(11,1);
  lcd.print(estado_sensor4);
  delay(3000);

  // Determina o nível com base nos sensores ativados
  String niveldeagua_monitor;
  String niveldeagua_lcd;

  if (estado_sensor1 == HIGH) {
    niveldeagua_monitor = "Reservatorio CHEIO!";
    niveldeagua_lcd = "CHEIO 100%!";
  }
 
  else if (estado_sensor2 == LOW) {
    niveldeagua_monitor = "Nivel em 75% - Meio cheio!";
    niveldeagua_lcd = "ENTRE 75 E 99%";    
  }

  else if (estado_sensor3 == LOW) {
    niveldeagua_monitor = "Nivel em 50% - Meio!";
    niveldeagua_lcd = "ENTRE 50 E 74%";
  }

  else if (estado_sensor4 == HIGH) {
    niveldeagua_monitor = "Nivel em 25% - Baixo!";
    niveldeagua_lcd = "ENTRE 25 E 49%";
  }

  else {
    niveldeagua_monitor = "Nivel abaixo de 25% - Critico!";
    niveldeagua_lcd = "BAIXO - CRITICO!";
  }

  // Exibir resultado no Display
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Reservatorio:");
  lcd.setCursor(0,1);
  lcd.print(niveldeagua_lcd);

  // Exibir resultado via Monitor Serial
  Serial.println("Estado do sensor 1: ");
  Serial.print(estado_sensor1);
  Serial.println("Estado do sensor 2: ");
  Serial.print(estado_sensor2);
  Serial.println("Estado do sensor 3: ");
  Serial.print(estado_sensor3);
  Serial.println("Estado do sensor 4: ");
  Serial.print(estado_sensor4);
  Serial.println("----------------------");
  Serial.println("Nivel do reservatorio: ");
  Serial.print(niveldeagua_monitor);
  
  delay(5000);
}
