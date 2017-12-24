import paho.mqtt.client as mqtt
import time

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("SALUDAR")

def on_message(client, userdata, msg):
    print(time.strftime("%d-%m-%Y %H:%M:%S", time.localtime())+" "+msg.topic+" "+str(msg.payload))

client = mqtt.Client()
client.username_pw_set("usuario", password="clave") 
client.on_connect = on_connect
client.on_message = on_message

client.connect("server", 1883, 60)
client.publish("SALUDAR", "1")