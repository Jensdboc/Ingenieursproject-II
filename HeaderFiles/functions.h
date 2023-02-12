/*
 * Functions.h
 *
 *  Authors: Jens De Bock
 *           Wannes De Vos
 */

#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

#include "dwenguino/dwenguino_lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

// Macro's

#define FULL_BLOCK 0xFF
#define GOAL "o"

#define HEIGHT 2
#define LENGTH 16

#define BOVEN 1
#define ONDER 2
#define RECHTS 3
#define LINKS 4

#define EMPTY_CHAR {0b00000, 0b00000 ,0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000}

// Functions

/*
    @brief Shows startscreen
*/
void begin_LCD();
/*
    @brief Puts character pattern in the provided place
    @param Pattern  The character pattern
    @param nummer   Number for the place of the character
*/
void custom_char(unsigned char *Pattern, const char nummer);
/*
    @brief Shifts bits 1 place to the right, fill in with a 1 on the leftside
    @param a    Bit that has to get shifted
    @return shifted bits
*/
unsigned char bits_r1(unsigned char a);
/*
    @brief Calculates new x- and y-position and x- and y-velocity
    @param a_x  Acceleration in the x-direction
    @param a_y  Acceleration in the y-direction
    @param v_x  Velocity in the x-direction
    @param v_y  Velocity in the y-direction
    @param x    Position of x-coordinate
    @param y    Position of y-coordinate
*/
void calc_pos(int a_x, int a_y, int* v_x, int* v_y, int* x, int* y);
/*
    @brief draws 3x3 block to screen
    @param x_pos    Position of x-coordinate of the middle
    @param y_pos    Position of y-coordinate of the middle
*/
void draw_3x3_to_LCD(int x_pos, int y_pos);
/*
    @brief Draws current level to LCD-screen
    @param level    List with coordinates of objects in current level
*/
void draw_level(unsigned char level[HEIGHT][LENGTH]);
/*
    @brief Checks for possible collisions
    @param level            List with coordinates of objects in current level
    @param x                Position of x-coordinate
    @param y                Position of y-coordinate
    @param state            Current state
    @param v_x              Velocity in the x-direction
    @param v_y              Velocity in the y-direction   
    @parem level_number     Current level number
*/
void collision_handler(unsigned char level[HEIGHT][LENGTH], int* x, int* y, unsigned char* state, int* v_x, int* v_y, unsigned char level_number);
/*
    @brief Sets current level
    @param current_level    List that will be filled in with coordinates of objects in current level
    @param level            Level that current_levels has to be set to
*/
void setlevel(unsigned char current_level[HEIGHT][LENGTH], unsigned char level[HEIGHT][LENGTH]);
/*
    @brief Resets all values to startpositions
    @param x        Position of x-coordinate
    @param y        Position of y-coordinate
    @param v_x      Velocity in the x-direction
    @param v_y      Velocity in the y-direction
    @param state    Current state
*/
void reset(int* x, int* y, int* v_x, int* v_y, unsigned char* state);

#endif //FUNCTIONS_H