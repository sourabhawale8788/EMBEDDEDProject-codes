# Smart Environment Monitor - Raspberry Pi Pico W version

Same Firebase database and same dashboard as the ESP32 version. Only the board and `main.py` change.

## 1. Parts

| Part | Spec | Qty |
|---|---|---|
| Raspberry Pi Pico W **with soldered headers** (the "WH" version) | RP2040, 2.4 GHz Wi-Fi | 1 |
| DHT11 module (3-pin) | temp + humidity | 1 |
| BH1750 (GY-302) module | I2C light sensor, lux | 1 |
| LED 5mm + 220 ohm resistor | | 1 + 1 |
| Active buzzer module | 3.3-5V | 1 |
| Breadboard, jumper wires, micro-USB data cable | | |

A Pico W without headers will not fit a breadboard unless you solder pins to it. Check the price: the Pico W usually costs more than an ESP32 board, so confirm the Rs 1,000 hardware budget with your company.

## 2. Wiring (see `wiring_diagram_pico.png`)

Count physical pins from the USB end: left side 1-20 (top to bottom), right side 21-40 (bottom to top).

| Part | Part pin | Pico W pin |
|---|---|---|
| DHT11 | VCC / DATA / GND | 3V3(OUT) pin 36 / GP15 pin 20 / GND |
| BH1750 | VCC / GND | 3V3(OUT) pin 36 / GND |
| BH1750 | SDA / SCL | GP4 pin 6 / GP5 pin 7 |
| BH1750 | ADDR | leave unconnected |
| LED | long leg via 220 ohm | GP14 pin 19 |
| LED | short leg | GND |
| Buzzer module | VCC / I/O / GND | 3V3(OUT) pin 36 / GP13 pin 17 / GND |

GND pins are 3, 8, 13, 18, 23, 28, 33 and 38. Use the breadboard + rail for 3V3 and the - rail for GND.

## 3. Firebase (reuse your existing project)

Project: `smart-env-monitor-5b905`. No new project is needed.

1. Realtime Database > **Rules**: paste `{ "rules": { ".read": true, ".write": true } }` and **Publish**.
2. Project settings (gear) > General > **Your apps**. If there is no web app, click `</>` and register one named `dashboard`.
3. Copy `apiKey` and `appId` from the `firebaseConfig` shown there.
4. Open `dashboard/index.html` and replace `YOUR_API_KEY` and `YOUR_APP_ID`. The database URL and project ID are already filled in.

## 4. Put MicroPython on the Pico W (once)

1. Download the latest `.uf2` for **Raspberry Pi Pico W** from micropython.org/download/RPI_PICO_W/.
2. Hold the **BOOTSEL** button, plug in USB, then release. A drive named **RPI-RP2** appears.
3. Drag the `.uf2` file onto that drive. The board restarts by itself.
4. Install **Thonny** (thonny.org). Bottom-right corner: choose **MicroPython (Raspberry Pi Pico)**.

## 5. Upload the code

1. Open `main.py` in Thonny. Set `WIFI_SSID` and `WIFI_PASS` (2.4 GHz only; a phone hotspot works).
2. Click the green **Run** button. The Shell should show `Wi-Fi connected, IP: ...` and then a line like `T=28C H=60% L=0.0lux alert=False | live:200 history:200` every 10 seconds.
3. To make it start by itself: **File > Save as > Raspberry Pi Pico**, name it exactly `main.py`. Unplug and replug USB to test.

The small LED on the Pico blinks while connecting and stays on when Wi-Fi is up.

If you get `ImportError: no module named 'requests'`, update MicroPython (step 4) or run this in the Shell:

```python
import network, time, mip
w = network.WLAN(network.STA_IF); w.active(True); w.connect("YOUR_WIFI_NAME", "YOUR_WIFI_PASSWORD")
time.sleep(10); mip.install("requests")
```

## 6. Dashboard

Open `dashboard/index.html` with the VS Code **Live Server** extension. Optional public link: `firebase init hosting` (public directory `dashboard`, answer **No** if asked to overwrite `index.html`), then `firebase deploy --only hosting`.

## 7. Test checklist

- Status pill shows **Pico W online**; cards, chart and history table update every 10 seconds.
- LED button turns the real LED on/off within about 3 to 5 seconds.
- Set the threshold below room temperature and save: the buzzer sounds and the red banner shows.
- Unplug the Pico. After 30 seconds the pill shows **Pico W offline**.
- BH1750 (tomorrow): cover it with your hand and lux drops near 0; shine a torch and it rises.

## 8. Troubleshooting

- `live:-1`: the Pico cannot reach Firebase. Check Wi-Fi name/password and `DB_URL`.
- `live:401` or `403`: the rules in step 3.1 are not published.
- `DHT11 read failed: [Errno 110]`: check DHT11 wiring and pin order (S, +, -).
- `BH1750 not found`: check SDA/SCL are not swapped, headers are soldered, and VCC is on 3V3.
- Dashboard blank: press F12 in Chrome, open Console, look for a red error (usually `apiKey` or `appId`).
- Buzzer on all the time: it is active-LOW. Swap `BUZZER_ON` and `BUZZER_OFF` at the top of `main.py`.
- Every request takes 1 to 2 seconds on the Pico W because each one opens a secure connection. This is normal.
