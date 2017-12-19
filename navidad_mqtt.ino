#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "MusicaNavidad.h"


const char* ssid = "linksys";
const char* password = "perromateo";
const char* mqtt_server = "144.217.165.119";

const char* keyDevice = "NAVIDAD2";

#define pinBoton D5
#define pinBoton2 D6 //reset
#define pinLed D2
#define pinLed2 D1
#define parlante D8
int buttonState=0;

int nro = 1;

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
  pinMode(pinBoton, INPUT);
  pinMode(pinBoton2, INPUT); //reset
  pinMode(parlante,OUTPUT);
  buttonState= LOW;
  
  digitalWrite(pinLed,LOW);
  digitalWrite(pinLed2,LOW);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);  

}

void setup_wifi() {

  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
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
    navidad.tocarMusica(2);
    digitalWrite(pinLed,LOW);
  } else if ((char)payload[0] == '3') {
    digitalWrite(pinLed2,HIGH);
    navidad.tocarMusica(3);
    digitalWrite(pinLed2,LOW);
  }
  
  digitalWrite(pinLed,LOW);
  digitalWrite(pinLed2,LOW);

}

boolean notificando = false;

void reconnect() {

  while (!client.connected()) {

    if (client.connect(keyDevice,"guillermo","Moscardon01")) {

      //client.publish("casa", keyDevice);

      client.subscribe("SALUDAR");
    } else {
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
    delay(5000);
    //client.publish("casa", keyDevice);
  } 
  
  client.loop();

  if(digitalRead(pinBoton) == HIGH) {
    client.publish("SALUDAR",String(nro).c_str());   
  }

}



