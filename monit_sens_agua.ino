// #include <WiFi.h>
// #include <HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configuração da rede wifi
const char* ssid = "SSID da rede / nome da rede wifi";
const char* password = "senha da rede wifi";
// Endereço do servidor
const char* servidor_http = "http://192.168.0.0:5000/receber"; // ip do servidor que recebera as noticificações

// Inicializa display I2C no endereço 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Defini pinos dos sensores - válidos para ESP12N (ESP8266)
const int sensor1Pin = 13; // Alto       -  GPIO12 - D7
const int sensor2Pin = 12; // Medio alto -  GPIO13 - D6
const int sensor3Pin = 14; // Meio baixo -  GPIO14 - D5
const int sensor4Pin = 16; // Baixo      -  GPIO16 - D0

// int estado_inicial = 0;
String ultimo_nivel = "";

void setup() {  
  Serial.begin(115200);
  // Serial.begin(9600);

  // Define pinos como entradas
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  pinMode(sensor4Pin, INPUT);

  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  // lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Monitor init...");
  delay(2000);
  
  // Conecta ao wifi
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectando WiFi");
  Serial.print("Conectando ao WiFi")

  WiFi.begin(ssid, password);
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 10) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  lcd.clear();
  if (WiFi.status() == WL_CONNECTED) {
    lcd.setCursor(0,0);
    lcd.print("WiFi Conectado");
    Serial.println("\nConectado ao WiFi");
    Serial.print("IP -> ");
    Serial.println(WiFi.localIP());
  }
  else {
    lcd.setCursor(0,0);
    lcd.print("WiFi Conect erro");
    Serial.println("\nFalha ao Conectar no WiFi");
  }

  delay(2000);
}

void loop() {
  int estado_sensor1 = digitalRead(sensor1Pin);
  int estado_sensor2 = digitalRead(sensor2Pin);
  int estado_sensor3 = digitalRead(sensor3Pin);
  int estado_sensor4 = digitalRead(sensor4Pin);

  // Serial.print("Estado do sensor 1: ");
  // Serial.println(estado_sensor1);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sensor 1:");
  lcd.setCursor(0,1);
  lcd.print("Estado: ");
  lcd.setCursor(11,1);
  lcd.print(estado_sensor1);
  delay(2000);

  // Serial.print("Estado do sensor 2: ");
  // Serial.println(estado_sensor2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sensor 2:");
  lcd.setCursor(0,1);
  lcd.print("Estado: ");
  lcd.setCursor(11,1);
  lcd.print(estado_sensor2);
  delay(2000);
  
  // Serial.print("Estado do sensor 3: ");
  // Serial.println(estado_sensor3);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sensor 3:");
  lcd.setCursor(0,1);
  lcd.print("Estado: ");
  lcd.setCursor(11,1);
  lcd.print(estado_sensor3);
  delay(2000);
  
  // Serial.print("Estado do sensor 4: ");
  // Serial.println(estado_sensor4);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sensor 4:");
  lcd.setCursor(0,1);
  lcd.print("Estado: ");
  lcd.setCursor(11,1);
  lcd.print(estado_sensor4);
  delay(2000);

  // Determina o nível com base nos sensores ativados
  String niveldeagua_monitor;
  String niveldeagua_lcd;

  if (estado_sensor1 == HIGH) {
    niveldeagua_monitor = "Reservatorio CHEIO!";
    niveldeagua_lcd = "CHEIO 100%!";
  }
 
  else if (estado_sensor2 == LOW) {
    niveldeagua_monitor = "Nivel em 75% - Meio cheio!";
    niveldeagua_lcd = "ENTRE 75-99%";    
  }

  else if (estado_sensor3 == LOW) {
    niveldeagua_monitor = "Nivel em 50% - Meio!";
    niveldeagua_lcd = "ENTRE 50-74%";
  }

  else if (estado_sensor4 == HIGH) {
    niveldeagua_monitor = "Nivel em 25% - Baixo!";
    niveldeagua_lcd = "ENTRE 25-49%";
  }

  else {
    niveldeagua_monitor = "Nivel abaixo de 25% - Critico!";
    niveldeagua_lcd = "CRITICO < 25% !";
  }

  // Exibir resultado no Display
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Reservatorio:");
  lcd.setCursor(0,1);
  lcd.print(niveldeagua_lcd);

  // Exibir resultado via Monitor Serial
  Serial.println("---#---#---#---#---#--");
  Serial.print("Estado do sensor 1: "); Serial.println(estado_sensor1);
  Serial.print("Estado do sensor 2: "); Serial.println(estado_sensor2);
  Serial.print("Estado do sensor 3: "); Serial.println(estado_sensor3);
  Serial.print("Estado do sensor 4: "); Serial.println(estado_sensor4);
  Serial.println("----------------------");
  Serial.println("Nivel do reservatorio: " + niveldeagua_monitor);
  Serial.println("----------------------");
  // delay(1000);

  // Envio do alerta caso ocorra mudança de nivel
  if (niveldeagua_monitor != ultimo_nivel && WiFi.status() == WL_CONNECTED) {
    String url = servidor_http;
    url += "?nivel=" + String(niveldeagua_monitor);

    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.println("Enviano alerta: " + url);
      Serial.println("Resposta do servidor: " + http.getString());
      Serial.println("Código HTTP: " + httpCode);
    }
    else {
      Serial.println("Falha na requisição HTTP");
      Serial.println("Código HTTP: " + httpCode);
    }

    http.end();
    ultimo_nivel = niveldeagua_monitor;
  }

  delay(10000);
}
