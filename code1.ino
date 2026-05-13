  #include <set.h>

  #include <BluetoothSerial.h>
  #include <BluetoothSerialJoystick.h>

  #include <SoftwareSerial.h>



  #include <Adafruit_MotorShield.h>

  #include <Wire.h>

  #include <AFMotor.h>

  #include "utility/Adafruit_MS_PWMServoDriver.h"

  #define TRIGGER_PIN_1 2 // Define the trigger pin for first ultrasonic sensor
  #define ECHO_PIN_1 3 // Define the echo pin for first ultrasonic sensor

  #define TRIGGER_PIN_2 4 // Define the trigger pin for second ultrasonic sensor
  #define ECHO_PIN_2 5 // Define the echo pin for second ultrasonic sensor

  #define TRIGGER_PIN_3 6 // Define the trigger pin for third ultrasonic sensor
  #define ECHO_PIN_3 7 // Define the echo pin for third ultrasonic sensor

  #define IN1 8 // IN1 pin of L298N motor driver
  #define IN2 9 // IN2 pin of L298N motor driver
  #define IN3 10 // IN3 pin of L298N motor driver
  #define IN4 11 // IN4 pin of L298N motor driver
  #define ENA 12// Enable A pin of L298N motor driver
  #define ENB 13// Enable B pin of L298N motor driver
  SoftwareSerial bluetooth(0,1);

  #define BLUETOOTH_SWITCH 14 // Define the pin for switch to turn on the Bluetooth module

  Adafruit_MotorShield AFMS = Adafruit_MotorShield();
  Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
  Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);

  void setup() {
    Serial.begin(9600);
    bluetoothSerial.begin(9600); 
    
    AFMS.begin();
    
    pinMode(TRIGGER_PIN_1, OUTPUT);
    pinMode(ECHO_PIN_1, INPUT);
    pinMode(TRIGGER_PIN_2, OUTPUT);
    pinMode(ECHO_PIN_2, INPUT);
    pinMode(TRIGGER_PIN_3, OUTPUT);
    pinMode(ECHO_PIN_3, INPUT);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    pinMode(BLUETOOTH_SWITCH, INPUT);

    // Set the PWM frequency for the motor driver
    TCCR0B = TCCR0B & B11111000 | B00000001;
    
    // Code to initialize and configure the Bluetooth module
    
    // Additional setup code for Bluetooth module
  }

  void loop() {
    int distance1 = measureDistance(TRIGGER_PIN_1, ECHO_PIN_1);
    int distance2 = measureDistance(TRIGGER_PIN_2, ECHO_PIN_2);
    int distance3 = measureDistance(TRIGGER_PIN_3, ECHO_PIN_3);

    if (digitalRead(BLUETOOTH_SWITCH) == HIGH) {
      // Code to handle control from the mobile app through Bluetooth module
    if (bluetoothSerial.available()) {
      char command = bluetoothSerial.read();
      // Process the command received from the mobile app
      switch (command) {
        case 'F': // Move forward
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          pinMode(ENA, OUTPUT);
        pinMode(ENB, OUTPUT);
          break;
        case 'B': // Move backward
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          pinMode(ENA, OUTPUT);
          pinMode(ENB, OUTPUT);
          break;
        case 'L': // Turn left
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          pinMode(ENA, OUTPUT);
          pinMode(ENB, OUTPUT);
          break;
        case 'R': // Turn right
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          pinMode(ENA, OUTPUT);
          pinMode(ENB, OUTPUT);
          break;
        case 'S': // Stop
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          pinMode(ENA, OUTPUT);
          pinMode(ENB, OUTPUT);
          break;
        // Add more cases for additional commands as needed
      }
    }
  }

  int measureDistance(int triggerPin, int echoPin) {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;
    return distance;
  }

