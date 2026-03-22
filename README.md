# Gesture Controlled Car using ESP32

Built a real-time gesture-controlled robotic car using dual ESP32 boards and an MPU6050 sensor.

## Features
- Gesture-based control using accelerometer and gyroscope
- Wireless communication using ESP-NOW
- Real-time motor control using L298N driver
- Supports forward, backward, left, right, and stop movements

## Tech Stack
- ESP32
- MPU6050
- ESP-NOW protocol
- Arduino C++

## Demo
https://youtu.be/ekI203TfxyY

## How it Works
Hand movements are captured using the MPU6050 sensor and processed by the ESP32.  
The processed data is transmitted wirelessly via ESP-NOW to another ESP32, which controls the motors in real time.
