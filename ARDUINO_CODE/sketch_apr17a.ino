#include <Wire.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ACS712XX.h>
char status;
WiFiClient client;


String apikey="Y979M1TXKR2ZBOU4";//THINGSPEAK API KEY
const char *ssid="OPPO Reno 10x Zoom"; //PLACE YOUR WIFI OR HOTSPOT ID
const char *pass="12345678"; //ENTER THE PASSWORD OF YOUR WIFI OR HOTSPOT
const char *server="api.thingspeak.com";
unsigned long interval=300000;
unsigned long previousMillis=0;
float E=0.0;
ACS712XX current_sensor(ACS712_30A, A0);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  current_sensor.autoCalibrate();
  Wire.begin();
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

}
void loop() {
  float V=220.0;
  float I = current_sensor.getAC();
// ASSUMING ALL LOADS ARE RESISTIVE HENCE PHASE-DIFFRENCE IS CONSIDERD AS ZERO.
  float P=V*I; //POWER CONSUMED
  unsigned long currentMillis = millis();
  if (client.connect(server, 80))
  {
    String postStr = apikey;
    postStr += "&field1=";
    postStr += String(V);
    postStr += "&field2=";
    postStr += String(I);
    postStr += "&field3=";
    postStr += String(P);
    if (currentMillis - previousMillis == interval) 
     {

            postStr += "&field4=";
            postStr += String(E);
        previousMillis=currentMillis;
        E=0.0;
     }
    else{
    E=E+P*0.1;
    }

    postStr += "\r\n\r\n\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apikey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n\n\n");
    client.print(postStr);
    Serial.print("Voltage(RMS): ");
    Serial.println(V);
    Serial.print("Curent(RMS): ");
    Serial.println(I);
    Serial.print("Power: ");
    Serial.println(P);
    Serial.print("Energy:");
    Serial.println(E);
  }
  client.stop();

  delay(100);
    
}
