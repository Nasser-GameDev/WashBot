// Author: 
// Net ID: 
// Date: 
//----------------------------------------------------------------------//
#include "lcd.h"
#include "timer.h"
#include <Arduino.h>
/*
 * Initializes all pins related to the LCD to be outputs
 */
void initLCDPins(){
  // set the data direction to output 
    DDRA |= 1 << DDA0; // set pin 22 data direction to output 
    DDRA |= 1 << DDA1; //set pin 23 data direction to output 
    DDRA |= 1 << DDA2; // set pin 24 data direction to output 
    DDRA |= 1 << DDA3; // set pin 25 data direction to output 

    // initially turn off all LED's 
    PORTA &= ~(1 << PORTA0); //  pin 22
    PORTA &= ~(1 << PORTA1); // pin 23 
    PORTA &= ~(1 << PORTA2); // pin 24  
    PORTA &= ~(1 << PORTA3); // pin 25

    DDRB |=  1 << DDB6; // Pin 12 : The register select ( RS bit ) 
    DDRB |=  1 << DDB4; // Pin 10 : The enable pin 

    // intially setting the volatage to Low.
    PORTB &= ~(1 << PORTB6); // turning off Pin 12  (RS pin)
    PORTB &= ~(1 << PORTB4); // turning off Pin 10
}

/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *      a. data is an unsigned char which has 8 bits. Therefore, you
 *         need assign the bottom 4 bits of "data" to the appropriate bits in
 *         PORTA
 *  2. This is a "command" signal, meaning RS should be Low
 *  3. Assert high on enable pin, delay, and asset low on enable pin
 *  4. delay the provided number in MICROseconds.
 */
void fourBitCommandWithDelay(unsigned char data, unsigned int delay){

  PORTA = data;
  PORTB &= ~(1 << PORTB6); // turning off Pin 12  ( The register select line)
  PORTB |= (1 << PORTB4); // turning on Pin 10 ( which is the enable pin)
  delayUs(1);
  PORTB &= ~(1 << PORTB4); // turning on Pin 10 ( which is the enable pin)
  delayUs(delay);
  
}

/* Similar to fourBitCommandWithDelay except that now all eight bits of command are
 * sent.
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *       a. unlike fourBitCommandWithDelay, you need to send the TOP four bits of
 *          "command" first. These should be assigned to appropriate bits in PORTA
 * 2. This is a command signal, meaning RS should be low
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of command to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay the provided number in MICROseconds.
 */
void eightBitCommandWithDelay(unsigned char command, unsigned int delay){
 
 // Keeping the upper four bits but writting that to the lower four bits of PortA
  PORTA = (command >> 4) & 0x0F;

  PORTB &= ~(1 << PORTB6); // turning off Pin 12  ( The register select line)
  PORTB |= (1 << PORTB4); // turning on Pin 10 ( which is the enable pin)
  // using the delay to give the lcd enough time to process our insturctions.
  delayUs(1);
  PORTB &= ~(1 << PORTB4); // turning on Pin 10 ( which is the enable pin)
  // using only 4 bits for the LCD out of our 8 bit portA and Command.
  PORTA = (command ) & 0x0F;
  PORTB |= (1 << PORTB4); // turning on Pin 10 ( which is the enable pin)
  delayUs(1); // delay one micro second 
  PORTB &= ~(1 << PORTB4); // turning on Pin 10 ( which is the enable pin)
 // 6. delay the provided number in MICROseconds.
  delayUs(delay); 
}

/* Similar to eightBitCommandWithDelay except that now RS should be high
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 * 2. This is a "data" signal, meaning RS should be high
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of character to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay is always 46 MICROseconds for a character write
 */
void writeCharacter(unsigned char character){
 // Keeping the upper four bits but writting that to the lower four bits of PortA
  PORTA = (character >> 4) & 0x0F;

  PORTB |= (1 << PORTB6); // turning on Pin 12  ( The register select line)
  PORTB |= (1 << PORTB4); // turning on Pin 10 ( which is the enable pin)
  // using the delay to give the lcd enough time to process our insturctions.
  delayUs(1);
  PORTB &= ~(1 << PORTB4); // turning on Pin 10 ( which is the enable pin)
  // using only 4 bits for the LCD out of our 8 bit portA and Command.
  PORTA = (character ) & 0x0F;
  PORTB |= (1 << PORTB4); // turning on Pin 10 ( which is the enable pin)
  delayUs(1);
  PORTB &= ~(1 << PORTB4); // turning on Pin 10 ( which is the enable pin)
  delayUs(46);

}

