/*
 * Main.c
 *
 *  Authors: Jens De Bock
 *           Wannes De Vos
 */

#include <avr/io.h>
#include <util/delay.h>
#include "dwenguino/dwenguino_board.h"
#include "dwenguino/dwenguino_lcd.h"
#include <avr/interrupt.h>

#include "dwenguino/functions_communication.h"
#include "dwenguino/functions_game.h"
#include "dwenguino/functions.h"
#include <math.h>

// Initialize state
unsigned char state_val = 0;
unsigned char* state = &state_val;

// Interrupts for changing mass
int massa = 1;
int antibounce = 0;

ISR(INT6_vect) {
   if(massa < 3 && antibounce == 0){
   massa += 1;
   antibounce = 3;
   }
}
ISR(INT4_vect) {
   if(massa > 1 && antibounce == 0){
     massa -= 1;
     antibounce = 3;

   }
}

int main(void)
{ 
  // Initialize board
  initBoard();
  initLCD();
  clearLCD();
  backlightOn();
  
  // Initialize registers
  init_interrupts();

  // Set sleepregister to 0
  write_register(0x6B, 0);  

  // Initialize the game
  init();
  calibrate();

  // Gameloop
  while(1) {   
    if (antibounce != 0){
      antibounce -= 1;
    }
    // Show startscreen
    if (*state == 0) {
      title_state();
    } 
    // Game is being played
    else if (*state == 1) {
      game_state();
    } 
    // Level has failed
    else if (*state == 2) {
      level_lost_state();
    }
    // Level has been completed
    else if (*state == 3) {
      level_won_state();
    }
    // Show final winscreen
    else if (*state == 4) {
      game_won_state();
    }
  }
  return 0;
}