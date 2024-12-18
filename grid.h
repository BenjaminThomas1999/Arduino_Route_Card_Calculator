#ifndef _GRID_H    // Put these two lines at the top of your file.
#define _GRID_H      // (Use a suitable name, usually based on the file name.)

class grid{
  public:
    uint8_t x1;
    uint8_t x2;

    uint8_t y1;
    uint8_t y2;

  String formatGrid(uint8_t grid1, uint8_t grid2){
    int grid = grid1*100+grid2;
    char buffer[5]; // 4 digits + 1 for the null terminator
    sprintf(buffer, "%04d", grid); // Format number with leading zeros
    return buffer; 
  }

  void bound(){
    if(x1>99){x1 = 99;}
    if(x1<0){x1 = 0;}

    if(x2>99){x2 = 99;}
    if(x2<0){x2 = 0;}

    if(y1>99){y1 = 99;}
    if(y1<0){y1 = 0;}

    if(y2>99){y2 = 99;}
    if(y2<0){y2 = 0;}
  }

  void reset(){
    x1, x2, y1, y2 = 0;
  }

  String getX(){
    return formatGrid((uint8_t) x1, (uint8_t) x2);
  }

  int getXInt(){
    return x1*100+x2;
  }

  int getYInt(){
    return y1*100+y2;
  }

  String getY(){
    return formatGrid((uint8_t) y1, (uint8_t) y2);
  }
  
  grid(uint8_t a1, uint8_t a2, uint8_t b1, uint8_t b2){
    x1 = a1;
    x2 = a2;
    y1 = b1;
    y2 = b2;
  }
};

#endif //_GRID_H 