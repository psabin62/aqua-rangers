// Define pins for ultrasonic sensors
#define TRIG_FRONT 2
#define ECHO_FRONT 3
#define TRIG_LEFT 4
#define ECHO_LEFT 5
#define TRIG_RIGHT 6
#define ECHO_RIGHT 7

// Define pins for motor control
#define ENA 9  // PWM control for left motor
#define ENB 10 // PWM control for right motor
#define IN1 8  // Left motor forward
#define IN2 11 // Left motor backward
#define IN3 12 // Right motor forward
#define IN4 13 // Right motor backward

// Define speed
#define MOTOR_SPEED 150

// Function to measure distance using an ultrasonic sensor
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Convert to cm
}

// Motor control functions
void moveForward() {
  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  analogWrite(ENA, MOTOR_SPEED / 2); // Slow down left motor
  analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED / 2); // Slow down right motor
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setup() {
  // Set up motor pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set up ultrasonic sensor pins
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  Serial.begin(9600);
}

void loop() {
  // Measure distances
  long frontDistance = getDistance(TRIG_FRONT, ECHO_FRONT);
  long leftDistance = getDistance(TRIG_LEFT, ECHO_LEFT);
  long rightDistance = getDistance(TRIG_RIGHT, ECHO_RIGHT);

  Serial.print("Front: ");
  Serial.print(frontDistance);
  Serial.print(" cm, Left: ");
  Serial.print(leftDistance);
  Serial.print(" cm, Right: ");
  Serial.println(rightDistance);

  // Navigation logic
  if (frontDistance < 20) {
    // Obstacle ahead, stop and decide
    stopMotors();
    delay(500);
    if (leftDistance > rightDistance) {
      turnLeft();
      delay(1000);
    } else {
      turnRight();
      delay(1000);
    }
  } else if (leftDistance < 15) {
    // Obstacle on the left, turn right
    turnRight();
  } else if (rightDistance < 15) {
    // Obstacle on the right, turn left
    turnLeft();
  } else {
    // Clear path, move forward
    moveForward();
  }

  delay(100); // Delay for stability
}
