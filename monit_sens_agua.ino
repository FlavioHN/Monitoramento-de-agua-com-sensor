// #include <WiFi.h>
// #include <HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configuração da rede wifi
const char* ssid = "REDE WI-FI";
const char* password = "SENHA DA REDE WI-FI";
// ThingSpeak
const char* tsapi = "api.thingspeak.com";
String apikey = "TNKNT83DB64U1Q21";
// Endereço do servidor
const char* servidor_url = "http://192.168.0.19:5000/receber";
// Inicializa display I2C no endereço 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Defini pinos dos sensores - válidos para ESP12N (ESP8266)
const int sensor1Pin = 13; // Alto       -  GPIO12 - D7
const int sensor2Pin = 12; // Medio alto -  GPIO13 - D6
const int sensor3Pin = 14; // Meio baixo -  GPIO14 - D5
const int sensor4Pin = 16; // Baixo      -  GPIO16 - D0

// int estado_inicial = 0;
String ultimo_nivel = "";
int ultimo_sensor;

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
  Serial.print("Iniciando monitoramento..");
  delay(2000);
  
  // Conecta ao wifi
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 10) {
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Conectando Wifi");
    lcd.setCursor(0, 1);
    lcd.print("...");
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
    delay(2000);
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
  int enviar_sensor;

  if (estado_sensor1 == HIGH) {
    niveldeagua_monitor = "Reservatorio CHEIO!";
    niveldeagua_lcd = "CHEIO 100%!";
    enviar_sensor = 4;
  }
 
  else if (estado_sensor2 == LOW) {
    niveldeagua_monitor = "Nivel em 75% - Meio cheio!";
    niveldeagua_lcd = "ENTRE 75-99%";
    enviar_sensor = 3;
  }

  else if (estado_sensor3 == LOW) {
    niveldeagua_monitor = "Nivel em 50% - Meio!";
    niveldeagua_lcd = "ENTRE 50-74%";
    enviar_sensor = 2;
  }

  else if (estado_sensor4 == HIGH) {
    niveldeagua_monitor = "Nivel em 25% - Baixo!";
    niveldeagua_lcd = "ENTRE 25-49%";
    enviar_sensor = 1;
  }

  else {
    niveldeagua_monitor = "Nivel abaixo de 25% - Critico!";
    niveldeagua_lcd = "CRITICO < 25% !";
    enviar_sensor = 0;
  }

  // Exibir resultado no Display
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Reservatorio:");
  lcd.setCursor(0,1);
  lcd.print(niveldeagua_lcd);
  delay(3000);

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
  if (niveldeagua_monitor != ultimo_nivel) { 
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      http.begin(client, servidor_url);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // String url = servidor_http;
      // url += "?nivel=" + String(niveldeagua_monitor);
      //String url = "http://192.168.1.100:5000/receber?nivel=" + String(niveldeagua_monitor);

      // Envio para banco de dados
      String postData = "nivel=" + String(niveldeagua_monitor);
  
      int httpCode = http.POST(postData);
      String response = http.getString();

      //Serial.println("Código:  " + String(httpCode));
      //Serial.println("Resposta: " + response);

      if (httpCode == 200) {     
        Serial.println("Enviando alerta: " + String(servidor_url));
        Serial.println("Código HTTP: " + String(httpCode));
        Serial.println("Resposta do servidor: " + response);
      }
      else {
        Serial.println("Falha na requisição HTTP");
        Serial.println("Código HTTP: " + String(httpCode));
      }
      http.end();
      ultimo_nivel = niveldeagua_monitor;
      // Fim do envio para o banco de dados
      
      // Envio para o ThingSpeak
      String requestData = "GET /update?api_key=" + apikey + 
                            "&field1=" + String(enviar_sensor) +
                            " HTTP/1.1\r\n" +
                            "Host: " + tsapi + "\r\n" +
                            "Connection: close\r\n\r\n";
      
      if (client.connect(tsapi, 80)) {
        Serial.println("Enviando dados ao ThingSpeak");
        client.print(requestData);
        Serial.println("Dados enviados");
        client.stop();
      } else {
        Serial.println("Falha na conexão com o ThingSpeak");
      }
      ultimo_sensor = enviar_sensor;
      // Fim do envio para ThingSpeak    
    } else {
      Serial.println("Wi-Fi off");
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Wi-Fi OFF");
    }
  }
  delay(20000);
}
