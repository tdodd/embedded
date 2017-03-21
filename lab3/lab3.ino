/******************************************************************************
 *
 * Names:        Thomas Dodd 6822414
 *               Peng Liu 4229132
 * Course Code:  SEG 4145
 * Lab Number:   2
 * File name:    lab2.ino
 * Date:         February 1st, 2017
 *
 * Description
 * *************
 * This file contains all functions for lab 2.
 ******************************************************************************
 */

/**
 * Libraries
 */
#include <SoftwareSerial.h>
#include <Wire.h>

/**
 * Global pin values.
 */
const int LED_PIN = 13;
const int LCD_PIN = 18;
const int RIGHT_MOTOR_PIN = 45;
const int LEFT_MOTOR_PIN = 8;
const int SONAR_PIN = 22;
const int reg = 0x01;

#define TEMPSENSOR 0x68 //Address

/**
 * Forward and backward movement
 */
const int LEFT_MOTOR_BACKWARD_PULSE = 200;
const int RIGHT_MOTOR_BACKWARD_PULSE = 100;
const int LEFT_MOTOR_FORWARD_PULSE = -100;
const int RIGHT_MOTOR_FORWARD_PULSE = 500;

/**
 * Student numbers.
 */
const char THOMAS_NUM[] = "6822414";
const char PENG_NUM[] = "4229132";

/**
 * Timing constants.
 */
const int FULL_TURN_MILLI = 3500;
const int TILE_DISTANCE = 1500;

/**
 * Instances
 */
SoftwareSerial LCD(0, LCD_PIN);

/**
 * printToFirst, Prints centered text on the 1st line of the LCD display.
 * 
 * @param *LCD, a pointer to the LCD screen.
 * @param str, the string to display.
 */
void printToFirst(char str[]) {
  LCD.write(0xFE);
  LCD.write(128 + getOffset(strlen(str)));
  LCD.print(str);
}

/**
 * printToSecond, Prints centered text on the 2nd line of the LCD display.
 * 
 * @param *LCD, a pointer to the LCD screen.
 * @param str, the string to display.
 */
void printToSecond(char str[]) {
  LCD.write(0xFE);
  LCD.write(64 + 128 + getOffset(strlen(str)));
  LCD.print(str);
}

/**
 * getOffset, Gets the required offset to center text on the LCD.
 * 
 * @param str_size, the length of the string.
 * @return, the offset.
 */
int getOffset(int str_size) {
  return (16 - str_size) / 2;
}

/**
 * clearLcd, Clears the LCD screen.
 */
void clearLcd() {
  LCD.write(0xFE);
  LCD.write(0x01);
  delay(10);  
}

/**
 * blinkLed, Blinks the led a specified number of times.
 * @param num, the number of times to blink the led.
 */
void blinkLed(int num) {
  for (int i = 0; i < num; i ++) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(800);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(200);
  }
}

/**
 * moveForward, Moves the robot forward for a specified amount of time and prints action to LCD.
 * @param milli_sec, the time in milliseconds for the wheels to turn.
 * @param *LCD, a pointer to the LCD screen.
 */
void moveForward(int milli_sec) {
  clearLcd();
  printToFirst("Moving");
  printToSecond("Forward");
  analogWrite(RIGHT_MOTOR_PIN, RIGHT_MOTOR_FORWARD_PULSE);
  analogWrite(LEFT_MOTOR_PIN, LEFT_MOTOR_FORWARD_PULSE);
  delay(milli_sec);
  stop();
}

/**
 * reverse, Reverses the robot for a specified of time and prints action to LCD.
 * @param milli_sec, the time in milliseconds for the wheels to turn.
 */
void reverse(int milli_sec) {
  clearLcd();
  printToFirst("Moving");
  printToSecond("Backwards");
  analogWrite(RIGHT_MOTOR_PIN, RIGHT_MOTOR_BACKWARD_PULSE);
  analogWrite(LEFT_MOTOR_PIN, LEFT_MOTOR_BACKWARD_PULSE);
  delay(milli_sec);
  stop();
}

/**
 * turnLeft, Turns the robot left and prints action to LCD.
 * 
 * @param increment, the # of increments of 45 degrees to turn.
 * @param *LCD, a pointer to the LCD screen.
 */
void turnRight(int increment) {
  clearLcd();
  printToFirst("Rotating");
  printToSecond("Right");
  analogWrite(RIGHT_MOTOR_PIN, RIGHT_MOTOR_FORWARD_PULSE);
  analogWrite(LEFT_MOTOR_PIN, LEFT_MOTOR_BACKWARD_PULSE);
  delay(FULL_TURN_MILLI * increment/8);
  stop();
}

/**
 * turnRight, Turns the robot right and prints action to LCD.
 *
 * @param increment, the # of increments of 45 degrees to turn.
 * @param *LCD, a pointer to the LCD screen.
 */
void turnLeft(int increment) {
  clearLcd();
  printToFirst("Rotating");
  printToSecond("left");
  analogWrite(RIGHT_MOTOR_PIN, RIGHT_MOTOR_BACKWARD_PULSE);
  analogWrite(LEFT_MOTOR_PIN, LEFT_MOTOR_FORWARD_PULSE);
  delay((FULL_TURN_MILLI - 350) * increment/8);
  stop();
}

/**
 * stop, Stops the current motion and prints action to LCD.
 * @param *LCD, a pointer to the LCD screen.
 */
void stop() {
  clearLcd();
  printToFirst("Stopping");
  analogWrite(RIGHT_MOTOR_PIN, 0);
  analogWrite(LEFT_MOTOR_PIN, 0);
}

/**
 * Pre-programmed collision avoidance algorithm.
 */
void avoidCollision() {
  printTemperature();
  delay(3000);
  reverse(TILE_DISTANCE);
  turnRight(2);
  moveForward(TILE_DISTANCE * 2);
  turnLeft(2);
}

/**
 * ping, sends a ping of ultrasound and calculates distance to an object.
 * 
 * @param
 */
int ping() {
  pinMode(SONAR_PIN, OUTPUT);
  digitalWrite(SONAR_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SONAR_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(SONAR_PIN, LOW);
  pinMode(SONAR_PIN, INPUT);

  pinMode(SONAR_PIN, INPUT);
  int duration = pulseIn(SONAR_PIN, HIGH);
  
  int inches = duration / 74 / 2;
  int cm = duration / 29 / 2;

  return cm;
}

/**
 * Print the current ambient temperature.
 */
int printTemperature() {
  Wire.beginTransmission(TEMPSENSOR);
  Wire.write(reg); // Indicate temperature value to read
  Wire.endTransmission();
  Wire.requestFrom(TEMPSENSOR, 1); // Request data
  while(Wire.available() < 1); // Wait for data
  byte temperatureData = Wire.read(); // Temp. value
  delay(50);

  // Print to screen.
  printToFirst("Temperature");
  LCD.write(0xFE);
  LCD.write(64 + 128 + 7);
  LCD.print(temperatureData);
}

void setup() {
  // Set pins for data transmission (output).
  pinMode(LED_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_PIN, OUTPUT);
  
  LCD.begin(9600); // open serial port to write at 9600 bps.
  Wire.begin(); // Join I2C bus.
}

void loop() {

  // Move forward 1 tile.
  moveForward(TILE_DISTANCE);
  delay(1000);

  // Check distance.
  if (ping() < 20) {
    avoidCollision();
  }


}
