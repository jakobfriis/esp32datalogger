#include <WiFi.h>
#define input 4
#define analogIn 34
#define led 2

const char* ssid     = "ASUS";
const char* password = "76TMQEGEWD";
int val=0;
int analogval=0;
const char* host = "jdata.dk";

void setup()
{
    Serial.begin(115200);
    delay(10);
    
    pinMode(led, OUTPUT);    
    digitalWrite(led,HIGH);
    pinMode(input, INPUT);  
    
    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(5000);
}

void wifiReq(int sensorid, int val) {
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
    String url = "/datalogger/storedata.php";
    url += "?unit=ff-ff-11-22-33-44&sensor=";
    url += sensorid;
    url += "&value=";
    url += val;

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
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

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        //Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");  
}

void loop()
{
    val=digitalRead(input);
    digitalWrite(led,val);
    analogval=analogRead(analogIn);
    Serial.print("Analog val: ");
    Serial.print(analogval);
    Serial.print(" Digital val: ");
    Serial.println(val);
    
    wifiReq(0,val);
    delay(1000);
    wifiReq(1,analogval);
    delay(10000);
}
