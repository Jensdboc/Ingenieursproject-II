/*
 * Functions_game.h
 *
 *  Authors: Jens De Bock
 *           Wannes De Vos
 */

#ifndef FUNCTIONS_GAME_H 
#define FUNCTIONS_GAME_H

#include "dwenguino/dwenguino_lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

// Functions

/*
    @brief Initialize variables 
*/
void init();
/*
    @brief Calibrate accelerometer
*/
void calibrate();
/*
    @brief Shows title of current level
¨*/
void title_state();
/*
    @brief Handles all gamefeatures
¨*/
void game_state();
/*
    @brief Handles state when level is lost
¨*/
void level_lost_state();
/*
    @brief Handles state when level is won
¨*/
void level_won_state();
/*
    @brief Handles state when final level is won
¨*/
void game_won_state();

#endif //FUNCTIONS_GAME_H