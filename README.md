# Arduino Route Card Calculator

This project is an Arduino-based route card calculator that uses a rotary encoder for input and a grid system for navigation.

## Installation

1. Clone the repository to your local machine.
2. Open the project in the Arduino IDE.
3. Connect your Arduino board to your computer.
4. Upload the code to the Arduino board.

## Usage

### Rotary Encoder

The `RotaryEncoder` class handles the rotary encoder input. It provides methods to read the encoder's position, direction, and speed. It also includes methods to check for button clicks and button holds.

#### Methods

- `void resetPosition()`: Resets the encoder position to 0.
- `void setPosition(int x)`: Sets the encoder position to `x`.
- `int getPosition()`: Returns the current encoder position.
- `void boundPosition(int smallest, int largest)`: Bounds the encoder position between `smallest` and `largest`.
- `float getMovement()`: Returns the movement of the encoder.
- `bool checkRotation()`: Checks if the encoder has rotated and updates the position.
- `bool click()`: Checks if the encoder button has been clicked.
- `bool buttonHeld()`: Checks if the encoder button is being held.

### Grid System

The `grid` class handles the grid system for navigation. It provides methods to format and bound the grid coordinates.

#### Methods

- `String formatGrid(uint8_t grid1, uint8_t grid2)`: Formats the grid coordinates with leading zeros.
- `void bound()`: Bounds the grid coordinates between 0 and 99.
- `void reset()`: Resets the grid coordinates to 0.
- `String getX()`: Returns the formatted X coordinates.
- `int getXInt()`: Returns the integer value of the X coordinates.
- `int getYInt()`: Returns the integer value of the Y coordinates.
- `String getY()`: Returns the formatted Y coordinates.