/*
 * Writes a provided string such as "Hello!" to the LCD screen. You should
 * remember that a c string always ends with the '\0' character and
 * that this should just call writeCharacter multiple times.
 */
void writeString(const char *string){
  while(*string != '\0')
  {
    writeCharacter(*string);
    string++;
  }
}

void clearScreen()
{
        moveCursor(0,0);
        writeString("               ");
        moveCursor(1,0);
        writeString("               ");
}
/*
 * This moves the LCD cursor to a specific place on the screen.
 * This can be done using the eightBitCommandWithDelay with correct arguments
 */
void moveCursor(unsigned char x, unsigned char y){
  // instruction given from the documentation page  
    // unsigned char command = 0x80 | (x << 6) | y;
    eightBitCommandWithDelay( 0x80 | (x << 6) | y, 46);
}


/* This is the procedure outline on the LCD datasheet page 4 out of 9.
 * This should be the last function you write as it largely depends on all other
 * functions working.
 */
void initLCDProcedure(){
  // Delay 15 milliseconds
    delayMs(15);
    // Write 0b0011 to DB[7:4] and delay 4100 microseconds
    fourBitCommandWithDelay(0b0011, 4100);
    // Write 0b0011 to DB[7:4] and delay 100 microseconds
    fourBitCommandWithDelay(0b0011, 100);

    // The data sheet does not make this clear, but at this point you are issuing
    // commands in two sets of four bits. You must delay after each command
    // (which is the second set of four bits) an amount specified on page 3 of
    // the data sheet.
    // write 0b0011 to DB[7:4] and 100us delay
    fourBitCommandWithDelay(0b0011, 100);

    // write 0b0010 to DB[7:4] and 100us delay.
    fourBitCommandWithDelay(0b0010, 100);

    // write 0b000101000 to DB[7:4] and 53 us delay.
     eightBitCommandWithDelay(0b00101000, 53);

    // Display off in the command table with 53us delay;
    // DB7: 0
    // DB6: 0
    // DB5: 0
    // DB4: 0
    // DB3: 1
    // DB2: D(cursor is ON when D=1, OFF when D=0)
    // DB1: C(cursor is displayed when C=1, not displayed when C=0)
    // DB0: B(cursor blinks when B=1, does not blink when B=0)
    eightBitCommandWithDelay(0b00001000, 53);

    // Clear display in the command table. Remember the delay is longer!!!
    // DB7: 0
    // DB6: 0
    // DB5: 0
    // DB4: 0
    // DB3: 0
    // DB2: 0
    // DB1: 0
    // DB0: 1
    eightBitCommandWithDelay(0b00000001, 3000);

    // Entry Mode Set in the command table.
    // DB7: 0
    // DB6: 0
    // DB5: 0
    // DB4: 0
    // DB3: 0
    // DB2: 1
    // DB1: I/D(specifies whether to increment(I/D=1) or decrement(I/D=0) the
    // address counter after subsequent DDRAM read or write operations)
    // DB0: S(shifting stuff we will not use, set it S=0)
    eightBitCommandWithDelay(0b00000110, 53);

    // Display ON/OFF Control in the command table. (Yes, this is not specified),
    // in the data sheet, but you have to do it to get this to work. Yay
    // datasheets!)
    // DB7: 0
    // DB6: 0
    // DB5: 0
    // DB4: 0
    // DB3: 1
    // DB2: D(cursor is ON when D=1, OFF when D=0)
    // DB1: C(cursor is displayed when C=1, not displayed when C=0)
    // DB0: B(cursor blinks when B=1, does not blink when B=0)
    eightBitCommandWithDelay(0b00001111, 53);

  }

/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins();
  initLCDProcedure();
}