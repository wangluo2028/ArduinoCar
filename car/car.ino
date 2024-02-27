#define DECODE_NEC   

#include <Arduino.h>

#include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.
#include <IRremote.hpp>

#include <Servo.h>



Servo servo_9;


void back() {
  digitalWrite(4,LOW);
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
}

void bizhang() {
  if (digitalRead(11) == 0 && digitalRead(12) == 0) {
    stop();
    delay(200);
    back();
    delay(300);
    left();
    delay(300);

  } else if (digitalRead(A2) == 1 && digitalRead(A1) == 0) {
    stop();
    delay(200);
    back();
    delay(300);
    left();
    delay(300);
  } else if (digitalRead(A2) == 0 && digitalRead(A1) == 1) {
    stop();
    delay(200);
    back();
    delay(300);
    right();
    delay(300);
  } else if (digitalRead(A2) == 1 && digitalRead(A1) == 1) {
    go();
  }
}

float checkdistance_2_3() {
  digitalWrite(2, LOW);
  delayMicroseconds(2);
  digitalWrite(2, HIGH);
  delayMicroseconds(10);
  digitalWrite(2, LOW);
  float distance = pulseIn(3, HIGH) / 58.00;
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

void go() {
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(7,LOW);
}

void left() {
  digitalWrite(4,LOW);
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
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

void right() {
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
}

void stop() {
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(7,LOW);
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

const int csb_pin = 9;

void setup(){
  Serial.begin(115200);

  IrReceiver.begin(A0);
  
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  pinMode(A2, INPUT);
  pinMode(A1, INPUT);

  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  
}

long ir_item = -1;
const int DelayTime = 100;

void loop(){
  if (IrReceiver.decode()) {
    // Print a short summary of received data
      //IrReceiver.printIRResultShort(&Serial);
      //IrReceiver.printIRSendUsage(&Serial);
      if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
          Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
          // We have an unknown protocol here, print more info
          //IrReceiver.printIRResultRawFormatted(&Serial, true);
          IrReceiver.resume(); // Enable receiving of the next value
          return;
      }
      Serial.print("command:");
      Serial.println(IrReceiver.decodedIRData.command, HEX);
      Serial.println();

      ir_item = IrReceiver.decodedIRData.command;
              /*
       * !!!Important!!! Enable receiving of the next value,
       * since receiving has stopped after the end of the current received data packet.
       */
      IrReceiver.resume(); // Enable receiving of the next value
        
    if (ir_item == 0x18) {
      Serial.println("go");
      go();
      delay(DelayTime);
      stop();
    } else if (ir_item == 0x52) {
      Serial.println("back");
      back();
      delay(DelayTime);
      stop();
    } else if (ir_item == 0x8) {
      Serial.println("left");
      left();
      delay(DelayTime);
      stop();
    } else if (ir_item == 0x5A) {
      Serial.println("right");
      right();
      delay(DelayTime);
      stop();
    } else if (ir_item == 0x1C) {
      Serial.println("stop");
      stop();
      delay(DelayTime);
    } else if (ir_item == 0x45) {
      Serial.println("xunji");
      xunji();
    } else if (ir_item == 0x46) {
      Serial.println("bizhang");
      bizhang();
    } else if (ir_item == 0x47) {
      Serial.println("csb");
      servo_9.attach(csb_pin);
      servo_9.write(90);
      delay(1000);
      csb();
      delay(10000);
      servo_9.detach();
    } else if (ir_item == 0x44) {
      Serial.println("mfs");
      servo_9.attach(csb_pin);
      servo_9.write(90);
      delay(1000);
      mfs();
      delay(10000);
      servo_9.detach();
    }
  } else {
  }

}
