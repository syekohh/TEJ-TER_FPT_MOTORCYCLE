#include <Servo.h>
#include <LiquidCrystal.h>

// Create an LCD object. Parameters: (RS, E, D4, D5, D6, D7):
LiquidCrystal lcd = LiquidCrystal(7, 6, 5, 4, 3, 2);

Servo Myservo;
int pos = 90;
char readVal = 'd';
float speed = 0;

float distance = 0;
float duration = 0;

const int maxRPM = 480;
const float maxSpeed = 255;
const float pi = 3.14159;
const float wheelRadius = 0.052;

const int trigPin = 9;
const int echoPin = 10;

void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2);
    Myservo.attach(13);
    pinMode(11, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop()
{

    readVal = Serial.read();
    delayMicroseconds(1);

    getDirection();
    getSpeed();
  
}

float calculateSpeed(float speed)
{

    float currentRPM = (speed / maxSpeed) * maxRPM;
    //Serial.println(currentRPM);
    float w = currentRPM * ((2 * pi) / 60);
    //Serial.println(w);
    float velocity = (wheelRadius * w) * 3.6;
    //Serial.println(1/3);

    return velocity;
}

float getDistance()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;

  return distance;
}

void getDirection()
{

    if (readVal == 'a' && pos < 180)
    {
        pos += 30;
        Myservo.write(pos);
        Serial.println(pos);
        lcd.setCursor(5, 0);
        if (pos == 90)
        {
            lcd.clear();
            lcd.setCursor(4, 0);
            lcd.print("STRAIGHT");
        }
        if (pos > 90)
        {
            lcd.clear();
            lcd.setCursor(6, 0);
            lcd.print("LEFT");
        }
        lcd.setCursor(4, 1);
        lcd.print(calculateSpeed(speed));
        lcd.print("km/h");
    }
    else if (readVal == 'd' && pos > 0)
    {
        pos -= 30;
        Myservo.write(pos);
        Serial.println(pos);
        lcd.setCursor(5, 0);
        if (pos == 90)
        {
            lcd.clear();
            lcd.setCursor(4, 0);
            lcd.print("STRAIGHT");
        }
        if (pos < 90)
        {
            lcd.clear();
            lcd.setCursor(5, 0);
            lcd.print("RIGHT");
        }
        lcd.setCursor(4, 1);
        lcd.print(calculateSpeed(speed));
        lcd.print("km/h");
    }
}

void getSpeed()
{

    lcd.setCursor(4, 1);
  	if (getDistance() < 50) {
      	Serial.println("TOO CLOSE!");
      	speed = 0;
      	analogWrite(11, speed);
        Serial.println(speed);
        lcd.print(calculateSpeed(speed));
        lcd.print("km/h");
    }else if (readVal == 'w' && speed < 255 && getDistance() >= 50)
    {
        speed += 5;
        analogWrite(11, speed);
        Serial.println(speed);
        lcd.print(calculateSpeed(speed));
        lcd.print("km/h");
    }
    else if (readVal == 's' & speed > 0 && getDistance() >= 50)
    {
        speed -= 5;
        analogWrite(11, speed);
        Serial.println(speed);
        lcd.print(calculateSpeed(speed));
        lcd.print("km/h");
    }
}