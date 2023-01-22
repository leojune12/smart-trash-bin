#include <HCSR04.h>
#include <Servo.h>

Servo myservo;

// hc(Trig, Echo)
HCSR04 hc_outside(2, 3);
HCSR04 hc_inside(5, 6);

int pos = 0;
int shredder_pin = 4;
int uv_light_pin = 7;
int red_led_pin = 10;
int blue_led_pin = 11;
int green_led_pin = 12;

void setup() {

  Serial.begin(9600);
  
  myservo.attach(8);
  myservo.write(pos);
  myservo.detach();
  
  pinMode(shredder_pin, OUTPUT);
  pinMode(uv_light_pin, OUTPUT);
  pinMode(red_led_pin, OUTPUT);
  pinMode(blue_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);

  green_led_on();
}

void loop() {
  
  check_for_incoming_trash();
}

void check_for_incoming_trash() {

  float distance = read_incoming_object_distance();
  
  Serial.println(distance);
  
  if (distance <= 15) {
    open_lid_servo();
    shredder_on();

    int counter = 0;

    while(true) {

      delay(1000);
      distance = read_incoming_object_distance();
      Serial.println(distance);
      
      if (distance > 15) {
        counter++;
        Serial.print("i=");
        Serial.println(counter);
        if (counter == 5) {
          break;
        }
      } else {
        counter = 0;
      }
    }
    
    shredder_off();
    close_lid_servo();
  }
}

float read_incoming_object_distance() {
  return hc_outside.dist(0);
}

void open_lid_servo() {
  myservo.attach(8);
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(12);                       // waits 15 ms for the servo to reach the position
  }
  myservo.detach();
}

void close_lid_servo() {
  myservo.attach(8);
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(12);                       // waits 15 ms for the servo to reach the position
  }
  myservo.detach();
}

void shredder_on() {
  digitalWrite(shredder_pin, HIGH);
}

void shredder_off() {
  digitalWrite(shredder_pin, LOW);
}

void red_led_on() {
  digitalWrite(red_led_pin, HIGH);
}

void red_led_off() {
  digitalWrite(red_led_pin, LOW);
}

void green_led_on() {
  digitalWrite(green_led_pin, HIGH);
}

void green_led_off() {
  digitalWrite(green_led_pin, LOW);
}

void blue_led_on() {
  digitalWrite(blue_led_pin, HIGH);
}

void blue_led_off() {
  digitalWrite(blue_led_pin, LOW);
}
