#define ULTRASONIC_SENSOR_TRIG 13
#define ULTRASONIC_SENSOR_ECHO 12

#define MAX_FORWARD_MOTOR_SPEED 100
#define MAX_MOTOR_TURN_SPEED_ADJUSTMENT 50

#define MIN_DISTANCE 5
#define MAX_DISTANCE 30

#define IR_SENSOR_RIGHT 10
#define IR_SENSOR_LEFT 9

// Right motor
int enableRightMotor = 5;
int rightMotorPin1 = 4;
int rightMotorPin2 = 3;

// Left motor
int enableLeftMotor = 6;
int leftMotorPin1 = 7;
int leftMotorPin2 = 2;

void setup()
{
  Serial.begin(9600);

  // Motor pins
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  // Sensor pins
  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);
  pinMode(ULTRASONIC_SENSOR_TRIG, OUTPUT);
  pinMode(ULTRASONIC_SENSOR_ECHO, INPUT);

  rotateMotor(0, 0);
}

void loop()
{
  int distance = getDistance();  // 
  int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
  int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);

  // --- Main logic ---
  if (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE)
  {
    // Object in front within range → move forward
    rotateMotor(MAX_FORWARD_MOTOR_SPEED, MAX_FORWARD_MOTOR_SPEED);
    Serial.println("Moving Forward");
  }
  else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH)
  {
    // Right IR detects → turn right
    rotateMotor(MAX_FORWARD_MOTOR_SPEED - MAX_MOTOR_TURN_SPEED_ADJUSTMENT,
                MAX_FORWARD_MOTOR_SPEED + MAX_MOTOR_TURN_SPEED_ADJUSTMENT);
    Serial.println("Turning Right");
  }
  else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW)
  {
    // Left IR detects → turn left
    rotateMotor(MAX_FORWARD_MOTOR_SPEED + MAX_MOTOR_TURN_SPEED_ADJUSTMENT,
                MAX_FORWARD_MOTOR_SPEED - MAX_MOTOR_TURN_SPEED_ADJUSTMENT);
    Serial.println("Turning Left");
  }
  else
  {
    // Otherwise stop
    rotateMotor(0, 0);
    Serial.println("Stopped");
  }

  delay(100); // avoid sensor spam
}

// --- Function to measure distance using ultrasonic sensor ---
int getDistance()
{
  digitalWrite(ULTRASONIC_SENSOR_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_SENSOR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_SENSOR_TRIG, LOW);

  long duration = pulseIn(ULTRASONIC_SENSOR_ECHO, HIGH, 30000); // 30ms timeout
  int distance = duration * 0.034 / 2;

  if (distance == 0 || distance > 400)
  {
    distance = 400; // ignore invalid readings
  }

  return distance;
}

// --- Function to rotate motors ---
void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  // Right motor direction
  if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  }
  else if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  }
  else
  {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }

  // Left motor direction
  if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1, HIGH);
    
    digitalWrite(leftMotorPin2, LOW);
  }
  else if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  }
  else
  {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }

  // Set motor speed (PWM)
  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));
}
