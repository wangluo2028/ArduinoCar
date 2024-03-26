#define DECODE_NEC   

#include <Arduino.h>

#include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.
#include <IRremote.hpp>

#include <Servo.h>

Servo servo_9;



const int DelayTime = 100;
const int BiZhangDelayTime = DelayTime / 10;

long ir_item = -1;

const int stepAngle = 30;

const int ObstacleDistance = 20;

const int checkAngle = 10;

const int LeftCheckPin = 11;
const int RightCheckPin = 12;
const int HumanDetectPin = 13;
const int ServoDuoJiPin = 9;
const int ChaoShengBoOutputPin = 2;
const int ChaoShengBoInputPin = 3;

#define IRemotePin A0

void bizhang() {
  if (digitalRead(LeftCheckPin) == 0 && digitalRead(RightCheckPin) == 0) {
    Serial.println("left right obstacle");
    stop();
    delay(BiZhangDelayTime);
    back();
    delay(BiZhangDelayTime * 2);
    left();
    delay(BiZhangDelayTime * 2);

  } else if (digitalRead(LeftCheckPin) == 1 && digitalRead(RightCheckPin) == 0) {
    Serial.println("right obstacle");
    stop();
    delay(BiZhangDelayTime);
    back();
    delay(BiZhangDelayTime*2);
    left();
    delay(BiZhangDelayTime*2);
  } else if (digitalRead(LeftCheckPin) == 0 && digitalRead(RightCheckPin) == 1) {
    Serial.println("left obstacle");
    stop();
    delay(BiZhangDelayTime);
    back();
    delay(BiZhangDelayTime*2);
    right();
    delay(BiZhangDelayTime*2);
  } else if (digitalRead(LeftCheckPin) == 1 && digitalRead(RightCheckPin) == 1) {
    Serial.println("no obstacle");
    go();
    delay(BiZhangDelayTime*2);
  }
}

void ServoTurnAngle(Servo& servo, int angle)
{
  stop();
  delay(DelayTime);
  int currentAngle = servo.read();
  int nextAngle = currentAngle + angle;
  if (nextAngle >= 180)
  {
    nextAngle = 180;
  }
  servo.write(nextAngle);
  delay(1000);
}

void go() {
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(7,LOW);
}

void left() {
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
}

void right() {
  digitalWrite(4,LOW);
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
}

void stop() {
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
}

void back() {
  digitalWrite(4,LOW);
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
}

float checkdistance_2_3() {
  digitalWrite(ChaoShengBoOutputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ChaoShengBoOutputPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ChaoShengBoOutputPin, LOW);
  float distance = pulseIn(ChaoShengBoInputPin, HIGH) / 58.00;
  delay(10);
  return distance;
}

void csb() {
  if (checkdistance_2_3() > 15) {
    go();

  } else if (checkdistance_2_3() <= 15) {
    stop();
    delay(200);
    back();
    delay(200);
    left();
    delay(200);
  }
}

void BiZhangWithCsb()
{
  int csbDistance = checkdistance_2_3();
  if ((digitalRead(LeftCheckPin) == 0 && digitalRead(RightCheckPin) == 0) || csbDistance <= ObstacleDistance)
  {
    Serial.println("left right obstacle");
    stop();
    delay(BiZhangDelayTime);
//    back();
//    delay(BiZhangDelayTime);
    left();
    delay(BiZhangDelayTime);
  } 
  else if (digitalRead(LeftCheckPin) == 1 && digitalRead(RightCheckPin) == 0) {
    Serial.println("right obstacle");
    stop();
    delay(BiZhangDelayTime);
    left();
    delay(BiZhangDelayTime);
  } else if (digitalRead(LeftCheckPin) == 0 && digitalRead(RightCheckPin) == 1) {
    Serial.println("left obstacle");
    stop();
    delay(BiZhangDelayTime);
    right();
    delay(BiZhangDelayTime);
  } else if (digitalRead(LeftCheckPin) == 1 && digitalRead(RightCheckPin) == 1) {
    Serial.println("no obstacle");
    go();
    delay(BiZhangDelayTime);
  }
}

