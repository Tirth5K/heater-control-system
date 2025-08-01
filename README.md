# Heater Control System (Arduino + DS18B20)

A basic heater control system built using Arduino Uno, DS18B20 temperature sensor, and simulated heater/buzzer. The system supports multiple states and handles overheat shutdown logic.

 🚀 Features

- Continuous temperature monitoring using DS18B20
- State machine:
  - IDLE
  - HEATING
  - STABILIZING
  - TARGET_REACHED
  - OVERHEAT
- Heater ON/OFF control
- Buzzer alert on overheat
- Auto recovery after cooling
- Serial monitor logging

🧰 Components Used

- Arduino Uno
- DS18B20 temperature sensor
- 4.7kΩ resistor (pull-up)
- LED to simulate heater
- Buzzer (or LED) for overheat alert

🧪 Simulation

Try it on Wokwi 👉 https://wokwi.com/projects/438107402887703553

👤 Author

Tirthankar Karmakar


