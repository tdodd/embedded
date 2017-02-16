/*****************************************************************************
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
int RIGHT_MOTOR_PIN = 45;
int LEFT_MOTOR_PIN = 8;
int RIGHT_MOTOR_SENSOR = 48;
int LEFT_MOTOR_SENSOR = 49;

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
const int TILE_DISTANCE = 113;

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

// 64 transitions for 360` rotation of the wheel (360/5.625)
// 113 transitions to move ~30.5 cm (30.5/(5.5*pi/64))
// 160 transitions to move ~43 cm (43*/(5.5*pi/64))
// 233 transitions for a wheel to rotate 360` around the centre of the robot (20*pi/(5.5*pi/64))
// 29 transitions for a wheel to rotate 45` around the centre of the robot ((20*pi/(5.5*pi/64))/8)

/**
 * 
 * lastLeft, lastRight is the previous signal (can be LOW or HIGH)
 * leftMove and rightMove determine with the left and right motors move forward or backward, respectively
 * (true means move forward, backward otherwise)
 */
void synchronizeStep(int lastLeft, int lastRight, bool leftMove, bool rightMove) {
  int targetLeftValue = lastLeft == LOW ? HIGH : LOW;
  int targetRightValue = lastRight == LOW ? HIGH : LOW;
  
  if (leftMove) {
    analogWrite(LEFT_MOTOR_PIN, LEFT_MOTOR_FORWARD_PULSE);
  } else {
    analogWrite(LEFT_MOTOR_PIN, LEFT_MOTOR_BACKWARD_PULSE);
  }
  if (rightMove) {
    analogWrite(RIGHT_MOTOR_PIN, RIGHT_MOTOR_FORWARD_PULSE);
  } else {
    analogWrite(RIGHT_MOTOR_PIN, RIGHT_MOTOR_BACKWARD_PULSE);
  }
  
  bool leftFlag = false, rightFlag = false;
  while (true) {
    if (!leftFlag && digitalRead(LEFT_MOTOR_SENSOR) == targetLeftValue) {
      analogWrite(LEFT_MOTOR_PIN, 0);
      leftFlag = true;
    }
    if (!rightFlag && digitalRead(RIGHT_MOTOR_SENSOR) == targetRightValue) {
      analogWrite(RIGHT_MOTOR_PIN, 0);
      rightFlag = true;
    }
    if (leftFlag && rightFlag) {
      return;
    }
  }
}

/**
 * moveForward, Moves the robot forward for a specified number of transitions and prints action to LCD.
 * 
 * @param transitions, the distance in number of transitions to move.
 * @param *LCD, a pointer to the LCD screen.
 */
void moveForward(int transitions, SoftwareSerial* LCD) {
  clearLcd(LCD);
  printToFirst(LCD, "Moving");
  printToSecond(LCD, "Forward");
  int initialLeft = digitalRead(LEFT_MOTOR_SENSOR);
  int initialRight = digitalRead(RIGHT_MOTOR_SENSOR);
  for (int i = 0; i < transitions; i ++) {
    synchronizeStep(initialLeft, initialRight, true, true);
    initialLeft = initialLeft == LOW ? HIGH : LOW;
    initialRight = initialRight == LOW ? HIGH : LOW;
  }
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
  printToSecond(LCD, "Left");
  int initialLeft = digitalRead(LEFT_MOTOR_SENSOR);
  int initialRight = digitalRead(RIGHT_MOTOR_SENSOR);
  for (int i = 0; i < 29*increment; i ++) {
    synchronizeStep(initialLeft, initialRight, true, false);
    initialLeft = initialLeft == LOW ? HIGH : LOW;
    initialRight = initialRight == LOW ? HIGH : LOW;
  }
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
  int initialLeft = digitalRead(LEFT_MOTOR_SENSOR);
  int initialRight = digitalRead(RIGHT_MOTOR_SENSOR);
  for (int i = 0; i < 29*increment; i ++) {
    synchronizeStep(initialLeft, initialRight, false, true);
    initialLeft = initialLeft == LOW ? HIGH : LOW;
    initialRight = initialRight == LOW ? HIGH : LOW;
  }
  stop(LCD);
}

/**
 * stop, Stops the current motion and prints action to LCD.
 *
 * @param *LCD, a pointer to the LCD screen.
 */
void stop(SoftwareSerial* LCD) {
  // analogWrite(RIGHT_MOTOR_PIN, 0);
  // analogWrite(LEFT_MOTOR_PIN, 0);
  clearLcd(LCD);
  printToFirst(LCD, "Stopping");
}

void setup() {
  // Set pins for data transmission (output).
  pinMode(LED_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_SENSOR, INPUT);
  pinMode(LEFT_MOTOR_SENSOR, INPUT);

  SoftwareSerial LCD = SoftwareSerial(0, LCD_PIN);
  LCD.begin(9600); // open serial port to write at 9600 bps.

  // Print student numbers.
//  printToFirst(&LCD, THOMAS_NUM);
//  printToSecond(&LCD, PENG_NUM);
//  blinkLed(5);
//  clearLcd(&LCD);

//      moveForward(113 * 5, &LCD);
//    turnRight(2, &LCD);
//    turnRight(2, &LCD);

//  // Traverse path.
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

void loop() {
}
