import sys
import threading
import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    if rc != 0:
        print("MQTT connect status: " + str(rc), file=sys.stderr)
        return
    client.subscribe("uresponse")
    client.publish("urequest", "arq")

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
    client.user_data_set(data)
    client.username_pw_set("rhgxhrur", "y6DIhw4y0FRk")
    client.connect("m24.cloudmqtt.com", 16448, 60)

    client.loop_forever()
    timer.cancel()
    try:
        return data.pop()
    except IndexError:
        return 0
