#include <Arduino.h>
#include <Ultrasonic.h>

#define echoPin 4
#define trigPin 2

long duration; // variable for the duration of sound wave travel
float distance;  // variable for the distance measurement
float lastDistance;
int andarAtual;

const int motorB1 = 3;
const int motorB2 = 6;

const int bterreo = 9;
const int bandar1 = 10;
const int bandar2 = 11;
const int bandar3 = 12;

int terreo = 0;
int andar1 = 0;
int andar2 = 0;
int andar3 = 0;
int andarAnt = 5;

int vSpeed = 250;

byte botaoandar = 4;
byte andar();
int checarAndar(float distancia);
float CheckMaxMin(float distance);
float getDistance();
void log();
void parado();
void desce();
void sobe();
unsigned long delayTempo;

// Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(echoPin, trigPin);

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);  // Sets the echoPin as an INPUT

  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  pinMode(bterreo, INPUT_PULLUP);
  pinMode(bandar1, INPUT_PULLUP);
  pinMode(bandar2, INPUT_PULLUP);
  pinMode(bandar3, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("inciando do andar 0");

  do {
    distance = getDistance();
    distance = CheckMaxMin(distance);
    andarAtual = checarAndar(distance);
    desce();
  }while(andarAtual != 0);
  parado();
  andarAtual = 0;
}

byte andar() {
  distance = getDistance();
  // delay(100);
  return checarAndar(distance);
}

float getDistance() {
  float dist;
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  // distance = ultrasonic.convert(duration, Ultrasonic::CM);
  dist = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  dist = CheckMaxMin(dist);
  return dist;
}

float CheckMaxMin(float dist) {
  if (dist > 500) {
    return 1;
  }

  if (dist >= 31) {
    return 31;
  }

  if (dist <= 1) {
    return 1;
  }

  return dist;
}

int checarAndar(float distancia) {
  // terreo
  if (distancia >= 31) {
    // Serial.print("Terreo\n");
    andarAtual = 0;
    andarAnt = andarAtual;
  }
  // primeiro andar
  else if (distancia >= 20.5 && distancia <= 21.5) {
    // Serial.print("Primeiro Andar\n");
    andarAtual = 1;
    andarAnt = andarAtual;
  }
  // segundo andar
  else if (distancia >= 10.5 && distancia <= 11) {
    // Serial.print("Segundo Andar\n");
    andarAtual = 2;
    andarAnt = andarAtual;
  }
  else if (distancia <= 1) {
    // Serial.print("Terceiro Andar\n");
    andarAtual = 3;
    andarAnt = andarAtual;
  }
  
  log();

  if (andarAnt == andarAtual)
    return andarAtual;
}

void log() {
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm ");
  Serial.print(" Destino: ");
  Serial.print(botaoandar);
  Serial.print(" Atual: ");
  Serial.println(andarAtual);
}

// Inicia o motor com o comando descer
void desce()
{
  analogWrite(motorB1, vSpeed);
  analogWrite(motorB2, 0);
}

// Inicia o motor com o comando subir
void sobe()
{
  analogWrite(motorB1, 0);
  analogWrite(motorB2, vSpeed);
}

// Inicia o motor com o comando parar
void parado()
{
  analogWrite(motorB1, 0);
  analogWrite(motorB2, 0);
}

void loop() {
  if (botaoandar == 4) {
    terreo = digitalRead(bterreo);
    if (terreo == LOW) {
      botaoandar = 0;
      Serial.print("Botao T\n");

      delayTempo = millis();
    }

    andar1 = digitalRead(bandar1);
    if (andar1 == LOW) {
      botaoandar = 1;
      Serial.print("Botao 1\n");
      delayTempo = millis();
    }

    andar2 = digitalRead(bandar2);
    if (andar2 == LOW) {
      botaoandar = 2;
      Serial.print("Botao 2\n");
      delayTempo = millis();
    }

    andar3 = digitalRead(bandar3);
    if (andar3 == LOW) {
      botaoandar = 3;
      Serial.print("Botao 3\n");
      delayTempo = millis();
    }
  }
  else {
    if (andar() != 4) {
      lastDistance = distance;
      if (botaoandar > andar()) {
          // Serial.print("subiu\n");
          sobe();
      }

      if (botaoandar < andar()) {
          // Serial.print("desceu\n");
          desce();
      }

      if (botaoandar == andar()) {
        parado();
        botaoandar = 4;
      }
    }
  }
}

void move() {
  
}
