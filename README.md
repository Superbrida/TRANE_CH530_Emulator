##🚀 ESP32 RS485 Sensor Emulator

Welcome to the coolest ESP32 project on the block! 🎉
This project has been successfully tested for communication between a centrifugal chiller control panel Trane CH530 and a temperature sensor, using the LLID bus communication protocol over RS485.

It records data from the sensor for 1 minute, then emulates it for another minute, allowing you to download the recorded data via a web server. All in just 2 minutes – because who has time to wait? ⏱️

#📦 What’s Inside?
	•	📡 RS485 Communication: Seamlessly talking to the Trane CH530 and sensor.
	•	💾 Data Recording: Capturing sensor data for 1 minute via LLID bus.
	•	🎭 Sensor Emulation: Mimicking the sensor behavior for analysis.
	•	🌐 Web Server: Download the recorded data straight from your browser.
	•	📶 Wi-Fi Connectivity: No cables, just pure convenience.

#🛠️ Hardware Requirements

To get started, you’ll need the following hardware components:
	•	🧠 ESP32 (for the brains of the operation)
	•	🔌 2x MAX485 modules (to handle RS485 communication)
	•	🛠️ Wiring essentials (jumper wires, breadboard, power supply)

Wiring Configuration

ESP32 Pin	Connection
GPIO14	MAX485 #1 RO (Receive) – CH530 panel
GPIO27	MAX485 #1 DI (Transmit) – CH530 panel
GPIO25	MAX485 #2 RO (Receive) – Temperature Sensor
GPIO33	MAX485 #2 DI (Transmit) – Temperature Sensor

⚡ Quick Start Guide
	1.	Flash the code onto your ESP32 (you got this 💪).
	2.	Connect to Wi-Fi (update your SSID & password in the code).
	3.	Let it do its thing!
	•	First minute: Sensor data is being recorded.
	•	Second minute: Sensor emulation starts.
	4.	Download your data:
Open your browser and go to http://<ESP32-IP>/download to retrieve the logs.

#🚦 Status LEDs (for those who love blinky things)
	•	🔴 Recording mode: ESP32 is listening to the sensor.
	•	🟡 Emulation mode: ESP32 is acting as the sensor.
	•	🟢 Ready: Web server is live and waiting for you.

#🚀 Future Improvements
	•	Expand support for additional sensors and devices.
	•	Improve data visualization through a web interface.
	•	Optimize power consumption for standalone deployments.

#🙌 Contributing

Feel free to open an issue or submit a PR – the more, the merrier! 🎉

🛠️ Made with love for ESP32 ❤️ – because it’s awesome.
Now go ahead and give it a try! 🚀
