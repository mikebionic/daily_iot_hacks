String stream;
int fan_motor = 13;

#include <Servo.h>
Servo servo_motor;
int servo_pin = 4;
long servo_millis = millis();
int servo_delay = 1000;

String door_opening_action = "open_the_cracking_door_already";

long uart_message_millis = millis();
int uart_message_delay = 2000;

int current_temp = 28;
int current_hum = 37;
int current_pir = 0;


void setup() {
  Serial.begin(115200);
  pinMode(fan_motor, OUTPUT);
  digitalWrite(fan_motor, 0);

  servo_motor.attach(servo_pin);
  servo_motor.write(10);
}

void loop() {
  if (Serial.available() != 0){
    stream = Serial.readStringUntil('\n');
    stream.trim();
    if (stream.length() > 0) {
      control_electronics(stream);
    }
  }

  if (millis() - servo_millis > servo_delay){
    servo_motor.write(10);
  }
  synchronize_sensor_datas();
}

void synchronize_sensor_datas(){
  if (millis() - uart_message_millis > uart_message_delay){
    Serial.println(String(current_temp) + " *C:" + String(current_hum) + " %:" + String(current_pir) + ":");
    uart_message_millis = millis();
  }
}

void control_electronics(String action){
  if (action == "fanon"){
    digitalWrite(fan_motor, 1);
  }
  if (action == "fanoff"){
    digitalWrite(fan_motor, 0);
  }

  if (action == door_opening_action){
    open_door();
  }
}

void open_door(){
  servo_millis = millis();
  servo_motor.write(160);
}