#pragma once
typedef enum menuScreen_enum
{
  MainMenu,FilterStatus,WaterQuantity
}menuScreen;
void initSwitchPB3();
void initSwitchPD0();
void readButtonPressed();
void ChangeMenuUpOption(volatile  menuScreen* CurrentMenu);
void ChangeMenuDownOption(volatile  menuScreen* CurrentMenu);