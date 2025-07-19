// =========================================================
// NodeMCU Car Control

// This code is designed to control a NodeMCU-based car
// with steering and throttle control via a web interface.

// It uses an ESP32 with a web server to handle
// commands from a web page.

// The car can be steered left or right, and it can move
// forward or backward with throttle control.

// The code also includes a simple HTML interface
// for user interaction.
// =========================================================

// =========================================================
// Libraries
// =========================================================
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <EEPROM.h>

#include "webpage.h"

// =========================================================
// Constants and Definitions
// =========================================================
// Throttle stop/min value
#define DAC_MIN_VALUE (255 * 0.8 / 3.3)

// control pins
#define PIN_THOTTLE 25
#define PIN_BRAKES 27
#define PIN_CAR_DIR 26

// steering pins
#define PIN_STEER_DIR 33  // Stepper DIR
#define PIN_STEP 32       // Stepper STEP

// EEPROM addresses
#define EEPROM_SIZE 512
#define EEPROM_INDICATOR_ADDR 0
#define EEPROM_POSITION_ADDR 1
#define EEPROM_INDICATOR_VALUE 0xA0

// =========================================================
// Enums for car direction and brakes
// =========================================================
// forward normally opened
enum {
  backward = 0,
  forward
};

// brakes off normally opened
enum {
  brakes_on = 0,
  brakes_off
};

// steering
enum {
  st_right = 0,
  st_left
};

// =========================================================
// Global Variables
// =========================================================
// Const variables:
const int max_steering_steps = 100;  // Limit to prevent oversteering
const int braking_time = 100;        // ms

int car_speed = DAC_MIN_VALUE;
signed char current_position = 0;        // Track current step position
int stepper_pulse_width = 3000;  // Pulse width in micro sec
bool auto_home_flag = 0;

// WIFI variables:
String dir = "S";  //String to store app command state.
const char* ssid = "Wifi Car";
const char* password = "12345678";
WebServer server(80);

// =========================================================
// Forward declarations for functions used before definition
// =========================================================
void goHome();
void goAhead();
void goBack();
void goLeft();
void goRight();
void stopRobot();
void setSpeed(int speed);
void setSteer(int speed);
void moveDirection(String dir);
void addToCurrentPosition(signed char val);

// =========================================================
// Setup Function
// =========================================================
void setup() {
  pinMode(PIN_THOTTLE, OUTPUT);
  pinMode(PIN_BRAKES, OUTPUT);
  pinMode(PIN_CAR_DIR, OUTPUT);
  pinMode(PIN_STEER_DIR, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);

  Serial.begin(115200);

  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  
  // Check if position was saved from previous run
  if (EEPROM.read(EEPROM_INDICATOR_ADDR) == EEPROM_INDICATOR_VALUE) {
    // Restore position from EEPROM (8-bit signed value)
    signed char saved_position = EEPROM.read(EEPROM_POSITION_ADDR);
    
    // Validate the restored position is within bounds
    if (saved_position >= -max_steering_steps && saved_position <= max_steering_steps) {
      current_position = saved_position;
      Serial.print("Restored position from EEPROM: ");
      Serial.println((int)current_position);
    } else {
      Serial.println("Invalid position in EEPROM, resetting to 0");
      current_position = 0;
      // Save the corrected position
      EEPROM.write(EEPROM_POSITION_ADDR, 0);
      EEPROM.commit();
    }
  } else {
    // First run or no saved position, set indicator and save current position
    EEPROM.write(EEPROM_INDICATOR_ADDR, EEPROM_INDICATOR_VALUE);
    EEPROM.write(EEPROM_POSITION_ADDR, (current_position & 0xFF));
    EEPROM.commit();
    Serial.println("First run - saved initial position to EEPROM");
  }

  // Connecting WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Serve the main page
  server.on("/", HTTP_GET, []() {
    Serial.println("Web request received for /");
    server.send(200, "text/html", htmlPage);
  });

  // Handle direction commands
  server.on("/cmd", []() {
    if (server.hasArg("dir")) {

      dir = server.arg("dir");

      Serial.print("Received command: ");
      Serial.println(dir);  // <-- Print the command sent

      server.send(200, "text/plain", "OK");

      moveDirection();


    } else if (server.hasArg("autoHome")) {

      String autoHome = server.arg("autoHome");

      if (autoHome == "1") {

        auto_home_flag = 1;

        Serial.println("Auto Go Home enabled");

        goHome();

      } else {

        auto_home_flag = 0;

        Serial.println("Auto Go Home disabled");
      }

      server.send(200, "text/plain", "OK");

    } else if (server.hasArg("manualHome")) {

      Serial.println("Manual Go Home triggered");

      goHome();

      server.send(200, "text/plain", "OK");

    } else {

      server.send(400, "text/plain", "Missing dir");
    }
  });

  // Handle speed commands
  server.on("/speed", []() {
    if (server.hasArg("car")) {

      int speed = server.arg("car").toInt();

      Serial.print("Received command: ");
      Serial.println(speed);  // <-- Print the command sent

      setSpeed(speed);

      server.send(200, "text/plain", "OK");

    } else if (server.hasArg("steer")) {

      int speed = server.arg("steer").toInt();

      Serial.print("Received command: ");
      Serial.println(speed);  // <-- Print the command sent

      setSteer(speed);

      server.send(200, "text/plain", "OK");

    } else {

      server.send(400, "text/plain", "Missing val");
    }
  });

  server.begin();
}

