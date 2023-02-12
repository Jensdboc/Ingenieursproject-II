/*
 * Functions_communication.c
 *
 *  Authors: Jens De Bock
 *           Wannes De Vos
 */

#include "dwenguino/functions_communication.h"
#include "dwenguino/dwenguino_lcd.h"
#include "dwenguino/dwenguino_board.h"

int low_plus_high_bits(char lowbits, char highbits){
  int high = (((int) highbits) << 8);
  return (long int)(((int) lowbits) + high);
}

long int calc_tot_x(void){
    int aantal = 50;
    char value1, value2;
    long int totaalx = 0;
    for(int i = 0; i < aantal; i++){
        // Reads both x-registers
        value1 = read_register(0x3B);
        value2 = read_register(0x3C);
        totaalx += low_plus_high_bits(value2, value1);
    }
    return totaalx/aantal;
}

long int calc_tot_y(void){
    int aantal = 50;
    char value1, value2;
    long int totaaly = 0;
    for(int i = 0; i < aantal; i++){
        // Reads both y-registers
        value1 = read_register(0x3D);
        value2 = read_register(0x3E);
        totaaly += low_plus_high_bits(value2, value1);
    }
    return totaaly/aantal;
}

long int calc_tot_z(void){
    int aantal = 50;
    char value1, value2; 
    long int totaalz = 0;
    for(int i = 0; i < aantal; i++){
        // Reads both z-registers
        value1 = read_register(0x3F);
        value2 = read_register(0x40);
        totaalz += low_plus_high_bits(value2, value1);
    }
    return totaalz/aantal;
}

char read_register(unsigned char registergetal) {
  communication com;
  // Try to read register 50 times 
  for(int i = 0; i < 5; i++){
    com.ret_value = 0;
    com.error = 0;
    // Send START condition
    // TWINT, TWSTA and TWEN is cleared 
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    // Wait for TWINT Flag Set  

    while (!(TWCR & (1<<TWINT))){}; 
    // Check value of TWI Status 
    if ((TWSR & 0xF8) != START) {  
      com.error = 1;
      continue;
    }
    // Slave adress
    TWDR = SLA_W;       
    // TWINT and TWEN is cleared          
    TWCR = (1<<TWINT)|(1<<TWEN);  
    // Wait for TWINT Flag Set
    while (!(TWCR & (1<<TWINT))){}; 
    if ((TWSR & 0xF8) != MT_SLA_ACK) {
      com.error = 2;
      continue;
    }

    // Register ACCEL_XOUT (0x3B) Standard value = SLA_W
    TWDR = registergetal;   
    // TWINT and TWEN is cleared             
    TWCR = (1<<TWINT)|(1<<TWEN);  
    // Wait for TWINT Flag Set
    while (!(TWCR & (1<<TWINT))){};  
    if ((TWSR & 0xF8) != MT_DATA_ACK) {  
      com.error = 3;
      continue;
    }

    // TWINT, TWSTA and TWEN is cleared 
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    // Wait for TWINT Flag Set
    while (!(TWCR & (1<<TWINT))){}; 
    // Check value of TWI Status
    if ((TWSR & 0xF8) != REP_START) { 
      com.error = 4;
      continue;
    }

    TWDR = SLA_R;
    // TWINT and TWEN is cleared
    TWCR = (1<<TWINT)|(1<<TWEN);
    // Wait for TWINT Flag Set
    while (!(TWCR & (1<<TWINT))){};  
    if ((TWSR & 0xF8) != MR_SLA_ACK)  {    
      com.error = 5;
      continue;
    }

    // TWINT and TWEN is cleared
    TWCR = (1<<TWINT)|(1<<TWEN);
    // Wait for TWINT Flag Set
    while (!(TWCR & (1<<TWINT))){}; 
    if ((TWSR & 0xF8) != MR_DATA_NACK)  {    
      com.error = 6;
      continue;
    } 

    com.ret_value = TWDR;
    // Transmit STOP condition: TWINT, TWSTA and TWEN is cleared
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);   
    
    if (com.error == 0){
      return com.ret_value;
    }
  }
  clearLCD();
  printStringToLCD("Error",0,4);
  printIntToLCD((int)com.error,0,10);
  printStringToLCD("Press reset",1,2);
  _delay_ms(500);
  exit(1);
}
  
void write_register(unsigned char registergetal, unsigned char waarde) {
    unsigned char error;
    // Try to write to register 50 times 
    for (int i = 0; i < 50; i++) {
      error = 0;
      // Send START condition
      // TWINT, TWSTA and TWEN is cleared 
      TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
      // Wait for TWINT Flag Set
      while (!(TWCR & (1<<TWINT))); 
      // Check value of TWI Status
      if ((TWSR & 0xF8) != START) {  
        error = 1;
        continue;
      }
      // Slave adress
      TWDR = SLA_W;        
      // TWINT and TWEN is cleared         
      TWCR = (1<<TWINT)|(1<<TWEN);  
      // Wait for TWINT Flag Set  
      while (!(TWCR & (1<<TWINT))); 
      if ((TWSR & 0xF8) != MT_SLA_ACK) {
        error = 2;
        continue;
      }
      // Register ACCEL_XOUT (0x3B) Standard value = SLA_W
      TWDR = registergetal; 
      // TWINT and TWEN is cleared            
      TWCR = (1<<TWINT)|(1<<TWEN);  
      // Wait for TWINT Flag Set
      while (!(TWCR & (1<<TWINT))); 
      if ((TWSR & 0xF8) != MT_DATA_ACK) {  
        error = 3;
        continue;
      }
      // Setting everything equal to zero
      TWDR = waarde;          
      // TWINT and TWEN is cleared          
      TWCR = (1<<TWINT)|(1<<TWEN);
      // Wait for TWINT Flag Set
      while (!(TWCR & (1<<TWINT))); 
      // Check value of TWI Status
      if ((TWSR & 0xF8) != MT_DATA_ACK) { 
        error = 4;
        continue;
      }
      // Transmit STOP condition: TWINT, TWSTA and TWEN is cleared 
      TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); 

      // Leave for-loop is errorvalue is 0, otherwise keep going
      if (error == 0) {
        return;
      }
    }
    // Print errormessage and exit 
    clearLCD();
    printIntToLCD((int) error,0,10);
    printStringToLCD("Error",0,4);
    printStringToLCD("Please reset",1,2);
    _delay_ms(500);
    exit(1);
  }

  void init_interrupts(){
  // Initialize registers
  DDRA |= 0xFF;
  DDRD |= (_BV(PD0) | _BV(PD1));
  PORTD |= (_BV(PD0) | _BV(PD1));   

  // Interrupts
  DDRE &= ~(1<<PE4); // Set PE4 to input
  PORTE |= (1<<PE4); // Activate PE4 pull-up

  SREG |= (1<<SREG_I); // Allow interrupts to occur
  EIMSK |= (1<<INT4);  // Enable INT4

  // Configure INT5 to occur on falling edge
  EICRB |= (_BV(ISC41)); //ISC41 bit set
  EICRB &= ~(_BV(ISC40)); // ISC40 bit cleared

  DDRE &= ~(1<<PE6); // Set PE6 to input
  PORTE |= (1<<PE6); // Activate PE6 pull-up

  SREG |= (1<<SREG_I); // Allow interrupts to occur
  EIMSK |= (1<<INT6);  // Enable INT6

  // Configure INT5 to occur on falling edge
  EICRB |= (_BV(ISC61)); //ISC61 bit set
  EICRB &= ~(_BV(ISC60)); // ISC60 bit cleared
}