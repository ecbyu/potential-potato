#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ssid";
const char* password = "password";

String serverName = "http://192.168.1.9";

const int buttonPin = 0;
const int pin = 18;

int buttonState = 0;
int pinState = 0;

int prevButtonState = HIGH;
int prevPinState = -1;

int sendRequest(char*, char*);

void setup() {
  Serial.begin(9600); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  pinMode(pin, INPUT_PULLUP);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  pinState = digitalRead(pin);

  if ((prevButtonState == LOW && buttonState == HIGH) || (prevPinState == LOW && pinState == HIGH)) {
    Serial.println("LOW to HIGH");
    if(WiFi.status()== WL_CONNECTED){
      int r1 = sendRequest("/", "1=user&2=user");
      if (r1 > 199 && r1 < 300) {
        sendRequest("/ajax.xml", "282=20");
      }
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  } else if ((prevPinState == HIGH && pinState == LOW)) {
    Serial.println("HIGH to LOW");
    Serial.println("--------------------");
  }
  prevButtonState = buttonState;
  prevPinState = pinState;
}

int sendRequest(char* path, char* body) {
  HTTPClient http;
  String serverPath = serverName + path;
  http.begin(serverPath.c_str());
  Serial.print("HTTP request to: ");
  Serial.println(serverPath.c_str());
  Serial.print("Payload: ");
  Serial.println(body);
  int httpResponseCode = http.POST(body);
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    const char* payload = http.getString().c_str();
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  Serial.println("--------------------");
  return httpResponseCode;
}
