#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa display I2C no endereço 0x27
// SDA=GPIO8-D8, SDL=GPIO9-D9
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Configuração da rede wifi
const char* ssid = "brisa-1899810";
const char* password = "d6yn8pgr";
// ThingSpeak
const char* tsapi = "api.thingspeak.com";
String apikey = "FD8OE0UQCSN2YMBJ";
// Endereço do servidor
const char* servidor_url = "http://192.168.0.19:5060/receber";

// Defini pinos dos sensores - válidos para ESP32-C3
const int sensor2Pin = 3; // Alto - GPIO03 - D3
const int sensor1Pin = 2; // Meio - GPIO02 - D2

// int estado_inicial = 0;
String ultimo_nivel = "";
int ultimo_sensor;

void setup() {  
  Serial.begin(115200);

  // Define pinos como entradas
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor1Pin, INPUT);

  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Monitor init..");
  Serial.print("Iniciando monitoramento..");
  delay(3000);
  
  // Conecta ao wifi
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 10) {
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Conectando Wifi");
    lcd.setCursor(5, 1);
    lcd.print("...");
    Serial.print(".");
    tentativas++;
  }

  lcd.clear();
  if (WiFi.status() == WL_CONNECTED) {
    lcd.setCursor(0,0);
    lcd.print("WiFi Conectado");
    lcd.setCursor(0,1);
    lcd.print("IP:");
    lcd.setCursor(3,1);
    lcd.println(WiFi.localIP());
    Serial.println("Conectado ao WiFi");
    Serial.print("IP -> ");
    Serial.println(WiFi.localIP());
    delay(3000);
  } 
  else {
    lcd.setCursor(0,0);
    lcd.print("WiFi Conect error");
    Serial.println("Falha ao Conectar no WiFi");
  }
  delay(3000);
}

void loop() {
  int estado_sensor2 = digitalRead(sensor2Pin);
  int estado_sensor1 = digitalRead(sensor1Pin);

  lcd.clear();
  // Sensor 2
  lcd.setCursor(0,0);
  lcd.print("Est. Sensor2 = ");
  lcd.setCursor(14,0);
  lcd.print(estado_sensor2);
  // Sensor 1
  lcd.setCursor(0,1);
  lcd.print("Est. Sensor1 = ");
  lcd.setCursor(14,1);
  lcd.print(estado_sensor1);
  delay(5000);

  // Determina o nível com base nos sensores ativados
  String niveldeagua_monitor;
  String niveldeagua_lcd;
  int enviar_sensor;

  if (estado_sensor2 == HIGH) {
    niveldeagua_monitor = "Nivel em 100% - Cheio!";
    niveldeagua_lcd = "CHEIO 100%";
    enviar_sensor = 2;
  }
  else if (estado_sensor1 == HIGH) {
    niveldeagua_monitor = "Nivel em 50% - Meio!";
    niveldeagua_lcd = "ENTRE 50-99%";
    enviar_sensor = 1;
  }
  else {
    niveldeagua_monitor = "Nivel abaixo de 50% - Baixo!";
    niveldeagua_lcd = "ABAIXO DA METADE";
    enviar_sensor = 0;
  }

  // Exibir resultado no Display
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Reservatorio:");
  lcd.setCursor(0,1);
  lcd.print(niveldeagua_lcd);
  delay(5000);

  // Exibir resultado via Monitor Serial
  Serial.println("---#---#---#---#---#--");
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
