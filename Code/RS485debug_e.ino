/*
MIT License

Copyright (c) 2024 [Fabio Bridarolli]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/



// This sketch allows connecting CH530 and a sensor between two RS485 interfaces
// The first minute, it forwards data from the sensor and records it,
// then it emulates the sensor for one minute by creating a web server
// where the received data can be downloaded.

#include <HardwareSerial.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <WebServer.h>

// Definition of UARTs and pins
#define UART1_RX 14
#define UART1_TX 27
#define UART2_RX 25
#define UART2_TX 33

// Wi-Fi credentials
const char* ssid = "FASTWEB-d9BZe9";
const char* password = "Jn8s3SU7wm";

HardwareSerial RS485Master(1); // UART1
HardwareSerial RS485Sensor(2); // UART2

const int baudRate = 19200; // Transmission speed
const int bufferSize = 256; // Buffer size (unchanged)
const unsigned long recordingDuration = 60000; // Recording duration in milliseconds (1 minute)
const unsigned long emulationDuration = 60000; // Emulation duration in milliseconds (1 minute)
const unsigned long wifiTimeout = 20000; // Wi-Fi connection timeout in milliseconds (20 seconds)
const int maxWifiRetries = 3; // Maximum number of Wi-Fi connection attempts
const int emulationDelay = 50; // Uniform delay between each data transmission during emulation
unsigned long startTime;
unsigned long emulationStartTime;
bool recording = true;
bool emulationCompleted = false;

struct Message {
  byte data[bufferSize];
  int length;
  unsigned long timestamp;
};

Message recordedMessages[100]; // Array to store messages (reduced to 100 messages)
int messageCount = 0;

WebServer server(80);

void setup() {
  // Initialize serial ports
  RS485Master.begin(baudRate, SERIAL_8N1, UART1_RX, UART1_TX); // RX=GPIO14, TX=GPIO27
  RS485Sensor.begin(baudRate, SERIAL_8N1, UART2_RX, UART2_TX); // RX=GPIO25, TX=UART2_TX
  
  // Initialize Serial for debugging
  Serial.begin(115200);
  while (!Serial) {}

  // Initialize SPIFFS with formatting if mounting fails
  if (!SPIFFS.begin(false)) {
    Serial.println("SPIFFS Mount Failed");
    // Attempt to format
    if (!SPIFFS.begin(true)) {
      Serial.println("SPIFFS Format Failed");
      while (true) {
        delay(1000);
      }
    } else {
      Serial.println("SPIFFS successfully mounted after formatting");
    }
  } else {
    Serial.println("SPIFFS successfully mounted");
  }

  // Wi-Fi connection
  int wifiRetries = 0;
  while (WiFi.status() != WL_CONNECTED && wifiRetries < maxWifiRetries) {
    WiFi.begin(ssid, password);
    unsigned long wifiStartTime = millis();
    Serial.print("Wi-Fi connection attempt ");
    Serial.println(wifiRetries + 1);
    while (WiFi.status() != WL_CONNECTED && millis() - wifiStartTime < wifiTimeout) {
      delay(500);
      Serial.print(".");
    }
    wifiRetries++;
    Serial.println();
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection failed after several attempts, restarting...");
    ESP.restart();
  } else {
    Serial.println("Connected to Wi-Fi network");
  }

  // Initialize web server
  server.on("/", handleRoot);
  server.on("/download", handleDownload);
  server.begin();
  Serial.println("Web Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Initialization complete");
  startTime = millis(); // Store start time
}

void loop() {
  if (!emulationCompleted) {
    if (recording) {
      // Record data for 1 minute
      handleCommunication(RS485Master, RS485Sensor, "Master to Sensor");
      handleCommunication(RS485Sensor, RS485Master, "Sensor to Master");
      
      if (millis() - startTime > recordingDuration) {
        recording = false;
        emulationStartTime = millis();
        Serial.println("Recording completed. Starting sensor emulation.");
      }
    } else {
      // Emulate the sensor
      emulateSensor();
      
      if (millis() - emulationStartTime > emulationDuration) {
        emulationCompleted = true;
        Serial.println("Emulation completed after 1 minute.");
        Serial.println("Data available on web server at: ");
        Serial.print("http://");
        Serial.print(WiFi.localIP());
        Serial.println("/download");

        while (true) {
          // Stop loop execution
          delay(1000);
        }
      }
    }
    
    delay(5); // Pause to prevent an overly fast loop
    server.handleClient();
  }
}

void handleCommunication(HardwareSerial &source, HardwareSerial &destination, const char* direction) {
  if (source.available()) {
    Serial.print("Receiving data: ");
    Serial.println(direction);
    byte message[bufferSize]; // Buffer to store the message
    int index = 0;

    // Read available data
    while (source.available() && index < bufferSize) {
      byte data = source.read();
      message[index++] = data;
    }

    // Store data if direction is "Sensor to Master"
    if (direction == "Sensor to Master" && messageCount < 100) {
      memcpy(recordedMessages[messageCount].data, message, index);
      recordedMessages[messageCount].length = index;
      recordedMessages[messageCount].timestamp = millis();
      messageCount++;
    }

    // Print received data in byte format
    Serial.print("Message in bytes: ");
    for (int i = 0; i < index; i++) {
      Serial.print(message[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // Send data to the recipient
    int bytesWritten = destination.write(message, index);
    destination.flush();

    Serial.print("Data sent: ");
    Serial.print(direction == "Master to Sensor" ? "to Sensor" : "to Master");
    Serial.print(": ");
    Serial.print(bytesWritten);
    Serial.println(" bytes");

    // Add a small delay to allow the recipient time to respond
    delay(8);
  }
}

void emulateSensor() {
  static int currentMessage = 0;
  if (currentMessage < messageCount) {
    // Send stored data to master
    int bytesWritten = RS485Master.write(recordedMessages[currentMessage].data, recordedMessages[currentMessage].length);
    RS485Master.flush();

    // Save sent data to SPIFFS
    File file = SPIFFS.open("/emulated_data.txt", FILE_APPEND);
    if (!file) {
      Serial.println("Error opening file");
      return;
    }

    file.print("Timestamp: ");
    file.print(recordedMessages[currentMessage].timestamp);
    file.print(" ms, Data: ");
    for (int i = 0; i < recordedMessages[currentMessage].length; i++) {
      file.print(recordedMessages[currentMessage].data[i], HEX);
      file.print(" ");
    }
    file.println();
    file.close();

    Serial.print("Emulated data sent to master: ");
    Serial.print(bytesWritten);
    Serial.print(" bytes, Timestamp: ");
    Serial.print(recordedMessages[currentMessage].timestamp);
    Serial.println(" ms");

    currentMessage++;
    delay(emulationDelay); // Add uniform delay between emulated messages
  }
}

void handleRoot() {
  Serial.println("GET request /");
  server.send(200, "text/plain", "Welcome to ESP32 Web Server");
}

void handleDownload() {
  Serial.println("GET request /download");
  File file = SPIFFS.open("/emulated_data.txt", FILE_READ);
  if (!file) {
    server.send(404, "text/plain", "File not found");
    return;
  }
  server.streamFile(file, "application/octet-stream");
  file.close();
}
