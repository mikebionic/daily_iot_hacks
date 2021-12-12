#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

IPAddress staticIP(192, 168, 1, 209); //ESP static ip
IPAddress gateway(192, 168, 1, 1);   //IP Address of your WiFi Router (Gateway) 
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS

const char* ssid = "ssid";
const char* password = "12345678";
const char* deviceName = "IoT Device with setIP";
String device_key = "some_IoT_Hash";

// access point!!
#define AP_SSID "daily_iot"
#define AP_PASS "helloworld"
ESP8266WebServer server(80);


String current_temp = "27 *C";
String current_hum = "40 %";
String current_pir = "0";
String stream;

String stateDiv_open = "<div style=\"margin:0 auto; margin:1rem; background-color:yellow\">";
String temp_state_html = "<br><p>Temperature: " + current_temp + "</p>";
String hum_state_html = "<br><p>Humidity: " + current_hum + "</p>";
String pir_state_html = "<br><p>PIR: " + current_pir + "</p>";
String stateDiv_close = "</div>";

String states_html = stateDiv_open + temp_state_html + hum_state_html + pir_state_html + stateDiv_close;


String control_input_on = "<input type=\"hidden\" name=\"control_button_input\" value=\"fanon\"></br>";
String submit_input_on = "<input type=\"submit\" style=\"border-radius: 50px 50px 50px 50px;text-align:center;height:150px;font-size:50px;width:400px;color:white;background-color: #00A8A9\" value=\"On\">";

String control_input_off = "<input type=\"hidden\" name=\"control_button_input\" value=\"fanoff\"></br>";
String submit_input_off = "<input type=\"submit\" style=\"border-radius: 50px 50px 50px 50px;text-align:center;height:150px;font-size:50px;width:400px;color:white;background-color: #f15d3c\" value=\"Off\">";

String div_view = "<div>";
String form_view = "<form style=\"margin-top: 180px;display:flex;flex-direction: column;align-items: center;\" action=\"/control_the_iot\" method=\"POST\">";
String form_end_view = "</form>";
String div_end_view = "</div>";
String button_html_view = div_view + form_view + control_input_on + submit_input_on + form_end_view + form_view + control_input_off + submit_input_off + form_end_view + div_end_view;

String example_input_label = "<h3>Hey there, type the password to open the door!</h3> <br>";
String example_input = "<input style=\"height: 200px;width: 600px;font-size: 50px;\" type=\"text\" name=\"example_input\" placeholder=\"ID Key\"></br>";
String submit_example_input = "<input type=\"submit\" style=\"border-radius: 50px 50px 50px 50px;text-align:center;height:150px;font-size:50px;width:400px;color:white;background-color: #00A8A9\" value=\"Validate\">";
String example_input_form_view = "<form style=\"margin-top: 180px;display:flex;flex-direction: column;align-items: center;\" action=\"/access_control\" method=\"POST\">";
String exapmle_input_form_end_view = "</form>";
String inputSubmit_html = div_view + example_input_form_view + example_input_label + example_input + submit_example_input + exapmle_input_form_end_view + div_end_view;

String door_opening_secret_key = "qwerty";
String door_opening_action = "open_the_cracking_door_already";

String app_html_view = div_view + states_html + button_html_view + inputSubmit_html + div_end_view;


void handlePong() {
 server.send(200, "text/html", device_key);
}

void handleRootView(){
  server.send(200, "text/html", app_html_view);
}

void handleButtonFormSubmit(){
  if(!server.hasArg("control_button_input") || server.arg("control_button_input") == NULL) {
    server.send(200, "text/html", app_html_view);
    return;
  }

  send_data_to_slave(server.arg("control_button_input"));
  server.send(200, "text/html", app_html_view);
}


void handlePasswordFormSubmit(){
  if(!server.hasArg("example_input") || server.arg("example_input") == NULL) {
    server.send(200, "text/html", app_html_view);
    return;
  }

  if (server.arg("example_input") == door_opening_secret_key){
    send_data_to_slave(door_opening_action);
  }

  server.send(200, "text/html", app_html_view);
}


void send_data_to_slave(String data) {
  Serial.println(data);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.disconnect();
  WiFi.hostname(deviceName);
  WiFi.config(staticIP, subnet, gateway, dns);

  // access point!!
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASS);
  WiFi.begin(ssid, password);
  ////

  delay(500);
  server.on("/ping/", handlePong);
  server.on("/control_the_iot", handleButtonFormSubmit);
  server.on("/access_control", handlePasswordFormSubmit);
  server.on("/", handleRootView);
  server.begin();
}
 
void loop() {
  server.handleClient();

  if (Serial.available() != 0){
    stream = Serial.readStringUntil('\n');
    stream.trim();
    if (stream.length() > 0) {
      current_temp = getStringPartByDelimeter(stream, ':', 0);
      current_hum = getStringPartByDelimeter(stream, ':', 1);
      current_pir = getStringPartByDelimeter(stream, ':', 2);
    }
  }
}


String getStringPartByDelimeter(String data, char separator, int index) {
  int stringData = 0;
  String dataPart = "";
  for (int i = 0; i < data.length() - 1; i++) {
    if (data[i] == separator) {
      stringData++;
    } else if (stringData == index) {
      dataPart.concat(data[i]);
    } else if (stringData > index) {
      return dataPart;
      break;
    }
  }
  return dataPart;
}
