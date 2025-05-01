
# Arduino Line Following Robot with Encoders and Ultrasonic Sensor

This project controls a robot that follows a black line using five IR sensors, and uses encoders to track distance and rotation. While an ultrasonic sensor is connected, obstacle detection is **not yet implemented** in the movement logic.

## 🛠 Hardware Components

- Arduino Uno
- 2 DC Motors with motor driver (L298N or similar)
- 2 FC-03 Optical Speed Encoders (connected to pins 2 and 0)
- 5 IR Line Sensors (connected to pins 8, 13, 10, 12, 11)
- Ultrasonic Sensor (TRIG: A0, ECHO: A1)
- Power supply

## ⚙️ Pin Configuration

### Motor Pins
- ENA: D3
- IN1: D4
- IN2: D5
- ENB: D9
- IN3: D6
- IN4: D7

### Encoder Pins
- Encoder 1: D2 (interrupt pin)
- Encoder 2: D0

### Ultrasonic Sensor
- TRIG: A0
- ECHO: A1

### Line Sensors (IR)
- Sensor 1: D8
- Sensor 2: D13
- Sensor 3: D10
- Sensor 4: D12
- Sensor 5: D11

## 📏 Distance Calculation

- `pulsesPerRev = 85`
- `wheelCircumference = 21.0 cm`
- `pulsesPerCM = pulsesPerRev / wheelCircumference`

## 🚦 Functionality

### ✅ Line Following

- Reads values from 5 IR sensors
- Makes decisions to move forward, turn left/right, or stop based on the line position
- Uses PWM to control motor speed

### ✅ Encoder Count

- Uses external interrupts to count pulses from both encoders for distance and rotation tracking

### ⚠️ Not Yet Implemented

#### ❌ Obstacle Detection

- Function `objectDistance()` is defined using the ultrasonic sensor, but it is **not integrated** in the movement logic.

You can add it inside `lineFollowing()` like this:

```cpp
if (objectDistance() < 10) {
  stopMotors();
  delay(1000); // pause or reroute
}
```

## 📂 Structure

- `setup()`: Initializes all pins and interrupts
- `loop()`: Calls `lineFollowing()` in a continuous loop
- `lineFollowing()`: Handles all motor actions based on IR sensor readings
- `encoder1ISR()` & `encoder2ISR()`: Count encoder pulses via interrupts

## 🧠 Notes

- Black line = `LOW` (sensor sees black)
- White surface = `HIGH` (sensor sees white)
- Using pin D0 (Encoder 2) may interfere with serial monitoring
