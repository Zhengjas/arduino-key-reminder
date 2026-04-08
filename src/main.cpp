#include <Arduino.h>

#define echoPin 2
#define trigPin 3
#define ledPin 4
#define buzzerPin 5
#define buttonPin 6

#define CALIBRATION_MS 10000
#define CHANGE_THRESHOLD 10

long duration;
int distance;
int baselineDistance;
bool calibrated = false;
bool doorOpen = false;
bool buzzerActive = false;

int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 30000);
  return duration / 58;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  delay(5000);
  Serial.begin(9600);
  Serial.println("Calibrating...");

  long sum = 0;
  int samples = 0;
  unsigned long start = millis();
  while (millis() - start < CALIBRATION_MS) {
    sum += measureDistance();
    samples++;
    digitalWrite(ledPin, HIGH);
    delay(50);
    digitalWrite(ledPin, LOW);
    delay(50);
  }
  baselineDistance = sum / samples;
  calibrated = true;
  Serial.print("Baseline: ");
  Serial.print(baselineDistance);
  Serial.println(" cm");
}

void loop() {
  distance = measureDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  int delta = abs(distance - baselineDistance);
  if (delta >= CHANGE_THRESHOLD) {
    Serial.println("OPEN");
    digitalWrite(ledPin, HIGH);
    if (!doorOpen) {
      doorOpen = true;
      buzzerActive = true;
    }
  } else {
    Serial.println("CLOSED");
    digitalWrite(ledPin, LOW);
    doorOpen = false;
  }

  if (buzzerActive) {
    if (digitalRead(buttonPin) == LOW) {
      buzzerActive = false;
      noTone(buzzerPin);
    } else {
      tone(buzzerPin, 1000);
    }
  }

  delay(100);
}
