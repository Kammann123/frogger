# frogger

Frogger game for Ubuntu and Raspberry Pi, developed in C language.

## Main menu screen
<img src="./screenshots/mainmenu.png">

## Game screen
<img src="./screenshots/gamescreen.png">

## Pause menu screen
<img src="./screenshots/pausemenu.png">

# Requirements

For Ubuntu: installing Allegro C Library

For RaspberryPi: having display and joystick modules
  
# Getting the binary file

For ubuntu: execute on terminal: "make -f frogger_mk allegro"
  
For RaspberryPi: execute on terminal: "make -f frogger_mk rpi"
  
# Executing the binary

The binary can be executed on terminal "./frogger" and some arguments may be added. These arguments will be taken as commands
with the following syntax:
  "-command=value"
 
So far, Frogger supports these commands:
  1. -name=HereGoesUsersName : It allows the user to set username to save it's score in the top score table.
  2. -character=characterIndex : It allows the user to change the game character, it depends on how many options there are.
  3. -map=mapIndex : It allows the user to change the game map being used, it depends on how many options available there are.