// =========================================================
// Loop Function
// =========================================================
void loop() {
  server.handleClient();
}

// =========================================================
// Core Motor Functions
// =========================================================
void stepMotor() {
  //move one step
  digitalWrite(PIN_STEP, HIGH);
  delayMicroseconds(stepper_pulse_width);
  digitalWrite(PIN_STEP, LOW);
  delayMicroseconds(stepper_pulse_width);
}

void setDirection(bool dir) {
  //Set direction
  digitalWrite(PIN_STEER_DIR, dir);
}

void goAhead() {
  //Release brakes
  digitalWrite(PIN_BRAKES, brakes_off);
  //Set direction
  digitalWrite(PIN_CAR_DIR, forward);
  //apply throttle
  dacWrite(PIN_THOTTLE, car_speed);
}

void goBack() {
  //Release brakes
  digitalWrite(PIN_BRAKES, brakes_off);
  //Set direction
  digitalWrite(PIN_CAR_DIR, backward);
  //apply throttle
  dacWrite(PIN_THOTTLE, car_speed);
}

void goLeft() {
  if (current_position < max_steering_steps) {
    stepMotor();
    addToCurrentPosition(1);
  }
}

void goRight() {
  if (abs(current_position - 1) <= max_steering_steps) {
    stepMotor();
    addToCurrentPosition(-1);
  }
}

void goHome() {
  //Check if Already at Home
  if (current_position == 0) {
    Serial.println("Already at Home");
    return;
  }

  // Set direction
  setDirection(current_position > 0 ? st_right : st_left);

  while ((dir == "F" || dir == "B" || dir == "S") && current_position != 0) {

    stepMotor();
    addToCurrentPosition(current_position > 0 ? -1 : 1);

    if (current_position == 0)
      Serial.println("Steering Reached Home");

    server.handleClient();
  }
}

void stopRobot() {
  // Release  throttle
  dacWrite(PIN_THOTTLE, DAC_MIN_VALUE);

  // Activate brakes
  digitalWrite(PIN_BRAKES, brakes_on);

  // Steering home
  if (auto_home_flag) {
    goHome();
  }
}

void moveDirection() {
  switch (dir[0]) {
    case 'F':
      {
        goAhead();
        stopRobot();
        goAhead();
        break;
      }
    case 'B':
      {
        goBack();
        stopRobot();
        goBack();
        break;
      }
    case 'S':
      {
        stopRobot();
        break;
      }
    case 'L':
      {
        setDirection(st_left);
        while (dir == "L") {
          goLeft();
          server.handleClient();
        }
        break;
      }
    case 'R':
      {
        setDirection(st_right);
        while (dir == "R") {
          goRight();
          server.handleClient();
        }
        break;
      }
    case 'I':
      {
        goAhead();
        setDirection(st_right);
        while (dir == "I") {
          goLeft();
          server.handleClient();
        }
        break;
      }
    case 'G':
      {
        goAhead();
        setDirection(st_left);
        while (dir == "G") {
          goRight();
          server.handleClient();
        }
        break;
      }
    case 'J':
      {
        goBack();
        setDirection(st_right);
        while (dir == "J") {
          goLeft();
          server.handleClient();
        }
        break;
      }
    case 'H':
      {
        goBack();
        setDirection(st_left);
        while (dir == "H") {
          goRight();
          server.handleClient();
        }
        break;
      }
  }
}

void setSpeed(int speed) {
  // map speed to min-255
  car_speed = map(speed, 0, 100, DAC_MIN_VALUE, 255);
  Serial.print("Car speed Volt: ");
  Serial.println(car_speed);
}

void setSteer(int speed) {
  // map speed to min-255
  stepper_pulse_width = map(speed, 1, 100, 10000, 1000);
  Serial.print("Steer speed delay us: ");
  Serial.println(stepper_pulse_width);
}

void addToCurrentPosition(signed char val) {
  current_position = current_position + val;

    
    // Save position to EEPROM (only if within bounds)
  EEPROM.write(EEPROM_POSITION_ADDR, (current_position & 0xFF));
  EEPROM.commit();
  
  Serial.print("Position changed: ");
  Serial.println((int)current_position);

}
