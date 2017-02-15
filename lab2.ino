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

#include <SoftwareSerial.h>

/**
 * Globals
 */
int LED_PIN = 13;
int LCD_PIN = 18;
int RIGHT_MOTOR_PIN = 45;
int LEFT_MOTOR_PIN = 8;

/**
 * Forward and backward movement
 */
int LEFT_MOTOR_BACKWARD_PULSE = 200;
int RIGHT_MOTOR_BACKWARD_PULSE = 100;
int LEFT_MOTOR_FORWARD_PULSE = -100;
int RIGHT_MOTOR_FORWARD_PULSE = 500;

char THOMAS_NUM[] = "6822414";
char PENG_NUM[] = "4229132";

/**
 * Timing constants.
 */
int FULL_TURN_MILLI = 3500;
int TILE_DISTANCE = 1200;

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
void moveForward(int milli_sec, SoftwareSerial* LCD) {
  clearLcd(LCD);
  printToFirst(LCD, "Moving");
  printToSecond(LCD, "Forward");
  analogWrite(RIGHT_MOTOR_PIN, RIGHT_MOTOR_FORWARD_PULSE);
  analogWrite(LEFT_MOTOR_PIN, LEFT_MOTOR_FORWARD_PULSE);
  delay(milli_sec);
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
  printToSecond(LCD, "Right");
  analogWrite(RIGHT_MOTOR_PIN, RIGHT_MOTOR_FORWARD_PULSE);
  analogWrite(LEFT_MOTOR_PIN, LEFT_MOTOR_BACKWARD_PULSE);
  delay(FULL_TURN_MILLI * increment/8);
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
  delay((FULL_TURN_MILLI - 350) * increment/8);
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

void setup() {
  // Set pins for data transmission (output).
  pinMode(LED_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_PIN, OUTPUT);
}

void loop() {
  SoftwareSerial LCD = SoftwareSerial(0, LCD_PIN);
  LCD.begin(9600); // open serial port to write at 9600 bps.
  
  // Print student numbers for 5 seconds.
  printToFirst(&LCD, THOMAS_NUM);
  printToSecond(&LCD, PENG_NUM);
  blinkLed(5);
  clearLcd(&LCD);

  // Traverse path.
  moveForward(TILE_DISTANCE * 2, &LCD);
  turnRight(2, &LCD);
  
  moveForward(TILE_DISTANCE * 2, &LCD);
  turnRight(2, &LCD);
  
  moveForward(TILE_DISTANCE * 3, &LCD);
  turnRight(2, &LCD);
  
  moveForward(TILE_DISTANCE * 3, &LCD);
  turnRight(2, &LCD);
  
  moveForward(TILE_DISTANCE * 2, &LCD);
  turnLeft(1, &LCD);
  
  moveForward(TILE_DISTANCE, &LCD);
  turnLeft(3, &LCD);
  
  moveForward(TILE_DISTANCE * 4, &LCD);
  turnLeft(2, &LCD);
  
  moveForward(TILE_DISTANCE * 2, &LCD);
  turnLeft(2, &LCD);
  
  moveForward(TILE_DISTANCE * 2, &LCD);
  
  delay(5000);
}
