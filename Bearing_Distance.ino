#include <Arduino.h>
#include "RotaryEncoder.h"
#include "grid.h"
#include <GyverOLED.h>
GyverOLED<SSH1106_128x64> oled;

RotaryEncoder leftDial(4, 2, 3);
RotaryEncoder rightDial(5, 7, 6);

void oledScrollPrint(String text, uint8_t highlight, bool newLine){
  oled.invertText(highlight == 1); 
  oled.print(text);
  oled.invertText(false); 
  if(newLine){
    oled.println("\n");
  }
}

String calculateDistance(grid& gridA, grid& gridB) {
  long int deltaX = gridB.getXInt() - gridA.getXInt();
  long int deltaY = gridB.getYInt() - gridA.getYInt();
  double distance = sqrt(sq(deltaX) + sq(deltaY)) * 10;

  if(distance>999){
    return String(distance/1000, 2) + "km";
  }else{
    return String((int) distance) + "m";
  }
}

String calculateBearing(grid& gridA, grid& gridB) {
    long int deltaX = gridB.getXInt() - gridA.getXInt();
    long int deltaY = gridB.getYInt() - gridA.getYInt();

    // Check for identical points
    if (deltaX == 0 && deltaY == 0) {
        return "N/A";
    }

    // Calculate bearing using atan2
    double bearing = atan2(deltaX, deltaY);
    // Normalize to ensure positive angle
    if (bearing < 0) {
        bearing += TWO_PI;
    }

    // Convert radians to military mils (6400 mils = 2π radians)
    long bearingMils = round(bearing * (6400.0 / TWO_PI));
    String leadingZeros = "";
    
    if(bearingMils < 1000){
      leadingZeros += "0";
    }
    if(bearingMils < 100){
      leadingZeros += "0";
    }
    if(bearingMils < 10){
      leadingZeros += "0";
    }
    
    return leadingZeros + String(bearingMils) + " mils";
}

const uint8_t MAX_SCROLL = 4;
const uint8_t MIN_SCROLL = 1;
const uint8_t MAX_POSITION = 99;
const uint8_t MIN_POSITION = 0;

void distanceBearingScreen(){
  uint8_t lastScroll = MIN_SCROLL;
  uint8_t currentScroll = MIN_SCROLL;

  static grid gridA(00, 00, 00, 00);
  static grid gridB(00, 00, 00, 00);

  leftDial.setPosition(gridA.x1);
  rightDial.setPosition(gridA.x2);

  String distance = "!";
  String bearing = "!";
  while(currentScroll > 0){

    leftDial.boundPosition(MIN_POSITION, MAX_POSITION);
    rightDial.boundPosition(MIN_POSITION, MAX_POSITION);

    oled.clear();
    oled.setCursor(0, 0);

    oled.print("Grid A: ");
    oledScrollPrint(gridA.getX(), currentScroll, false);
    oled.print(" ");
    oledScrollPrint(gridA.getY(), currentScroll-1, true);

    oled.print("Grid B: ");
    oledScrollPrint(gridB.getX(), currentScroll-2, false);
    oled.print(" ");
    oledScrollPrint(gridB.getY(), currentScroll-3, true);

    distance = calculateDistance(gridA, gridB);
    bearing = calculateBearing(gridA, gridB);

    oled.print("Distance: ");
    oled.println(distance);
    oled.println("");
    oled.print("Bearing: ");
    oled.println(bearing);
    oled.println("");

    switch(currentScroll){
      case 1:
        gridA.x1 = leftDial.getPosition();
        gridA.x2 = rightDial.getPosition();
        break;
      case 2:
        gridA.y1 = leftDial.getPosition();
        gridA.y2 = rightDial.getPosition();
        break;
      case 3:
        gridB.x1 = leftDial.getPosition();
        gridB.x2 = rightDial.getPosition();
        break;
      case 4:
        gridB.y1 = leftDial.getPosition();
        gridB.y2 = rightDial.getPosition();
        break;
    }

    if(leftDial.buttonHeld() && rightDial.buttonHeld()){
      gridA.reset();
      gridB.reset();
      leftDial.setPosition(MIN_POSITION);
      rightDial.setPosition(MIN_POSITION);
    }
    
    lastScroll = currentScroll;
    if(rightDial.click()){
      currentScroll++;
    }
    if(leftDial.click()){
      currentScroll--;
    }

    if(currentScroll > MAX_SCROLL){
      currentScroll = MAX_SCROLL;
    }

    if(lastScroll != currentScroll){
      switch(currentScroll){
        case 1:
          leftDial.setPosition(gridA.x1);
          rightDial.setPosition(gridA.x2);
          break;
        case 2:
          leftDial.setPosition(gridA.y1);
          rightDial.setPosition(gridA.y2);
          break;
        case 3:
          leftDial.setPosition(gridB.x1);
          rightDial.setPosition(gridB.x2);
          break;
        case 4:
          leftDial.setPosition(gridB.y1);
          rightDial.setPosition(gridB.y2);
          break;
      }
    }
    oled.update();
  }
  return 0;
}

void setup() {
  Serial.begin(9600);
  oled.init();
  // Enable pin-change interrupt for PCINT2 group
  PCICR |= (1 << PCIE2); // Enable PCINT2 (PCINT[23:16])

  // Enable interrupts for individual pins (unmask them in PCMSK2)
  PCMSK2 |= (1 << PCINT18); // Enable interrupt for pin 2
  PCMSK2 |= (1 << PCINT20); // Enable interrupt for pin 4
  PCMSK2 |= (1 << PCINT21); // Enable interrupt for pin 5
  PCMSK2 |= (1 << PCINT23); // Enable interrupt for pin 7

  // Clear any pending interrupt flags
  PCIFR |= (1 << PCIF2);

}

void loop() {
  distanceBearingScreen();
}

ISR(PCINT2_vect) {
    // Assume all pins were HIGH initially
    static uint8_t lastState = 0xFF;
    uint8_t currentState = PINC;  // Read the current state of PORTC (pins 0-7)

    if(lastState != currentState){
      leftDial.checkRotation();
      rightDial.checkRotation();
    }

    lastState = currentState; // Update the last state
}