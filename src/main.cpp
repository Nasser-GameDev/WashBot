#include <avr/io.h>
#include <avr/interrupt.h>  
#include <Arduino.h>
#include "switch.h"
#include "lcd.h"
#include "timer.h"
typedef enum stateType_enum {
  wait_press, debounce_press, wait_release, debounce_release
} stateType; // defining this so that we can use it for the button press sensitivity
typedef enum error_enum {
  AllGood,DangerousButStillGood,ExitImmediately
}errorStatus;

volatile menuScreen MenuScreen = MainMenu; // set the LCD Screen to intially be the main menu 
// volatile makes it so that every read comes from main memory instead of the cash memory
volatile stateType ButtonPressedStateUp = wait_press; // start at a wait state
volatile stateType ButtonPressedStateDown = wait_press; // start at a wait state
volatile errorStatus ErrorStatus; // figure out what will go wrong later.

void SetUp()
{
  // intializing 
  sei(); // to enable global iterrupts 
  initTimer0();
  initTimer1();
  initSwitchPB3();
  initSwitchPD0();
  initLCD();
}
int main()
{
  bool WashBotActivated = true;
  Serial.begin(9600); 
  SetUp();
    moveCursor(0, 0);
    writeString("WELCOME TO");
    moveCursor(1, 0);
    writeString("WASHBOT!!!!");
    while(WashBotActivated)
    {
      switch (ButtonPressedStateUp)
      {
        case wait_press:
          break;
        case debounce_press:
          delayMs(1); // wait for one milisecond 
          ButtonPressedStateUp = wait_release;
        break;
        case wait_release:
          break;
        case debounce_release:
          delayMs(1);
          ButtonPressedStateUp = wait_press;
          break;
      }
      if(ErrorStatus == ExitImmediately)
      {
        return 911;
      }
    switch (ButtonPressedStateDown)
        {
          case wait_press:
            break;
          case debounce_press:
            delayMs(1); // wait for one milisecond 
            ButtonPressedStateDown = wait_release;
          break;
          case wait_release:
            break;
          case debounce_release:
            delayMs(1);
            ButtonPressedStateDown = wait_press;
            break;
        }
        if(ErrorStatus == ExitImmediately)
        {
          return 911;
        }
      
    }
}

// the Interrupt for pin 51 
ISR(PCINT0_vect)
{

    switch (MenuScreen)
    {
      case MainMenu:
      // make sure to clear the screen before printing a message 
        clearScreen();
        moveCursor(0, 0);
        writeString("WELCOME TO");
        moveCursor(1, 0);
        writeString("WASHBOT!!!!");
        break;
      case WaterQuantity:
      // make sure to clear the screen before printing a message 
        clearScreen();
        moveCursor(0, 0);
        writeString("There is one");
        moveCursor(1, 0);
        writeString("Liter Of Water!");
        break;
      case FilterStatus:
      // make sure to clear the screen before printing a message 
        clearScreen();
        moveCursor(0, 0);
        writeString("Water Filter");
        moveCursor(1, 0);
        writeString("Status is Good");
        break;
    }
  
  if(ButtonPressedStateDown == wait_press)
  {
    ButtonPressedStateDown = debounce_press;
  }
  else if (ButtonPressedStateDown == wait_release)
  {
    // instead of polling we use an interrupt to call our function 
   ChangeMenuUpOption(&MenuScreen);
   ButtonPressedStateDown = debounce_release;
  }

}

// The Interrupt for pin 21  
ISR(INT0_vect)
{
  
    switch (MenuScreen)
    {
      case MainMenu:
      // make sure to clear the screen before printing a message 
        clearScreen();
        moveCursor(0, 0);
        writeString("WELCOME TO");
        moveCursor(1, 0);
        writeString("WASHBOT!!!!");
        break;
      case WaterQuantity:
      // make sure to clear the screen before printing a message 
        clearScreen();
        moveCursor(0, 0);
        writeString("There is one");
        moveCursor(1, 0);
        writeString("Liter Of Water!");
        break;
      case FilterStatus:
      // make sure to clear the screen before printing a message 
        clearScreen();
        moveCursor(0, 0);
        writeString("Water Filter");
        moveCursor(1, 0);
        writeString("Status is Good");
        break;
    }
  
  if(ButtonPressedStateDown == wait_press)
  {
    ButtonPressedStateDown = debounce_press;
  }
  else if (ButtonPressedStateDown == wait_release)
  {
    // instead of polling we use an interrupt to call our function 
   ChangeMenuDownOption(&MenuScreen);
   ButtonPressedStateDown = debounce_release;
  }
}