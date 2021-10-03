#include <avr/io.h>
#include "switch.h"
void initSwitchPB3()
{
  /*   --- External Note --- 
   it's just a concidance that portB3 happened to line up with our interrupt INT3
  we notice that with switch PD0 lines up with register ISC01 = 0 and ISC00 = 1
  */
  DDRB &= ~(1<<DDB3); // Making our PB3 button function as input
  PORTB |= (1<<PORTB3); //SET pin 50 which is PB3 | enable pull up resistors 
  PCICR |= (1<<PCIE0); // ENABLE PCINT 0-7
  // note we don't have to worry about generate ansynchoronous interrupt request
  // since register PCINT does that behind the scenes
  PCMSK0 |= (1<<PCINT3); // ENABLE PCINT3
}

void initSwitchPD0() 
{
 // setting pin 21 on the arudiono board to be input signal
    DDRD &= ~(1 << DDD0); // setting pin 21 on arduino board to be an input signal
    // to enable its pull up resistors
    PORTD |= 1 << PORTD0; // to enable pull up resistors  
     // when you hit the button it activates the interrupts 
    // button up since an "edge" (raising or falling of clock ) means the buttons has just been pressed or just been releases
    //  Any edge of INTn generates asynchronously an interrupt request 
  
    EICRA &= ~(1<< ISC01); // setting it up to have INT0 interrupt 
    EICRA |= (1<< ISC00);  
    
    EIMSK |= (1<< INT0); // the bit masking
}

void ChangeMenuUpOption(volatile  menuScreen* CurrentMenu) 
{
 // bit masking all the bits other than the one we care about
    int UpBit = (PINB >> PINB3) & 1;
    int DownBit = (PIND >> PIND3) & 1;
   // or 
  //  int bit = (1<<PINA0) & PINA;
  // this means our up button was pushed 
    if (UpBit == 0)
     {
      // if switch is pressed, input goes to zero
      if(*CurrentMenu == MainMenu)
      {
        *CurrentMenu = FilterStatus;
      }
      else if (*CurrentMenu == FilterStatus)
      {
        *CurrentMenu =  WaterQuantity;
      }
      else if (*CurrentMenu == WaterQuantity)
      {
        *CurrentMenu = MainMenu;
      }
    }
    else if(DownBit == 0)
      {
        // if switch is pressed, input goes to zero
          if(*CurrentMenu == MainMenu)
          {
            *CurrentMenu = WaterQuantity;
          }
          else if (*CurrentMenu == FilterStatus)
          {
            *CurrentMenu =  MainMenu;
          }
          else if (*CurrentMenu == WaterQuantity)
          {
            *CurrentMenu = FilterStatus;
          }
      }
    
    
}
void ChangeMenuDownOption(volatile  menuScreen* CurrentMenu) 
{
    int DownBit = (PIND >> PIND3) & 1;
    if(DownBit == 0)
      {
        // if switch is pressed, input goes to zero
          if(*CurrentMenu == MainMenu)
          {
            *CurrentMenu = WaterQuantity;
          }
          else if (*CurrentMenu == FilterStatus)
          {
            *CurrentMenu =  MainMenu;
          }
          else if (*CurrentMenu == WaterQuantity)
          {
            *CurrentMenu = FilterStatus;
          }
      }
      
}