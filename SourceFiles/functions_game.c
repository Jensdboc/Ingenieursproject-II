/*
 * Functions_game.c
 *
 *  Authors: Jens De Bock
 *           Wannes De Vos
 */

#include "dwenguino/functions_game.h"
#include "dwenguino/functions_communication.h"
#include "dwenguino/dwenguino_lcd.h"
#include "dwenguino/dwenguino_board.h"
#include "dwenguino/functions.h"

// Initialize state
extern unsigned char* state;

// Initialize mass
extern int massa;

// Initialize startingvalues of x, y and z 
long int gem_x, gem_y, gem_z;

// Initialize x, y and velocities
int a = 30;
int* x = &a;
int b = 20;
int* y = &b;
int c = 0;
int* v_x = &c; 
int d = 0;
int* v_y = &d;

// Initialize acceleration
int a_x = 0;
int a_y = 0;

// Initialize startingvalues of x, y and z 
long int init_gem_x;
long int init_gem_y;
long int init_gem_z;

// Initialize startingvalue of roll_x and pitch_y
float init_roll_x;
float init_pitch_y;

// Declare variables
long int gem_x, gem_y, gem_z;
float roll_x, pitch_y;

// Initiliaze level and state variables
unsigned char level = 1;
unsigned char current_level[HEIGHT][LENGTH];
unsigned char level_1[HEIGHT][LENGTH] = {{0,0,0,0,0,0,0,0,3,3,3,3,3,3,3,3},{0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,2}};
unsigned char level_2[HEIGHT][LENGTH] = {{0,0,0,0,0,0,3,3,1,1,0,0,0,0,0,0},{0,1,3,1,0,0,0,0,0,0,0,0,1,1,2,3}};
unsigned char level_3[HEIGHT][LENGTH] = {{0,1,3,0,0,0,0,0,0,0,0,0,0,3,0,1},{0,0,0,0,1,3,0,0,0,0,3,3,0,0,0,2}};
unsigned char level_4[HEIGHT][LENGTH] = {{0,3,3,3,3,3,3,3,3,3,3,2,1,2,1,1},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
unsigned char level_5[HEIGHT][LENGTH] = {{0,3,1,3,0,0,0,0,0,0,0,3,0,0,0,2},{0,0,0,0,0,0,3,3,3,1,0,0,0,0,1,0}};
unsigned char level_6[HEIGHT][LENGTH] = {{0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1},{0,2,1,0,0,0,0,3,3,3,3,3,0,0,1,2}};
unsigned char level_7[HEIGHT][LENGTH] = {{0,1,0,0,0,0,0,0,0,0,1,0,0,3,0,2},{0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1}};
unsigned char level_8[HEIGHT][LENGTH] = {{2,3,0,3,0,3,0,3,0,3,0,3,0,3,0,1},{2,0,3,0,3,0,3,0,3,0,3,0,3,0,3,1}};
unsigned char level_9[HEIGHT][LENGTH] = {{0,1,1,1,1,1,1,1,1,1,1,2,1,2,2,2},{0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2}};
unsigned char level_10[HEIGHT][LENGTH] = {{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},{0,1,0,1,0,0,0,0,0,0,0,0,3,2,1,1}};

// Initialize bounceblocks
unsigned char char_bounce_block[8] = {0b11111, 0b10011 ,0b10101, 0b11001, 0b11001, 0b10101, 0b10011, 0b11111};

void init(){
      // Set custom_char of bounce_block
    custom_char(char_bounce_block, 1);

    // Set first level
    setlevel(current_level, level_1);
}

void calibrate(){
    // Initialize startingvalues of x, y and z 
    init_gem_x = calc_tot_x();
    init_gem_y = calc_tot_y();
    init_gem_z = calc_tot_z();

    // Initialize startingvalues of roll_x and roll_y
    init_roll_x = atan2(init_gem_y, sqrt((init_gem_z)*(init_gem_z) + (init_gem_x)*(init_gem_x)));
    init_pitch_y = atan2(-init_gem_x, (init_gem_z)); 
}

void title_state(){
    if (level == 0){
        begin_LCD();
        level = 1;
    }
    printStringToLCD("Bouncy  Ball",0, 2);
    printStringToLCD("Level", 1, 4);
    printIntToLCD(level, 1, 11);
    _delay_ms(1000);
    *state = 1; 
}

void game_state(){
    // Calculate averages values of x, y and z
    gem_x = calc_tot_x();
    gem_y = calc_tot_y();
    gem_z = calc_tot_z();

    // Calculate roll, pitch and accelerations
    roll_x = atan2(gem_y, sqrt((gem_z)*(gem_z) + (gem_x)*(gem_x))) - init_roll_x;
    pitch_y = atan2(-gem_x, (gem_z)) - init_pitch_y;
    
    // The 20 is a correctional factor to make the game more enjoyable. It was determined experimentally.
    a_x = sin(roll_x)*20/massa; 
    a_y = sin(pitch_y)*20/massa; 
    
    // Collision_handler(current_level, x, y, state, v_x, v_y); 
    calc_pos(a_x, a_y, v_x, v_y, x, y); 
    collision_handler(current_level, x, y, state, v_x, v_y, level); 

    // State can change here
    if(*state == 1){
        draw_level(current_level);
        draw_3x3_to_LCD((int)((*x)/10), (int)((*y)/10));
    }
    _delay_ms(100);
    clearLCD();
}

void level_lost_state(){
    printStringToLCD("Game over!", 0, 3);
    _delay_ms(1000);
    level = 1;   // Optional: player will return back to level 1 when dying 
    reset(x, y, v_x, v_y, state);
    setlevel(current_level, level_1);
    clearLCD();
}

void level_won_state(){
    printStringToLCD("You won!", 0, 4);
    _delay_ms(1000);
    clearLCD();
    reset(x, y, v_x, v_y, state);
    level++;
    if (level == 2) {
        setlevel(current_level, level_2);
    }
    else if (level == 3) {
        setlevel(current_level, level_3);
    }
    else if (level == 4) {
        setlevel(current_level, level_4);
    }
    else if (level == 5) {
        setlevel(current_level, level_5);
    }
    else if (level == 6) {
        *y = 650;  // Different starting position
        setlevel(current_level, level_6);
    }
    else if (level == 7) {
        setlevel(current_level, level_7);
    }
    else if (level == 8) {
        *y = 700;  // Different starting position
        setlevel(current_level, level_8);
    }
    else if (level == 9) {
        setlevel(current_level, level_9);
    }
    else if (level == 10) {
        setlevel(current_level, level_10);
    }
}

void game_won_state(){
    printStringToLCD("You won the game",0,0);
    printStringToLCD("Reset to restart",1,0);
}