void xunji() {
  if (digitalRead(A2) == 0 && digitalRead(A1) == 0) {
    stop();

  } else if (digitalRead(A2) == 1 && digitalRead(A1) == 0) {
    left();
  } else if (digitalRead(A2) == 0 && digitalRead(A1) == 1) {
    right();
  } else if (digitalRead(A2) == 1 && digitalRead(A1) == 1) {
    go();
  }
}

void mfs() {
  if (checkdistance_2_3() < 6) {
    back();

  } else if (checkdistance_2_3() >= 6 && checkdistance_2_3() <= 12) {
    go();
  } else if (checkdistance_2_3() > 12) {
    stop();
  }
}



void setup(){
  Serial.begin(9600);

  IrReceiver.begin(IRemotePin);
  
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  pinMode(A2, INPUT);
  pinMode(A1, INPUT);

  pinMode(LeftCheckPin, INPUT);
  pinMode(RightCheckPin, INPUT);
  pinMode(ChaoShengBoOutputPin, OUTPUT);
  pinMode(ChaoShengBoInputPin, INPUT);

  pinMode(HumanDetectPin, INPUT);

  servo_9.attach(ServoDuoJiPin);
  servo_9.write(90);
  delay(1000);
}


void loop(){
    // check if we have bluetooth
    char val = Serial.read();
    Serial.write(val);
    if (-1 != val) {
      if ('w' == val)
      {
        Serial.println("go");
        go();
        delay(DelayTime);
        stop();
      }
      else if ('a' ==val)
      {
        Serial.println("left");
        left();  
        delay(DelayTime);
        stop();
      }   
      else if ('e' ==val)
      {
        Serial.println("stop");
        stop();  
        delay(DelayTime);
        stop();
      }
      else if ('d' == val)
      {
        Serial.println("right");
        right();  
        delay(DelayTime);
        stop();
      }
      else if ('s' == val)
      {
        Serial.println("back");
        back();  
        delay(DelayTime);
        stop();
      }
      
      return;
    }
  
    long curMode = -1;
    if (IrReceiver.decode()) 
    {
      if (IrReceiver.decodedIRData.protocol == UNKNOWN) 
      {
          IrReceiver.resume();
      }
      else
      {
        ir_item = IrReceiver.decodedIRData.command;
        curMode = ir_item;
        IrReceiver.resume(); // Enable receiving of the next value

        Serial.print("command:");
        Serial.println(ir_item, HEX);
        Serial.println();
      }
    }

    if (curMode > 0)
    {
      if (ir_item == 0x18) 
      {
          Serial.println("go");
          go();
          delay(DelayTime);
          stop();
      } 
      else if (ir_item == 0x52) 
      {
        Serial.println("back");
        back();
        delay(DelayTime);
        stop();
      } 
      else if (ir_item == 0x8) 
      {
        Serial.println("left");
        left();
        delay(DelayTime);
        stop();
      } 
      else if (ir_item == 0x5A) 
      {
        Serial.println("right");
        right();
        delay(DelayTime);
        stop();
      } 
      else if (ir_item == 0x1C) 
      {
        Serial.println("stop");
        stop();
        delay(DelayTime);
      } 
      else if (ir_item == 0x40) // 5
      {
        ServoTurnAngle(servo_9, stepAngle);
      }
      else if (ir_item == 0x43) // 6
      {
        ServoTurnAngle(servo_9, -stepAngle);
      }
    }
    else
    {
      if (ir_item == 0x45) // 1
      {
        Serial.println("xunji");
        xunji();
      } 
      else if (ir_item == 0x46) // 2
      {
        Serial.println("bizhang");
        bizhang();
      } 
      else if (ir_item == 0x47) // 3
      {
        Serial.println("csb");
        csb();
      }
      else if (ir_item == 0x44) //4
      {
        mfs();
      }
      else if (ir_item == 0x7) // 7
      {
        BiZhangWithCsb();
      }
      else if (ir_item == 0x15) // 8
      {
        
      }
      else if (ir_item == 0x9) // 9
      {
        
      }
    }
    
}
