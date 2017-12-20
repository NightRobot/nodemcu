//Adafruit_AM2315 am2315;
#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTPIN 14     // what digital pin the DHT22 is conected to D5
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors

//setting WiFi Connecting
const char* ssid = "Smartfarm_Sansara@9";
const char* password = "kmutnbsansara";

//const char* ssid = "xp_atz";
//const char* password = "11111111";

DHT dht(DHTPIN, DHTTYPE);

const char* host = "35.198.211.170";
String url = "";


void setup() {
  Serial.begin(9600);


  // Set WiFi to station mode and disconnect from an AP if it was previously connected
//  WiFi.mode(WIFI_STA);
//  WiFi.disconnect();

  Serial.println("Setup done");
//  scan_Wifi();
  connectWifi(ssid, password );
  //  if (! am2315.begin()) {
  //    Serial.println("Sensor not found, check wiring & pullups!");
  //    while (1);
  //  }

}

void loop() {
//  scan_Wifi();
  checking_WiFi();
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  //DHT22 - test
  Serial.print("DHT22 - test ");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  //send moisture
  url = "/api/insert/soil_moisture/1/1/";
  url += h;

  send_data(host, url);


  //send temp
  url = "/api/insert/soil_temperature/1/1/";
  url += t;

  send_data(host, url);
  //am2315 - test
  //    Serial.print("AM2315 - test ");
  //    Serial.print("Humidity: ");
  //    Serial.print(am2315.readHumidity());
  //    Serial.print(" %\t");
  //    Serial.print("Temperature: ");
  //    Serial.print(am2315.readTemperature());
  //    Serial.println(" *C ");
  delay(30000);
}
void scan_Wifi() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
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
  Serial.println("WiFi already connected to ");
  Serial.println(ssid);
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
  Serial.println("request sended");
//  int timeout = millis() + 5000;
//  while (client.available() == 0) {
//    if (timeout - millis() < 0) {
//      Serial.println(">>> Client Timeout !");
//      client.stop();
//      return;
//    }
//  }
//  while (client.connected()) {
//    String line = client.readStringUntil('\n');
//    if (line == "\r") {
//      Serial.println("headers received");
//      break;
//    }
//  }
  while (client.available()) {
    String line = client.readStringUntil('\r');
//    Serial.println(line);
    if (line.startsWith("HTTP/1.1 200 OK")) {
      Serial.println("send successfull!");

      break;
    }
  }
  Serial.println();
  Serial.println("closing connection");
}


