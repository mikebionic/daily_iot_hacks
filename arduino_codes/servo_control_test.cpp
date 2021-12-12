#include <Servo.h>
Servo servo_motor;
int servo_pin = 4;

void setup() {
  servo_motor.attach(servo_pin);
  servo_motor.write(10);
}

void loop(){
  run_servo();
}


void run_servo(){
  servo_motor.write(160);
  delay(2000);
  servo_motor.write(10);
  delay(2000);
}