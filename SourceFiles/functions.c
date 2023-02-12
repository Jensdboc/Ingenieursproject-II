/*
 * Functions.c
 *
 *  Authors: Jens De Bock
 *           Wannes De Vos
 */

#include "dwenguino/functions.h"
#include "dwenguino/dwenguino_lcd.h"
#include "dwenguino/dwenguino_board.h"

void begin_LCD(){
  // Shows start screen
  printStringToLCD("Welcome to", 0,3);
  printStringToLCD("Bouncy  Ball",1,2);
  _delay_ms(1000);
  clearLCD();
}

void custom_char(unsigned char *Pattern, const char nummer) {
  int i = 0;
  commandLCD(0x40+(nummer*8));
  for(i=0;i<8;i++) {
    // Write data to CGRAM RS bit high
    LCD_RS_HIGH;  
    // Write data to CGRAM R/W bit high
    LCD_RW_LOW;   
    // Enable pin high
    LCD_EN_HIGH;  
    // Puts the data in the right place to the data port
    LCD_DATA = Pattern[i]; 
    // Data is clocked at falling edge of enable pin
    LCD_EN_LOW; 
    _delay_us(45);
  }
}

unsigned char bits_r1(unsigned char a){
  a = a >> 1;
  a |= 0b10000;
  return a;
}

void calc_pos(int a_x, int a_y, int* v_x, int* v_y, int* x, int* y){
  // Time is 1 frame: delta(t) = 1 can be omitted in formulas
  *v_x = a_x + 0.9*(*v_x); 
  *v_y = a_y + 0.9*(*v_y);
  
  // Setting up velocity limit
  if (*v_x > 60){
    *v_x = 60;
  }
  if (*v_y > 60){
    *v_y = 60;
  }
  if (*v_x < -60){
    *v_x = -60;
  }
  if (*v_y < -60){
    *v_y = -60;
  }

  // Calculating coordinates
  *x = *v_x + *x; 
  *y = *v_y + *y; 
}

void draw_3x3_to_LCD(int x_pos, int y_pos){
  // Initialize x and y values
  int x_pix, y_pix, x_row, y_col;    
  int x_min = (x_pos - 1)/8;
  int x_max = (x_pos + 1)/8;
  int y_min = (y_pos - 1)/5;
  int y_max = (y_pos + 1)/5;
  unsigned char temp = 0;

  // Initialize custom characters
  unsigned char char_1[8] = {0b00000, 0b00000 ,0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};
  unsigned char char_2[8] = {0b00000, 0b00000 ,0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};
  unsigned char char_3[8] = {0b00000, 0b00000 ,0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};
  unsigned char char_4[8] = {0b00000, 0b00000 ,0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};
  
  for(int x_waarde = x_pos-1; x_waarde <= x_pos + 1; x_waarde++){
    for (int y_waarde = y_pos - 1; y_waarde <= y_pos + 1;y_waarde++){
      x_pix = x_waarde%8;   
      y_pix = y_waarde%5;  
      x_row = x_waarde/8;  
      y_col = y_waarde/5;  
      // Top left -> Char 1
      if (x_row == x_min && y_col == y_min){ 
        temp = 1 << (4-y_pix);
        char_1[x_pix] |= temp; 
      } 
      // Top right -> Char 2
      else if (x_row == x_min && y_col == y_max) { 
        temp = 1 << (4-y_pix);
        char_2[x_pix] |= temp;
      } 
      // Bottom left -> Char 3
      else if (x_row == x_max && y_col == y_min) { 
        temp = 1 << (4-y_pix);  
        char_3[x_pix] |= temp;
      } 
      // Bottom right -> Char 4
      else if (x_row == x_max && y_col == y_max) { 
        temp = 1 << (4-y_pix);
        char_4[x_pix] |= temp;
      } 
    }
  }

  // Assign values to custom characters
  custom_char(char_1, 3);
  custom_char(char_2, 4);
  custom_char(char_3, 5);
  custom_char(char_4, 6);

  // Print bal to screen
  printCharToLCD(0x05, x_max, y_min);
  printCharToLCD(0x03, x_min, y_min);
  if (y_min != y_max) {
    printCharToLCD(0x06, x_max, y_max);
    printCharToLCD(0x04, x_min, y_max);
  }
}

void draw_level(unsigned char level[HEIGHT][LENGTH]) {
  for (int i = 0; i < HEIGHT; i++){
    for(int j = 0; j < LENGTH; j++){
      if(level[i][j] == 1){ 
        printCharToLCD(FULL_BLOCK, i, j);
      } 
      else if (level[i][j] == 2){
        printStringToLCD(GOAL, i, j);
      }
      else if (level[i][j] == 3){
        printCharToLCD(0x01, i, j);
      }
    }  
  }
}

void collision_handler(unsigned char level[HEIGHT][LENGTH], int* x, int* y, unsigned char *state, int* v_x, int* v_y, unsigned char level_number){ // 0: empty, 1: Full block
  // Initialize x and y values 
  int x_pos = (int)((*x)/10);
  int y_pos = (int)((*y)/10);
  int x_min = (x_pos - 1)/8;
  int x_max = (x_pos + 1)/8;
  int y_min = (y_pos - 1)/5;
  int y_max = (y_pos + 1)/5;

  //Check for collision
  if ((level[x_max][y_max] == 1) || (level[x_max][y_min] == 1) || (level[x_min][y_min] == 1) || (level[x_min][y_max] == 1)){
    *state = 2;
  } 
  else if ((level[x_max][y_max] == 2) || (level[x_max][y_min] == 2) || (level[x_min][y_min] == 2) || (level[x_min][y_max] == 2)){
    if (level_number == 10) {
      *state = 4;
    }
    else {
      *state = 3;
    }
  } 
  else if ((level[x_max][y_max] == 3) || (level[x_max][y_min] == 3) || (level[x_min][y_min] == 3) || (level[x_min][y_max] == 3)){
   // When the ball hits a bouncy wall from the left side it will bounce back
   // Otherwise the ball will be sucked through the wall
   // The faster the ball appreaches the slime, the deeper it will go in the slime
   *v_y = -30;
  }

  // Collision detection walls
  if (*x <= 11) {  
    *x = 11; 
    //Bounce back
    *v_x = -(*v_x);  
  }
  if (*x >= 149) {
    *x = 149;
    //Bounce back
    *v_x = -(*v_x);
  } 
  if (*y <= 11) {  
    *y = 11;
    //Bounce back
    *v_y = -(*v_y);
  }
  if (*y >= 789) {
    *y = 789;
    //Bounce back
    *v_y = -(*v_y);
  } 
}

void setlevel(unsigned char current_level[HEIGHT][LENGTH], unsigned char level[HEIGHT][LENGTH]){
  for (int i = 0; i < HEIGHT; i++){
    for(int j = 0; j < LENGTH; j++){
      current_level[i][j] = level[i][j];
    }  
  }
}

void reset(int* x, int* y, int* v_x, int* v_y, unsigned char* state){
  *state = 0;
  *x = 30;
  *y = 20;
  *v_x = 0; 
  *v_y = 0;
}
