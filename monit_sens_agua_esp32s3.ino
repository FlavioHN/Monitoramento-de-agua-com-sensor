#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa display I2C no endereço 0x3F
// SDA=GPIO8, SDL=GPIO9
LiquidCrystal_I2C lcd(0x3F, 20, 4);

// Configuração da rede wifi
const char* ssid = "rede-wifi";
const char* password = "senha-wifi";
// ThingSpeak
const char* tsapi = "api.thingspeak.com";
String apikey = "TNKNT83DB64U1Q21";
// Endereço do servidor
const char* servidor_url = "http://192.168.0.4:5050/receber1";

// Defini pinos dos sensores - válidos para ESP32S3 mini
const int sensor4Pin = 6; // Alto        -  GPIO6
const int sensor3Pin = 5; // Medio alto  -  GPIO5
const int sensor2Pin = 4; // Medio baixo -  GPIO4
const int sensor1Pin = 3; // Baixo       -  GPIO3

// int estado_inicial = 0;
String ultimo_nivel = "";
int ultimo_sensor;

void setup() {  
  Serial.begin(115200);

  // Define pinos como entradas
  pinMode(sensor4Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor1Pin, INPUT);

  Wire.begin(8, 9); // SDA = GPIO 8, SCL = GPIO 9
  //lcd.begin(20,4);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Monitoramento init...");
  Serial.print("Iniciando monitoramento..");
  delay(3000);
  
  // Conecta ao wifi
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 10) {
    delay(500);
    lcd.clear();
    lcd.setCursor(3, 1);
    lcd.print("Conectando Wifi");
    lcd.setCursor(5, 2);
    lcd.print("...");
    Serial.print(".");
    tentativas++;
  }

  lcd.clear();
  if (WiFi.status() == WL_CONNECTED) {
    lcd.setCursor(3,1);
    lcd.print("WiFi Conectado");
    lcd.setCursor(0,1);
    lcd.print("IP-> ");
    lcd.setCursor(5,1);
    lcd.println(WiFi.localIP());
    Serial.println("Conectado ao WiFi");
    Serial.print("IP -> ");
    Serial.println(WiFi.localIP());
    delay(3000);
  } 
  else {
    lcd.setCursor(3,0);
    lcd.print("WiFi Conect erro");
    Serial.println("Falha ao Conectar no WiFi");
  }
  delay(2000);
}

void loop() {
  int estado_sensor4 = digitalRead(sensor4Pin);
  int estado_sensor3 = digitalRead(sensor3Pin);
  int estado_sensor2 = digitalRead(sensor2Pin);
  int estado_sensor1 = digitalRead(sensor1Pin);

  lcd.clear();
  // Sensor 4
  lcd.setCursor(0,0);
  lcd.print("Est. Sensor4 = ");
  lcd.setCursor(15,0);
  lcd.print(estado_sensor4);
  // Sensor 3
  lcd.setCursor(0,1);
  lcd.print("Est. Sensor3 = ");
  lcd.setCursor(15,1);
  lcd.print(estado_sensor3);
  // Sensor 2
  lcd.setCursor(0,2);
  lcd.print("Est. Sensor2 = ");
  lcd.setCursor(15,2);
  lcd.print(estado_sensor2);
  // Sensor 1
  lcd.setCursor(0,3);
  lcd.print("Est. Sensor1 = ");
  lcd.setCursor(15,3);
  lcd.print(estado_sensor1);
  delay(10000);

  // Determina o nível com base nos sensores ativados
  String niveldeagua_monitor;
  String niveldeagua_lcd;
  int enviar_sensor;

  if (estado_sensor4 == HIGH) {
    niveldeagua_monitor = "Reservatorio CHEIO!";
    niveldeagua_lcd = "CHEIO 100%!";
    enviar_sensor = 4;
  }
  else if (estado_sensor3 == LOW) {
    niveldeagua_monitor = "Nivel em 75% - Meio cheio!";
    niveldeagua_lcd = "ENTRE 75-99%";
    enviar_sensor = 3;
  }
  else if (estado_sensor2 == LOW) {
    niveldeagua_monitor = "Nivel em 50% - Meio!";
    niveldeagua_lcd = "ENTRE 50-74%";
    enviar_sensor = 2;
  }
  else if (estado_sensor1 == HIGH) {
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
  lcd.setCursor(2,0);
  lcd.print("Nivel de agua no");
  lcd.setCursor(3,1);
  lcd.print("reservatorio");
  lcd.setCursor(3,2);
  lcd.print(niveldeagua_lcd);
  delay(5000);

  // Exibir resultado via Monitor Serial
  Serial.println("---#---#---#---#---#--");
  Serial.print("Estado do sensor 4: "); Serial.println(estado_sensor4);
  Serial.print("Estado do sensor 3: "); Serial.println(estado_sensor3);
  Serial.print("Estado do sensor 2: "); Serial.println(estado_sensor2);
  Serial.print("Estado do sensor 1: "); Serial.println(estado_sensor1);
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

      // Envio para banco de dados
      String postData = "nivel=" + String(niveldeagua_monitor);
  
      int httpCode = http.POST(postData);
      String response = http.getString();

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
  delay(30000);
}
