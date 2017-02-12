
#include <Servo.h> 
#include <NewPing.h>

char val, tempVal;


const int buttonPin = 4;
const int trigPin = 6;
const int echoPin = 5;
int inches;

bool pressed = false, sensed = false;

unsigned long time, previousTime = 0;

Servo myservo;

NewPing sonar(trigPin, echoPin, 30);

void setup()
{
  Serial.begin(9600);
  myservo.attach(9);
  pinMode(buttonPin, INPUT_PULLUP);
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

    inches = sonar.ping_in(30);
  
   if(inches <= 7 && inches > 0 && (time - 2000) > previousTime)
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

