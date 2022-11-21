# PINBALL

## Description

Pinball game created for Physics II subject.

## Key Features

 - {F1} - View logic/colliders + enable mouse joints.
 - {R} - Reset ball's position
 - {G} - Activate/Deactivate God Mode

## Debug keys
  - {U} - Adds +1 FPS
  - {J} - Reduces -1 FPS
  - {I} - Adds +0.1 Gravity
  - {K} - Reduces -0.1 Gravity
  - {O} - Adds +0.1 Restitution Coefficient
  - {L} - Reduces -0.1 Restitution Coefficient

## Controls

 - {LEFT Arrow} - Move left flipper.
 - {RIGHT Arrow} - Move right flipper.
 - {DOWN Arrow} - Action the ball kicker.

## Developers

 ![](https://github.com/Historn/PinBall_Game/blob/master/TeamPhotos/arnaujimenez.png)
 - [Arnau Jiménez Gallego](https://github.com/Historn) - CODE 
 
 ![](https://github.com/Historn/PinBall_Game/blob/master/TeamPhotos/adriapons.jpg)
 - [Adrià Pons Mensa](https://github.com/AdriaPm) - CODE & ART
 
 ![](https://github.com/Historn/PinBall_Game/blob/master/TeamPhotos/rylangraham.jpg)
 - [Rylan Graham](https://github.com/RylanJGraham) - CODE
 
 ![](https://github.com/Historn/PinBall_Game/blob/master/TeamPhotos/joelchaves.jpg)
 - [Joel Chaves Moreno](https://github.com/JoeyCM) - CODE
 
 ## Github repository
  Click [here](https://github.com/Historn/PinBall_Game) to go to our project's Github repository.
 
## Fixed Bugs

Ball reset after touching collider crashes becuase is inside the death collider. FIX --> Wait an amount of ms before reseting ball position.

Ball stuck in bouncers doing the same path. FIX --> For an amount of bounces changes normal impulse direction for changing the path.

Flippers stuck after using them. FIX --> Change the flippers & joints position so non of their bodies collide with anything else when using them. 

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](https://mit-license.org/) for further details.
