# Bomberman-Clone

### About
This seminar project has been created as the project to a subject PA2 taught at the Czech Technical University in Prague. The task was to create a game similar to one of the most popular games in the world, the Bomberman. The program itself contains two game modes, a singleplayer and a multiplayer. In the beginning, each player has only one bomb and three lives. A bomb explosion is initially very limited. However, a player can collect various bonuses, which are randomly generated with a certain probability. These bonuses may add the player life, increase a bomb explosion, enable the player to walk through placed bombs, add a new bomb to a player's stack or make remote control a bomb possible. Each game lasts 200 seconds, then a playing area is reducing. Wins the player, who survives longer. Be aware that even own bombs can damage a player himself. Sometimes there is no direct path to another player, therefore a player has to make his own path by placing bombs in the right places. Thank you for playing the game.

### Author
Jan Steuer

### How to create a new map
Anybody may create a map but must observe some rules. Firstly, only a few characters are allowed to use. Here they are:

* `S` stands for a solid block
* `B` stands for a barrel
* ` ` (empty space) stands for an empty space
* `X` stands for a player1
* `Y` for a player2

Futhermore, a map must be FRAMED by SOLID BLOCKS and the shape of map must be a RECTANGLE. 

Game can be also modified by changing variables in the file `constants.cpp` in the folder `/src`. Read usage of all constants in this documentation. 

### How to start the game
To start a new game, type `cd <file path>` and then `make run` in terminal command line. To generate documentation, type `make doc`. To clean binary objects, type `make clean`.
