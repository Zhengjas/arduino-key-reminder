import serial
import requests

SERIAL_PORT = "COM3"   # change to your Arduino's port
BAUD_RATE = 9600
WEBHOOK_URL = "https://discord.com/api/webhooks/1490244874359013516/B0x_KiDFGpdopBzD9fUzH2t00pyfyWc_l3DZfWD8Tcd9dGklhuhPzPDXtwv0NN0LVTpj"       # paste your Discord webhook URL here

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
