#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "MusicaNavidad.h"

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h> 

const char* ssid = "wifi";
const char* password = "clave";
const char* mqtt_server = "server";

const char* keyDevice = "NAVIDAD4";

#define pinBoton D5
#define pinBoton2 D6 //reset
#define pinLed D2
#define pinLed2 D1

#define pinLedEstrella D4

#define parlante D8
int buttonState=0;

int nro = 2;

MusicaNavidad navidad;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {

  Serial.begin(9600);
  
  pinMode(pinLed, OUTPUT);
  pinMode(pinLed2, OUTPUT);

  pinMode(pinLedEstrella, OUTPUT);
  
  pinMode(pinBoton, INPUT);
  pinMode(pinBoton2, INPUT); //reset
  pinMode(parlante,OUTPUT);
  buttonState = LOW;
  
  digitalWrite(pinLed,LOW);
  digitalWrite(pinLed2,LOW);
  
  digitalWrite(pinLedEstrella,LOW);

 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);  

}

void setup_wifi() {

  delay(10);
  WiFi.begin(ssid, password);
  
  digitalWrite(pinLedEstrella,LOW);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  digitalWrite(pinLedEstrella,HIGH);
}




void callback(char* topic, byte* payload, unsigned int length) {

  if (((char)payload[0] == '1')) {   

    digitalWrite(pinLed,HIGH);
    digitalWrite(pinLed2,HIGH);
    
    navidad.tocarMusica(1);    

    digitalWrite(pinLed,LOW);
    digitalWrite(pinLed2,LOW); 
    
  } else if ((char)payload[0] == '2') {

    digitalWrite(pinLed,HIGH);
    digitalWrite(pinLed2,HIGH);
    
    navidad.tocarMusica(2);

    digitalWrite(pinLed,LOW);
    digitalWrite(pinLed2,LOW); 
    
  } else if ((char)payload[0] == '3') {

    digitalWrite(pinLed,HIGH);
    digitalWrite(pinLed2,HIGH);
    
    navidad.tocarMusica(3);

    digitalWrite(pinLed,LOW);
    digitalWrite(pinLed2,LOW);    
  }
  digitalWrite(pinLed,LOW);
  digitalWrite(pinLed2,LOW);

}

boolean notificando = false;

void reconnect() {

  while (!client.connected()) {
    
    if (client.connect(keyDevice,"usuario","clave")) {

      //client.publish("casa", keyDevice);

      client.subscribe("SALUDAR");
      
    } else {
      delay(5000);
    }
  }
}

long interval = 5000; //5 segundos
long previousMillis = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
    delay(5000);
    //client.publish("casa", keyDevice);
  } 
  
  client.loop();

  unsigned long currentMillis = millis();
  if ( digitalRead(pinBoton2) == HIGH ) {
    
    if(currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      
      digitalWrite(pinLedEstrella,LOW);
      delay(100);
      digitalWrite(pinLedEstrella,HIGH);
      delay(100);
      digitalWrite(pinLedEstrella,LOW);
      delay(100);      
      digitalWrite(pinLedEstrella,HIGH);
     
      
      WiFiManager wifiManager;
      wifiManager.resetSettings();
      wifiManager.setTimeout(120);
      if (!wifiManager.startConfigPortal("OnDemandAP")) {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        //reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(5000);
      }
    }
  }

  if(digitalRead(pinBoton) == HIGH) {
    client.publish("SALUDAR",String(nro).c_str());   
  }

}



