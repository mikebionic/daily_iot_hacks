String stream;
int led_indicator = 13;

void setup() {
  Serial.begin(115200);
  pinMode(led_indicator, OUTPUT);
  digitalWrite(led_indicator, 0);
}

void loop() {
  if (Serial.available() != 0){
    stream = Serial.readStringUntil('\n');
    stream.trim();
    if (stream == "hello"){
      digitalWrite(led_indicator, 1);
    }
  }
}