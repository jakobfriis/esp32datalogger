#include "defines.h"
#include <WiFi.h>

void setup()
{
    Serial.begin(115200);
    delay(10);
    
    pinMode(led, OUTPUT);    
    pinMode(input0, INPUT);  
    
    // We start by connecting to a WiFi network

    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(5000);
}

void wifiReq(int sensorid, int val) {
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    String url = "/datalogger/storedata.php";
    url += "?unit=ff-ff-ff-22-33-44&sensor=";
    url += sensorid;
    url += "&value=";
    url += val;

    ledOn();
  
    Serial.print("Requesting URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    while(client.available()) {
        String line = client.readStringUntil('\r');
        //Serial.print(line);
    }

    ledOff();  
}

void loop()
{
    val0=digitalRead(input0);
    digitalWrite(led,val0);
    val3=analogRead(input3);
    Serial.print("Analog val: ");
    Serial.print(val3);
    Serial.print(" Digital val: ");
    Serial.println(val0);
    
    wifiReq(0,val0);
    delay(1000);
    wifiReq(1,val3);
    delay(10000);
}

void ledOn() {
      digitalWrite(led,HIGH);
}

void ledOff() {
      digitalWrite(led,LOW);
}

