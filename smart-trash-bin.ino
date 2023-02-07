#include <HCSR04.h>
#include <Servo.h>

Servo myservo;

// hc(Trig, Echo)
HCSR04 hc_bin(2, 3);
int uv_light_pin= 4;

int inductive_proximity_sensor = 5;
int shredder_pin  = 6;
int lid_servo = 7;

int red_led_pin = 10;
int blue_led_pin = 11;
int green_led_pin = 12;

int pos = 0;

int reset_pin = 8;

int potentiometer_pin = A5;
int potentiometer_value = 0;

void setup() {

  Serial.begin(9600);
  
  myservo.attach(8);
  myservo.write(pos);
  myservo.detach();
  
  pinMode(shredder_pin, OUTPUT);
  pinMode(inductive_proximity_sensor, INPUT);
  pinMode(uv_light_pin, OUTPUT);
  pinMode(red_led_pin, OUTPUT);
  pinMode(blue_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
  pinMode(reset_pin, INPUT_PULLUP);

  green_led_on();
}

void loop() {
  
  check_for_incoming_trash();
  check_if_bin_is_full();
}

void check_for_incoming_trash() {

  bool trash_detected = digitalRead(inductive_proximity_sensor);
  
  Serial.println(trash_detected);
  
  if (trash_detected) {
    open_lid_servo();
    shredder_on();

    int counter = 0;

    while(true) {

      delay(1000);
      trash_detected = digitalRead(inductive_proximity_sensor);
      Serial.println(trash_detected);
      
      if (!trash_detected) {
        counter++;
        Serial.print("i=");
        Serial.println(counter);
        if (counter == 10) {
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

void check_if_bin_is_full() {

  float distance = read_bin_distance();
  
  Serial.println(distance);

  potentiometer_value = analogRead(potentiometer_pin);
  potentiometer_value = map(potentiometer_value, 0, 1023, 0, 50);
  Serial.print("Potentiometer value: ");
  Serial.println(potentiometer_value );
  
  if (distance <= potentiometer_value) {
    
    int counter = 0;

    while(true) {

      delay(1000);
      distance = read_bin_distance();
      Serial.println(distance);

      potentiometer_value = analogRead(potentiometer_pin);
      potentiometer_value = map(potentiometer_value, 0, 1023, 0, 50);
      Serial.print("Potentiometer value: ");
      Serial.println(potentiometer_value );
      
      if (distance <= potentiometer_value) {
        counter++;
        Serial.print("i=");
        Serial.println(counter);
        if (counter == 3) {
          bin_full();
          break;
        }
      } else {
        break;
      }
    }
  }
}

float read_bin_distance() {
  return hc_bin.dist(0);
}

void bin_full() {
  green_led_off();
  uv_on();
  blue_led_on();
  delay(30000);
  uv_off();
  blue_led_off();
  red_led_on();

  wait_for_reset();
}

void wait_for_reset() {
  
  while(true) {
    
    int reset_status = digitalRead(reset_pin);
    
    if (!reset_status) {
      red_led_off();
      green_led_on();
      break;
    } 
  }
}

void open_lid_servo() {
  myservo.attach(lid_servo);
  for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  myservo.detach();
}

void close_lid_servo() {
  myservo.attach(lid_servo);
  for (pos = 100; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
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

void uv_on() {
  digitalWrite(uv_light_pin, HIGH);
}

void uv_off() {
  digitalWrite(uv_light_pin, LOW);
}
