# 🚀 ESP32 RS485 Sensor Emulator

![Alt text](https://github.com/Superbrida/TRANE_CH530_Emulator/blob/main/Multimedia/IMG_8722%203.JPG)

Welcome to the coolest ESP32 project on the block! 🎉
This project has been successfully tested for communication between a centrifugal chiller control panel Trane CH530 and a temperature sensor, using the LLID bus communication protocol over RS485.

It records data from the sensor for 1 minute, then emulates it for another minute, allowing you to download the recorded data via a web server. All in just 2 minutes – because who has time to wait? ⏱️

# 🏭 Supported Trane Chillers

The Tracer® CH530 controller is used in various Trane chillers, including:

📌 Centrifugal Chillers:
	•	CVGF – Multistage centrifugal chillers with direct drive compressors.
	•	CVGF Series R – High-efficiency centrifugal chillers.
	•	CDHF/CDHF+ – Large-capacity centrifugal chillers.
	•	RTCF – Centrifugal chillers with variable-speed control.

📌 Screw Chillers:
	•	RTWD/RTUD 060-250 – Water-cooled screw chillers.
	•	CGWN/CCUN 205-215 – Compact water-cooled chillers.
	•	RTAC – Air-cooled screw chillers for large capacities.

📌 Scroll Chillers:
	•	CGAN/CXAN 209-214 – Air-cooled scroll chillers.
	•	CGCL 200-600 – Modular scroll chillers.
	•	CGWH/CCUH 115-250 – Heat pump and cooling-only models.
	•	RAUL 190-800 – Large-capacity air-cooled chillers.

# 📦 What’s Inside?
	•	📡 RS485 Communication: Seamlessly talking to the Trane CH530 and sensor.
	•	💾 Data Recording: Capturing sensor data for 1 minute via LLID bus.
	•	🎭 Sensor Emulation: Mimicking the sensor behavior for analysis.
	•	🌐 Web Server: Download the recorded data straight from your browser.
	•	📶 Wi-Fi Connectivity: No cables, just pure convenience.

# 📚 Required Libraries

To compile and run this project on your ESP32, make sure to install the following libraries:

	1.	HardwareSerial – For serial communication with RS485 devices.
 
 	2.	FS (File System) – For file handling operations on SPIFFS.
  
  	3.	SPIFFS (SPI Flash File System) – Used to store and retrieve sensor data.
   
   	4.	WiFi – To connect the ESP32 to a wireless network.
    
    5.	WebServer – To host the web interface for downloading recorded data.
     

# 📥 Installation Instructions

To install these libraries, follow these steps:
	1.	Using the Arduino IDE:
	•	Go to Sketch > Include Library > Manage Libraries...
	•	Search for the required libraries (e.g., “SPIFFS”, “WiFi”, etc.)
	•	Click Install.

 
# 🛠️ Hardware Requirements

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

# ⚡ Quick Start Guide
	1.	Flash the code onto your ESP32 (you got this 💪).
	2.	Connect to Wi-Fi (update your SSID & password in the code).
	3.	Let it do its thing!
	•	First minute: Sensor data is being recorded.
	•	Second minute: Sensor emulation starts.
	4.	Download your data:
Open your browser and go to http://<ESP32-IP>/download to retrieve the logs.

# 🚦 Status LEDs (for those who love blinky things)
	•	🔴 Recording mode: ESP32 is listening to the sensor.
	•	🟡 Emulation mode: ESP32 is acting as the sensor.
	•	🟢 Ready: Web server is live and waiting for you.

# 🚀 Future Improvements
	•	Expand support for additional sensors and devices.
	•	Improve data visualization through a web interface.
	•	Optimize power consumption for standalone deployments.

# 🙌 Contributing

Feel free to open an issue or submit a PR – the more, the merrier! 🎉

🛠️ Made with love for ESP32 ❤️ – because it’s awesome.
Now go ahead and give it a try! 🚀
