#include <dht.h>

#define relay 13
#define dhtdat 8
int settedTemp = 25;
dht DHT;
long x=millis();

void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
}

void loop() {
  check_temp();
  lamp();
}

void lamp(){
  if (DHT.temperature <= settedTemp){
    digitalWrite(relay, 1);
  } 
  else if (DHT.temperature > settedTemp){
    digitalWrite(relay, 0);
  }
}

void check_temp(){
  if ((x+4000)<millis()){ 
    float readData = DHT.read11(dhtdat);
    unsigned int t=DHT.temperature;
    unsigned int h=DHT.humidity;
    Serial.print("Temp = ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("Humidity = ");
    Serial.print(h);
    Serial.println(" %");
    x=millis();
  }
}
