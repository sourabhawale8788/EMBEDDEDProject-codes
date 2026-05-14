#include <SoftwareSerial.h>

// Defining the pins
const int trigPin1 = 3;
const int echoPin1 = 5;
const int trigPin2 = 6;
const int echoPin2 = 9;
const int trigPin3 = 10;
const int echoPin3 = 11;
int irpin = 2;

// Defining variables
long duration1;
long duration2;
long duration3;
int distanceleft;
int distancefront;
int distanceright;
int a = 0;

// Bluetooth setup
SoftwareSerial BTSerial(13, 14); // RX | TX

// Motor pins
const int motor1A = 4;
const int motor1B = 7;
const int motor2A = 8;
const int motor2B = 12;

// Bluetooth control state
bool bluetoothControl = false;

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  pinMode(irpin, INPUT);
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  
  // Start Bluetooth communication
  BTSerial.begin(9600);
}

void loop() {
  // Check if Bluetooth control is active
  if (BTSerial.available() > 0) {
    char command = BTSerial.read();
    if (command == '1') {
      bluetoothControl = true; // Activate Bluetooth control
    } else if (command == '0') {
      bluetoothControl = false; // Deactivate Bluetooth control
    }
  }

  if (!bluetoothControl) {
    // Ultrasonic sensor control
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    duration1 = pulseIn(echoPin1, HIGH);
    distanceleft = duration1 * 0.034 / 2;
    Serial.print("Distance1: ");
    Serial.println(distanceleft);

    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    duration2 = pulseIn(echoPin2, HIGH);
    distancefront = duration2 * 0.034 / 2;
    Serial.print("Distance2: ");
    Serial.println(distancefront);

    digitalWrite(trigPin3, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin3, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin3, LOW);
    duration3 = pulseIn(echoPin3, HIGH);
    distanceright = duration3 * 0.034 / 2;
    Serial.print("Distance3: ");
    Serial.println(distanceright);

    int s = digitalRead(irpin);

    if ((s == LOW) && (distanceleft <= 20 && distancefront > 20 && distanceright <= 20) || (s == LOW) && (distanceleft > 20 && distancefront > 20 && distanceright > 20)) {
      // Move forward
      moveForward();
    } else if ((s == LOW) && (distanceleft <= 20 && distancefront <= 20 && distanceright > 20) || (s == LOW) && (distanceleft <= 20 && distancefront <= 20 && distanceright > 20) || (s == LOW) && (distanceleft <= 20 && distancefront > 20 && distanceright > 20) || (distanceleft <= 20 && distancefront > 20 && distanceright > 20)) {
      // Move backward
      moveBackward();
      delay(100);
      a = 0;
    } else if ((s == LOW) && (distanceleft > 20 && distancefront <= 20 && distanceright <= 20) || (s == LOW) && (distanceleft > 20 && distancefront > 20 && distanceright <= 20) || (s == LOW) && (distanceleft > 20 && distancefront <= 20 && distanceright > 20)) {
      // Turn right
      turnRight();
    } else {
      // Stop
      stopMotors();
    }
  }
}

void moveForward() {
  digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void moveBackward() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, HIGH);
}

void turnRight() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, HIGH);
  digitalWrite(motor2A, HIGH);
  digitalWrite(motor2B, LOW);
}

void stopMotors() {
  digitalWrite(motor1A, LOW);
  digitalWrite(motor1B, LOW);
  digitalWrite(motor2A, LOW);
  digitalWrite(motor2B, LOW);
}
