# Asteroids

<div align="center">
  <img src="./gif/demo.gif" alt="demo">
</div>

Asteroids is a Star Wars themed remake of the classic game Asteroids, made in C++ with the SFML library. In this game, the player controls a spaceship and must navigate through space while avoiding asteroids and destroying enemy ships.

## Installation

To run this game on your computer, you will need to have SFML installed. Here are the steps to install SFML and run this game:

* Download SFML from the official website [here](https://www.sfml-dev.org/download.php).
* Select the appropriate version for your operating system and extract the files to a directory of your choice.
* Download the source code for this game directly from this repository, or by cloning it using the command line:
```bash
git clone https://github.com/whazami/Asteroids.git
```
* Open the project in your preferred C++ IDE and configure the build settings to link SFML. You can find instructions for linking SFML in various C++ IDEs [here](https://www.sfml-dev.org/tutorials/2.5/start-vc.php).
* Build the project.

Alternatively, Windows users can test the game by running the Asteroids.exe file, located in the "bin" folder.

## How to Play

You can control your spaceship using the mouse and keyboard or a gamepad, here's how:

<table>
  <tr>
    <td></td>
    <th>Move</th>
    <th>Rotate</th>
    <th>Fire</th>
  </tr>
  <tr>
    <th>Mouse & Keyboard</th>
    <td align="center">WASD/ZQSD keys</td>
    <td align="center">Move mouse pointer</td>
    <td align="center">Left click</td>
  </tr>
  <tr>
    <th>Gamepad</th>
    <td align="center">Left joystick</td>
    <td align="center">Right joystick</td>
    <td align="center">Right trigger</td>
  </tr>
</table>

The game ends if you run out of lives or if the countdown timer reaches zero.

## Troubleshooting

If you compile the source code and you get a black window but no error, you may have to modify the `assetsPath` constant located in `include/Utils/Constantes.h` to match your file tree.

The vibration of the gamepad can cause the game to freeze/crash if it is connected to the PC via a wired connection due to underpowering. You can deactivate it from the pause menu or connect your gamepad via bluetooth.

## Credits

This game was created by Waël Hazami. Special thanks to the creators of SFML for providing the graphics and audio libraries used in this game.
