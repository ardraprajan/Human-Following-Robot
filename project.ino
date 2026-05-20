#define ULTRASONIC_SENSOR_TRIG 10
#define ULTRASONIC_SENSOR_ECHO 9
#define MAX_FORWARD_MOTOR_SPEED 100
#define MAX_MOTOR_TURN_SPEED_ADJUSTMENT 50

#define MIN_DISTANCE 10
#define MAX_DISTANCE 30

#define IR_SENSOR_RIGHT 13
#define IR_SENSOR_LEFT 12

// Right motor
int enableRightMotor = 6;
int rightMotorPin1 = 7;
int rightMotorPin2 = 5;

// Left motor
int enableLeftMotor = 3;
int leftMotorPin1 = 4;
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
  
  digitalWrite(ULTRASONIC_SENSOR_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(ULTRASONIC_SENSOR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_SENSOR_TRIG, LOW);

  int duration = pulseIn(ULTRASONIC_SENSOR_ECHO , HIGH);

  int distance= (duration * 0.0343) / 2;

  int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
  int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);

  

  if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH)
  {
    rotateMotor(0,MAX_FORWARD_MOTOR_SPEED + MAX_MOTOR_TURN_SPEED_ADJUSTMENT);
 
  }
  else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW)
  {
    rotateMotor(MAX_FORWARD_MOTOR_SPEED + MAX_MOTOR_TURN_SPEED_ADJUSTMENT,0);
    
  }
 else if (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE)
  {
    rotateMotor(MAX_FORWARD_MOTOR_SPEED, MAX_FORWARD_MOTOR_SPEED);
    }

  else if (distance < MIN_DISTANCE)
  {
    rotateMotor(-MAX_FORWARD_MOTOR_SPEED, -MAX_FORWARD_MOTOR_SPEED);
  }

  else
  {
    rotateMotor(0, 0);
    
  }

  delay(200); 
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  // Right motor direction
  if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  }
  else if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  }
  else
  {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, HIGH);
  }

  // Left motor direction
  if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  }
  else if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  }
  else
  {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }

  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));
}
