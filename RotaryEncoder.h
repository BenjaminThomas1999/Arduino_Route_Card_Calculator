#ifndef _ROTARYENCODER_H    // Put these two lines at the top of your file.
#define _ROTARYENCODER_H    // (Use a suitable name, usually based on the file name.)

int smallest(int a, int b){
  if(a<b){
    return a;
  }else{
    return b;
  }
}

int biggest(int a, int b){
  if(a>b){
    return a;
  }else{
    return b;
  }
}

int *arrayPush(int array[4], int newItem){
  array[0] = array[1];
  array[1] = array[2];
  array[2] = array[3];
  array[3] = newItem;

  return array;
}

class RotaryEncoder {
  private:
    uint8_t pinA;
    uint8_t pinB;
    int button;

    int lastDialState;
    int currentDialState;

    bool lastButtonState;
    bool currentButtonState;

    int direction = 0;
    int directionLog[4] = {0,0,0,0};
    int speedLog[4] = {0,0,0,0};
    int lastTime = 0;
    int maxRotationSpeed = 400;

    float position = 0;

    float getSpeed(){//if mode == 1, the function will always return a slow speed for menus.
      if(biggest(biggest(speedLog[0], speedLog[1]), biggest(speedLog[2], speedLog[3])) == maxRotationSpeed){
        return 0.25;
      }
      return 0.80;
    }

    int getDirection(){
      int directionSum = directionLog[0]+directionLog[1]+directionLog[2]+directionLog[3];
      if(directionSum > 0){
        return 1;
      }else if(directionSum < 0){
        return -1;
      }
      return 0;
    }
    
  public:
    
    void resetPosition(){
      position = 0;
    }

    void setPosition(int x){
      position = x;
    }

    int getPosition(){
      return (int) position;
    }

    void boundPosition(int smallest, int largest){
      if((int) position > largest){
        position = largest;
      }
      else if((int) position < smallest){
        position = smallest;
      }
    }
    
    float getMovement(){
      return getDirection()*getSpeed();
    }

    bool checkRotation(){//returns true if there's a change
      lastDialState = currentDialState;
      currentDialState = (digitalRead(pinA) << 1) | digitalRead(pinB);
      
      if (currentDialState != lastDialState) { // State has changed
        int speed = millis()-lastTime;
        lastTime = millis();
        arrayPush(speedLog, biggest(smallest(speed, maxRotationSpeed), maxRotationSpeed-1));
        // Determine direction using a lookup table or direct comparison
        if ((lastDialState == 0b00 && currentDialState == 0b01) ||
            (lastDialState == 0b01 && currentDialState == 0b11) ||
            (lastDialState == 0b11 && currentDialState == 0b10) ||
            (lastDialState == 0b10 && currentDialState == 0b00)) {
          
          arrayPush(directionLog, 1); // Clockwise
          
        } else if ((lastDialState == 0b00 && currentDialState == 0b10) ||
                  (lastDialState == 0b10 && currentDialState == 0b11) ||
                  (lastDialState == 0b11 && currentDialState == 0b01) ||
                  (lastDialState == 0b01 && currentDialState == 0b00)) {
          
          arrayPush(directionLog, -1); // Counterclockwise
        }
        
        lastDialState = currentDialState;
        position += getMovement();
      }
      return 0;
    }

    bool click(){
      lastButtonState = currentButtonState;
      currentButtonState = digitalRead(button);

      if(lastButtonState == 1 && currentButtonState == 0){
        return 1;
      }
      else{
        return 0;
      }
    }

    bool buttonHeld(){
      return !digitalRead(button);
    }

    RotaryEncoder(int x, int y, int z){
      pinA = x;
      pinB = y;
      button = z;

      pinMode(pinA, INPUT_PULLUP);
      pinMode(pinB, INPUT_PULLUP);
      pinMode(button, INPUT_PULLUP);
      lastButtonState = digitalRead(button);
      currentButtonState = lastButtonState;

      currentDialState = (digitalRead(pinA) << 1) | digitalRead(pinB);
    }
};

#endif // _ROTARYENCODER_H 