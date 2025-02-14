
int sensor1 = 5; /* Define sensor1 como pino 5 */
int sensor2 = 6; /* Define sensor2 como pino 6 */
int sensor2 = 7; /* Define sensor2 como pino 7 */

void setup() {
  Serial.begin(9600);
  
  pinMode(sensor1, INPUT); /* Define o pino do sensor como entrada */
  pinMode(sensor2, INPUT); /* Define o pino do sensor como entrada */
}

void loop() {
  int estado_sensor1 = digitalRead(sensor1);
  int estado_sensor2 = digitalRead(sensor2);
  Serial.print("Estado do sensor 1: ");
  Serial.println(estado_sensor1);
  Serial.print("Estado do sensor 2: ");
  Serial.println(estado_sensor2);
  delay(1500);
}