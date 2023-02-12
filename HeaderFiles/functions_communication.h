/*
 * Functions_communication.h
 *
 *  Authors: Jens De Bock
 *           Wannes De Vos
 */

#ifndef FUNCTIONS_COMM_H 
#define FUNCTIONS_COMM_H

#include "dwenguino/dwenguino_lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

// Macro's

#define START 0x08
#define SLA_W 0b11010000
#define MT_SLA_ACK 0x18
#define DATA 0x75
#define MT_DATA_ACK 0x28
#define SLA_R 0b11010001
#define MR_SLA_ACK 0x40
#define MR_DATA_NACK 0x58
#define REP_START 0x10

/*  
    @struct communication
    @brief This structure is used for reading from the accelerometer
    @param communication::ret_value   Value after reading registers
    @param communication::error       Number of error 
*/
typedef struct communication{
    char ret_value;
    unsigned char error;
}communication;

/*
    @brief Puts high bits after low bits
    @param lowbits      First bits
    @param highbits     Second bits
    @return concatenation of bits
*/
int low_plus_high_bits(char lowbits, char highbits);
/*
    @brief Calculates average x-value
    @return average x-value
*/
long int calc_tot_x(void);
/*
    @brief Calculates average y-value
    @return average y-value
*/
long int calc_tot_y(void);
/*
    @brief Calculates average z-value
    @return average z-value
*/
long int calc_tot_z(void);
/*
    @brief Read register
    @param registergetal    Number of register
    @return read value
*/
char read_register(unsigned char registergetal);
/*
    @brief Writes to register
    @param registergetal    Number of register
    @param waarde           Value that will be written to register          
*/
void write_register(unsigned char registergetal, unsigned char waarde);
/*
    @brief Initialize all registers and interrupts
*/
void init_interrupts();

#endif //FUNCTIONS_COMM_H