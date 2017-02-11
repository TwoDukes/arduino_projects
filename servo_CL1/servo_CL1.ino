
#include <Servo.h> 
#include <avr/interrupt.h>

volatile char val, tempVal;
volatile long duration;
volatile long pulseTime;

const int buttonPin = 4;
const int trigPin = 6;
const int echoPin = 5;
const int interuptPin = 0;

bool pressed = false, sensed = false;

unsigned long time, previousTime = 0;

Servo myservo;

void setup()
{
  Serial.begin(9600);
  myservo.attach(9);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //pinMode(interuptPin, INPUT_PULLUP);  // Enable pullup resistor
  attachInterrupt(interuptPin, pulseTest, RISING);
}

void pulseTest()
{
  ultraWrite();
}

void loop ()
{
  time = millis();
  ultraRead();
  buttonRead();
  if(tempVal != val){
    setServo();
  }
  tempVal = val;
}

void ultraRead()
{ 
  delay(2000);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pulseTime = micros();

}

void ultraWrite()
{
    duration = (micros() - pulseTime);
  
   int inches = duration / 74 / 2;
  
  Serial.println(inches);
  
   if(inches <= 7 && inches > 1 && (time - 2000) > previousTime)
  {
   sensed = true;
   previousTime = time;
   if(val == '0') {
        val = '1';
   }
   else {
        val = '0'; 
   }
  }else{
   sensed = false; 
  }
}

void buttonRead()
{
  int sensorVal = digitalRead(buttonPin);
  
  if(sensorVal == LOW && !pressed)
  {
   pressed = true;
   if(val == '0') {
        val = '1';
   }
   else {
        val = '0'; 
   }
  }else{
   pressed = false; 
  }
}

void serialEvent()
{
  if (Serial.available() > 0) 
  {
    val = Serial.read();
  }
}

void moveServo(int initial, int del, int final)
{
      myservo.write(initial);
      delay(del);
      myservo.write(final);
}

void setServo()
{
  if (val == '0')
    {
      moveServo(0,200, 82);
    }
    else if (val == '1') 
    {
      moveServo(180,200, 90);
    }
}

