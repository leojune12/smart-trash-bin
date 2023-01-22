#include <HCSR04.h>
#include <Servo.h>

Servo myservo;

// hc(Trig, Echo)
HCSR04 hc(5, 6);

int pos = 0;

void setup() {

  myservo.attach(9);
  Serial.begin(9600);
}

void loop() {
  
  float distance = read_incoming_object_distance();
  Serial.println(distance);
}

float read_incoming_object_distance() {
  return hc.dist(0);
}

void move_servo() {
  
}
