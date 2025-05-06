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

}

void loop() {
  int estado_sensor1 = digitalRead(sensor1);
  int estado_sensor2 = digitalRead(sensor2);
  int estado_sensor3 = digitalRead(sensor3);
  int estado_sensor4 = digitalRead(sensor4);

  Serial.print("Estado do sensor 1: ");
  Serial.println(estado_sensor1);
  delay(1000);
  Serial.print("Estado do sensor 2: ");
  Serial.println(estado_sensor2);
  delay(1000);
  Serial.print("Estado do sensor 3: ");
  Serial.println(estado_sensor3);
  delay(1000);
  Serial.print("Estado do sensor 4: ");
  Serial.println(estado_sensor4);
  delay(1000);

  if ((estado_sensor1 == 1) && (estado_sensor2 == 1) && (estado_sensor3 == 1) && (estado_sensor4 == 1)) {
    Serial.println("Reservatorio Cheio!");
  }
 
  else if ((estado_sensor1 == 0) && (estado_sensor2 == 1) && (estado_sensor3 == 1) && (estado_sensor4 == 1)) {
    Serial.println("Nivel de 70 a 100% - Meio cheio!");
  }

  else if ((estado_sensor1 == 0) && (estado_sensor2 == 0) && (estado_sensor3 == 1) && (estado_sensor4 == 1)) {
    Serial.println("Nivel de 50% a 75% - Meio!");
  }

  else if ((estado_sensor1 == 0) && (estado_sensor2 == 0) && (estado_sensor3 == 0) && (estado_sensor4 == 1)) {
    Serial.println("Nivel em 25% e 50% - Meio baixo!");
  }
  else if ((estado_sensor1 == 0) && (estado_sensor2 == 0) && (estado_sensor3 == 0) && (estado_sensor4 == 0)) {
    Serial.println("Nivel abaixo de 25% - Critico!");
  }
  else {
    Serial.println("ERRO - Verificar sensor!");
  }
  delay(2000);
}
