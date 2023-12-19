from flask import Flask, render_template
from flask_socketio import SocketIO, emit
import pandas as pd
import matplotlib.pyplot as plt
import base64
import io
import pymysql
import paho.mqtt.client as mqtt

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

# MQTT broker 설정
mqtt_broker = 'localhost'
mqtt_topic = 'temperature_data'

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    # 연결이 성공하면 구독 시작
    client.subscribe(mqtt_topic)

def on_message(client, userdata, msg):
    # 메시지 수신 시 동작
    print(f"Received message: {msg.payload.decode()}")
    

def fetch_data():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(mqtt_broker, 1883, 60)
    client.loop()
    return pd.DataFrame(data)

@app.route('/')
def index():
    return render_template('index_refresh2.html')

@app.route('/data')
def visualize_data():
    while True:
        data = fetch_data().to_dict(orient='records')
        socketio.emit('update_data', data)
        socketio.sleep(1)  # Update every 1 second


if __name__ == "__main__":
    socketio.run(app)
    # MQTT 클라이언트 생성
    client = mqtt.Client()

    # 연결 및 메시지 콜백 설정
    client.on_connect = on_connect
    client.on_message = on_message

    # MQTT broker에 연결
    client.connect(mqtt_broker, 1883, 60)

    # 루프 시작 (Ctrl+C로 종료)
    client.loop_forever()