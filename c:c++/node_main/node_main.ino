#include <ESP8266WiFi.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_AM2315.h>

//define valve control
#define VALVE1 D5
#define VALVE2 D6
#define VALVE3 D7
#define VALVE4 D8


BH1750 lightMeter(0x23);
Adafruit_AM2315 am2315;

//setting WiFi Connecting
const char* ssid = "Smartfarm_Sansara@9";
const char* password = "kmutnbsansara";

//const char* ssid = "xp_atz";
//const char* password = "11111111";

//setting host connection
const char* host = "35.198.211.170";
String url = "";

//setting relay pin


String status_text = "";

void setup()
{
  pinMode(D5, OUTPUT);     // Initialize the VALVE pin as an output
  pinMode(D6, OUTPUT);     // Initialize the VALVE pin as an output
  pinMode(D7, OUTPUT);     // Initialize the VALVE pin as an output
  pinMode(D8, OUTPUT);     // Initialize the VALVE pin as an output
  Wire.begin();


  Serial.begin(9600);
  connectWifi(ssid, password );

  lightMeter.begin(BH1750_CONTINUOUS_HIGH_RES_MODE);
  Serial.println(F("BH1750 Test"));

  if (! am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
    while (1);
  }
}
void loop() {

  Serial.print("Light: ");
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print(lux);
  Serial.println(" lx");
  //send brightness
  url = "/api/insert/brightness/1/1/";
  url += lux;

  send_data(host, url);
  
  
  uint16_t humid = am2315.readHumidity();
  //send air_humidity
  url = "/api/insert/air_humidity/1/1/";
  url += humid;

  send_data(host, url);
  

  uint16_t temp = am2315.readTemperature();
  //send air_temperature
  url = "/api/insert/air_temperature/1/1/";
  url += temp;

  send_data(host, url);
  delay(30000);

  
  url = "/api/get/valve_status/1/1";

  //http://35.198.211.170/api/get/valve_status/1/1
  checking_status(host, url);

  delay(10000);
}
void connectWifi(const char* ssid, const char* password) {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {

    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connect");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void checking_WiFi() {
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("reconnect to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
  }
  Serial.println("WiFi currenly connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void send_data(const char* host, String url) {
  Serial.print("connecting to ");
  Serial.println(host);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  // We now create a URI for the request
  //  String url = "/insert/brightness/";
  //    url += data;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
    int timeout = millis() + 5000;
    while (client.available() == 0) {
      if (timeout - millis() < 0) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
  while (client.available()) {
    String line = client.readStringUntil('\r');
        Serial.println(line);
    if (line.startsWith("HTTP/1.1 200 OK")) {
      Serial.println("send successfull!");

      break;
    }
  }
  Serial.println();
  Serial.println("closing connection");
}

String checking_status(const char* host, String url) {
  String status_text = "";
  Serial.print("connecting to ");
  Serial.println(host);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    
  }

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sended");
    int timeout = millis() + 5000;
    while (client.available() == 0) {
      if (timeout - millis() < 0) {
        Serial.println(">>> Client Timeout !");
        client.stop();
      }
    }
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.println(line);
    if (line.startsWith("ON")) {
      Serial.println("ON state");
      digitalWrite(D5, LOW);
      //      break;
    } else if (line.startsWith("OFF")) {
      Serial.println("OFF state");
      digitalWrite(D5, HIGH);
      //      break;
    }
  }
  Serial.println();
  Serial.println("closing connection");
}

//void relay(String status, int pin) {
//  if (status == "ON") {
//    Serial.println("turn on");
//    digitalWrite(pin, LOW);
//    delay(10000);
//  } else if (status == "OFF") {
//    Serial.println("turn off");
//    digitalWrite(pin, HIGH);
//    delay(10000);
//  }
//}
