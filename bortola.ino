// =================== PIN DEFINITIONS ===================
// Motor Pins
#define ENA 3
#define IN1 4
#define IN2 5
#define ENB 9
#define IN3 6
#define IN4 7

// Encoder Pins
#define ENC1_DO 2  // Encoder 1 — interrupt pin
#define ENC2_DO 0  // Encoder 2 — second encoder 

// Ultrasonic Sensor Pins
#define TRIG A0
#define ECHO A1

// Line Follower Pins
#define LINE_SENSOR1 8
#define LINE_SENSOR2 13
#define LINE_SENSOR3 10
#define LINE_SENSOR4 12
#define LINE_SENSOR5 11

// =================== VARIABLES ===================
// Encoder variables
volatile int encoderCount1 = 0;
volatile int encoderCount2 = 0;
const int pulsesPerRev = 85;           
const float wheelCircumference = 21.0;  
float pulsesPerCM = pulsesPerRev / wheelCircumference;

// Timing
#define OBSTACLE_WAIT_TIME 1000

// =================== SETUP ===================
void setup() {
  // Motor setup
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Encoders
  pinMode(ENC1_DO, INPUT);
  pinMode(ENC2_DO, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC1_DO), encoder1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC2_DO), encoder2ISR, RISING);

  // Ultrasonic sensor
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Line sensors
  pinMode(LINE_SENSOR1, INPUT);
  pinMode(LINE_SENSOR2, INPUT);
  pinMode(LINE_SENSOR3, INPUT);
  pinMode(LINE_SENSOR4, INPUT);
  pinMode(LINE_SENSOR5, INPUT);

 
  delay(2000);  
}

// =================== MAIN LOOP ===================
void loop() {


  lineFollowing(); 
}
// =================== ENCODER INTERRUPTS ===================
void encoder1ISR() {
  encoderCount1++;
}

void encoder2ISR() {
  encoderCount2++;
}

// =================== STOP MOTOR ===================
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// =================== LINE FOLLOWING ===================
void lineFollowing() {
  int baseSpeed = 60;  // Base speed for motors (0-255)
  int turnSpeed = 50;  // Speed for turning

  while (true) {
    int sensor1 = digitalRead(LINE_SENSOR1);
    int sensor2 = digitalRead(LINE_SENSOR2);
    int sensor3 = digitalRead(LINE_SENSOR3);
    int sensor4 = digitalRead(LINE_SENSOR4);
    int sensor5 = digitalRead(LINE_SENSOR5);

    Serial.print("Sensors: ");
    Serial.print(sensor1);
    Serial.print(" ");
    Serial.print(sensor2);
    Serial.print(" ");
    Serial.print(sensor3);
    Serial.print(" ");
    Serial.print(sensor4);
    Serial.print(" ");
    Serial.println(sensor5);

    // Black line = LOW, White surface = HIGH
    if (sensor1 == HIGH && sensor2 == HIGH && sensor3 == HIGH && sensor5 == HIGH) {
      moveBackward(baseSpeed);
      moveBackward(baseSpeed);
      turnLeft(turnSpeed);
      turnRight(turnSpeed);
      stopMotors();
      moveBackward(baseSpeed);
    }
    if (sensor3 == LOW && sensor2 == HIGH && sensor4 == HIGH) {
      moveForward(baseSpeed);
    } else if (sensor2 == LOW) {
      stopMotors();
      moveBackward(baseSpeed);
      turnLeft(turnSpeed);
    } else if (sensor1 == LOW && sensor5 == HIGH) {
      stopMotors();
      moveBackward(baseSpeed);
      turnLeft(turnSpeed);
    } else if (sensor4 == LOW && sensor5 == HIGH) {
      stopMotors();
      moveBackward(baseSpeed);
      turnRight(turnSpeed);
    } else if (sensor5 == LOW) {
      stopMotors();
      moveBackward(baseSpeed);
      turnRight(turnSpeed);
    } else {
      stopMotors();
      moveBackward(baseSpeed);
      stopMotors();
    }
  }
}
// Function to move forward
void moveForward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Function to turn slightly right
void turnRight(int speed) {
  analogWrite(ENA, 0);
  analogWrite(ENB, 150);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Function to turn slightly left
void turnLeft(int speed) {
  analogWrite(ENA, speed + 30);
  analogWrite(ENB, 0);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


float objectDistance() {
  long duration, distance;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = (duration / 2) / 29.1;
  return distance;
}