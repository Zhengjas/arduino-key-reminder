import os
import serial
import requests
from dotenv import load_dotenv

load_dotenv()

SERIAL_PORT = "COM3"   # change to your Arduino's port
BAUD_RATE = 9600
WEBHOOK_URL = os.getenv("DISCORD_WEBHOOK_URL")

ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

print(f"Listening on {SERIAL_PORT}...")

last_state = None

while True:
    line = ser.readline().decode("utf-8").strip()
    if line:
        print(line)
    if line == "OPEN" and last_state != "OPEN":
        requests.post(WEBHOOK_URL, json={"content": "Door is open\nDon't forget to bring your keys and wallet!"})
        last_state = "OPEN"
    elif line == "CLOSED":
        last_state = None
