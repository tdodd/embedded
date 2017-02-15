/******************************************************************************
*
* Names:        Thomas Dodd 6822414
*               Peng Liu 4229132
* Course Code:  SEG 4145
* Lab Number:   1
* File name:    lab1.ino
* Date:         January 18, 2017
*
* Description
* *************
* This file contains all functions for lab 1.
******************************************************************************
*/

#include <SoftwareSerial.h>

/**
 * Globals
 */
int LED_PIN = 13;
int LCD_PIN = 18;
int RIGHT_MOTOR_PIN = 45; // Forward: 10, backward: 191.5
int LEFT_MOTOR_PIN = 8; // Forward: 191.5, backward: 10

/**
 * Forward and backward movement
 */
int LEFT_MOTOR_BACKWARD_PULSE = 200;
int RIGHT_MOTOR_BACKWARD_PULSE = 100;
int LEFT_MOTOR_FORWARD_PULSE = -100;
int RIGHT_MOTOR_FORWARD_PULSE = 500;

char THOMAS_NUM[] = "6822414";
char PENG_NUM[] = "4229132";
int FULL_TURN_MICROS = 3500;

/**
 * printToFirst, Prints text on the 1st line of the LCD display.
 * 
 * @param *LCD, a pointer to the LCD screen.
 * @param str, the string to display.
 */
void printToFirst(SoftwareSerial* LCD, char str[]) {
  LCD->write(0xFE);
  LCD->write(128 + getOffset(strlen(str)));
  LCD->print(str);
}

/**
 * printToSecond, Prints text on the 2nd line of the LCD display.
 * 
 * @param *LCD, a pointer to the LCD screen.
 * @param str, the string to display.
 */
void printToSecond(SoftwareSerial* LCD, char str[]) {
  LCD->write(0xFE);
  LCD->write(64 + 128 + getOffset(strlen(str)));
  LCD->print(str);
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
 * 
 * @param *LCD, a pointer to the LCD screen.
 */
void clearLcd(SoftwareSerial* LCD) {
  LCD->write(0xFE);
  LCD->write(0x01);
  delay(10);  
}

/**
 * blinkLed, Blinks the led a specified number of times.
 * 
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
 * 
 * @param micro_sec, the time in microseconds for the wheels to turn.
 * @param *LCD, a pointer to the LCD screen.
 */
void moveForward(int micro_sec, SoftwareSerial* LCD) {
  clearLcd(LCD);
  printToFirst(LCD, "Moving");
  printToSecond(LCD, "Forward");
  analogWrite(RIGHT_MOTOR_PIN, RIGHT_MOTOR_FORWARD_PULSE);
  analogWrite(LEFT_MOTOR_PIN, LEFT_MOTOR_FORWARD_PULSE);
  delay(micro_sec);
  stop(LCD);
}

/**
 * turnLeft, Turns the robot left and prints action to LCD.
 * 
 * @param increment, the # of increments of 45 degrees to turn.
 * @param *LCD, a pointer to the LCD screen.
 */
void turnRight(int increment, SoftwareSerial* LCD) {
  clearLcd(LCD);
  printToFirst(LCD, "Rotating");
  printToSecond(LCD, "right");
  analogWrite(RIGHT_MOTOR_PIN, RIGHT_MOTOR_FORWARD_PULSE);
  analogWrite(LEFT_MOTOR_PIN, LEFT_MOTOR_BACKWARD_PULSE);
  delay(FULL_TURN_MICROS * increment/8);
  stop(LCD);
}

/**
 * turnRight, Turns the robot right and prints action to LCD.
 *
 * @param increment, the # of increments of 45 degrees to turn.
 * @param *LCD, a pointer to the LCD screen.
 */
void turnLeft(int increment, SoftwareSerial* LCD) {
  clearLcd(LCD);
  printToFirst(LCD, "Rotating");
  printToSecond(LCD, "left");
  analogWrite(RIGHT_MOTOR_PIN, RIGHT_MOTOR_BACKWARD_PULSE);
  analogWrite(LEFT_MOTOR_PIN, LEFT_MOTOR_FORWARD_PULSE);
  delay((FULL_TURN_MICROS - 350) * increment/8);
  stop(LCD);
}

/**
 * stop, Stops the current motion and prints action to LCD.
 *
 * @param *LCD, a pointer to the LCD screen.
 */
void stop(SoftwareSerial* LCD) {
  clearLcd(LCD);
  printToFirst(LCD, "Stopping");
  analogWrite(RIGHT_MOTOR_PIN, 0);
  analogWrite(LEFT_MOTOR_PIN, 0);
}

/**
 * path1, Steps required to complete path 1.
 *
 * @param *LCD, a pointer to the LCD screen.
 */
void path1(SoftwareSerial* LCD) {
  moveForward(5000, LCD);  
  turnLeft(2, LCD);

  moveForward(5000, LCD);
  turnRight(2, LCD);

  moveForward(5000, LCD);
  turnRight(2, LCD);

  moveForward(5000, LCD);
  turnLeft(2, LCD);

  moveForward(5000, LCD);
}

/*
 * path2, Steps required to complete path 2.
 *
 * @param *LCD, a pointer to the LCD screen.
 */
void path2(SoftwareSerial* LCD) {
  moveForward(5000, LCD);
  turnRight(2, LCD);

  moveForward(3000, LCD);
  turnLeft(3, LCD);

  moveForward(5000, LCD);
  turnRight(3, LCD);

  moveForward(3000, LCD);
  turnLeft(2, LCD);

  moveForward(5000, LCD);
}

void setup() {
  // Set pins for data transmission (output).
  pinMode(LED_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_PIN, OUTPUT);

  SoftwareSerial LCD = SoftwareSerial(0, LCD_PIN);
  LCD.begin(9600); // open serial port to write at 9600 bps.

  // Print student numbers.
  printToFirst(&LCD, THOMAS_NUM);
  printToSecond(&LCD, PENG_NUM);

  // Blink the led 5 times.
  blinkLed(5);
  clearLcd(&LCD);

  // Print 'Path 1' for 3 seconds.
  printToFirst(&LCD, "Path");
  printToSecond(&LCD, "1");
  delay(3000);
  clearLcd(&LCD);

  path1(&LCD); // Traverse path 1.
  delay(3000);
  
  // Print 'Path 2' for 3 seconds.
  printToFirst(&LCD, "Path");
  printToSecond(&LCD, "2");
  delay(3000);
  clearLcd(&LCD);
  
  path2(&LCD); // Traverse path 2.
}

void loop() {
  //analogWrite(RIGHT_MOTOR_PIN, 500);
  //analogWrite(LEFT_MOTOR_PIN, -100);
}
