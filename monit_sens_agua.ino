#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define Sensor1 5 /* Define sensor1 como pino 5 */
#define Sensor2 6 /* Define sensor2 como pino 6 */
#define Sensor3 7 /* Define sensor3 como pino 7 */
#define Sensor4 8 /* Define sensor2 como pino 8 */

int sensor1 = 5; /* Alto */
int sensor2 = 6; /* Meio alto */
int sensor3 = 7; /* Meio baixo */
int sensor4 = 8; /* Baixo */

/* int estado_inicial = 0; */

void setup() {  
  Serial.begin(9600);
  
  pinMode(Sensor1, INPUT); /* Define o pino do sensor como entrada */
  pinMode(Sensor2, INPUT); /* Define o pino do sensor como entrada */
  pinMode(Sensor3, INPUT); /* Define o pino do sensor como entrada */
  pinMode(Sensor4, INPUT); /* Define o pino do sensor como entrada */

  lcd.init();
  lcd.setBacklight(HIGH);
}

void loop() {
  int estado_sensor1 = digitalRead(sensor1);
  int estado_sensor2 = digitalRead(sensor2);
  int estado_sensor3 = digitalRead(sensor3);
  int estado_sensor4 = digitalRead(sensor4);

  Serial.print("Estado do sensor 1: ");
  Serial.println(estado_sensor1);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Est do sensor 1: ");
  lcd.setCursor(0,1);
  lcd.print("Estado:" estado_sensor1);
  delay(3000);

  Serial.print("Estado do sensor 2: ");
  Serial.println(estado_sensor2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Est do sensor 2: ");
  lcd.setCursor(0,1);
  lcd.print("Estado:" estado_sensor2);
  delay(3000);
  
  Serial.print("Estado do sensor 3: ");
  Serial.println(estado_sensor3);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Est do sensor 3: ");
  lcd.setCursor(0,1);
  lcd.print("Estado:" estado_sensor3);
  delay(3000);
  
  Serial.print("Estado do sensor 4: ");
  Serial.println(estado_sensor4);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Est do sensor 4: ");
  lcd.setCursor(0,1);
  lcd.print("Estado:" estado_sensor4);
  delay(3000);

  if ((estado_sensor1 == 1) && (estado_sensor2 == 1) && (estado_sensor3 == 1) && (estado_sensor4 == 1)) {
    Serial.println("Reservatorio Cheio!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reservatorio:");
    lcd.setCursor(3,1);
    lcd.print("CHEIO!");
  }
 
  else if ((estado_sensor1 == 0) && (estado_sensor2 == 1) && (estado_sensor3 == 1) && (estado_sensor4 == 1)) {
    Serial.println("Nivel de 75 a 100% - Meio cheio!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reservatorio:");
    lcd.setCursor(0,1);
    lcd.print("ENTRE 75 E 100%");    
  }

  else if ((estado_sensor1 == 0) && (estado_sensor2 == 0) && (estado_sensor3 == 1) && (estado_sensor4 == 1)) {
    Serial.println("Nivel de 50% a 75% - Meio!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reservatorio:");
    lcd.setCursor(0,1);
    lcd.print("ENTRE 50 E 75%");
  }

  else if ((estado_sensor1 == 0) && (estado_sensor2 == 0) && (estado_sensor3 == 0) && (estado_sensor4 == 1)) {
    Serial.println("Nivel em 25% e 50% - Meio baixo!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reservatorio:");
    lcd.setCursor(0,1);
    lcd.print("ENTRE 25 E 50%");
  }

  else if ((estado_sensor1 == 0) && (estado_sensor2 == 0) && (estado_sensor3 == 0) && (estado_sensor4 == 0)) {
    Serial.println("Nivel abaixo de 25% - Critico!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reservatorio:");
    lcd.setCursor(0,1);
    lcd.print("BAIXO - CRITICO!");
  }

  else {
    Serial.println("ERRO - Verificar sensor!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ERRO DE LEITURA");
    lcd.setCursor(0,1);
    lcd.print("Verificar sensor");
  }
  delay(5000);
}