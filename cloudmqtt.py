import logging
import sys
import threading
import paho.mqtt.client as mqtt
from enum import IntEnum

log = logging.getLogger('gunicorn.error')

class Color(IntEnum):
    red = 0
    blue = 1
    green = 2
    aqua = 3
    yellow = 4
    pink = 5
    white = 6
    black = 7
    gold = 8
    orange = 9
    magenta = 10
    maroon = 11
    violet = 12
    india = 13
    pakistan = 14
    rainbow = 15

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    logging.error(rc)
    logging.error(userdata)
    if rc != 0:
        print("MQTT connect status: " + str(rc), file=sys.stderr)
        return
    client.subscribe("uresponse")
    client.publish("urequest", userdata)

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    val = msg.payload.decode()
    print(msg.topic + " " + val)
    userdata.append(val)
    client.disconnect()
    
def on_timeout(client):
    print("Request timed out!", file=sys.stderr)
    client.disconnect()

def get_arq(timeout=10.0):
    client = mqtt.Client("uclient")
    client.on_connect = on_connect
    client.on_message = on_message
    
    timer = threading.Timer(timeout, on_timeout, [client])
    timer.start()

    data = list()
    client.user_data_set("arq")
    client.username_pw_set("rhgxhrur", "y6DIhw4y0FRk")
    client.connect("m24.cloudmqtt.com", 16448, 60)

    client.loop_forever()
    timer.cancel()
    try:
        return data.pop()
    except IndexError:
        return 0

def set_ledstrip_pattern(timeout=10.0, pattern="blue"):
    print("Led Pattern: ", pattern)
    logging.error("Led Pattern: ", pattern)
    client = mqtt.Client("uclient")
    client.on_connect = on_connect
    client.on_message = on_message

    timer = threading.Timer(timeout, on_timeout, [client])
    timer.start()

    try:
        client.user_data_set("ledstrip " + str(Color[pattern.lower()].value))
    except KeyError:
        client.user_data_set("ledstrip " + str(Color.black.value))
    client.username_pw_set("rhgxhrur", "y6DIhw4y0FRk")
    client.connect("m24.cloudmqtt.com", 16448, 60)

    client.loop_forever()
    timer.cancel()
    return 0