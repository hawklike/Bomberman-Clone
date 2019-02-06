#include <ncurses.h>
#include <unistd.h>
#include <iostream>

#include "game.hpp"
#include "human.hpp"
#include "computer.hpp"

/**
 * @mainpage The Bomberman Clone
 *
 * @section About
 *
 * @detail This seminar paper has been created as a project to the subject PA2 taught
 * at the Czech Technical University in Prague. The task was to create a game similar to
 * one of the most popular games in the world, the Bomberman. The program itself contains
 * two game modes, a singleplayer and a multiplayer. At the beginning, each player has only one
 * bomb and three lifes. A bomb explosion is initially very limitated. However, a player can
 * collect various bonuses, which are randomly generated with a certain probability. These
 * bonuses may add the player life, increase a bomb explosion, enable the player to walk through
 * placed bombs, add a new bomb to a player's stack or make remote control a bomb possible.
 * Each game lasts 200 seconds, then a playing area is reducing. Wins the player, who
 * survives longer. Be aware that even own bombs can damage a player himself. Sometimes
 * there is no direct path to other player, therefore a player has to make his own path
 * by placing bombs at the right places. Thank you for playing the game.
 *
 * @author Jan Steuer
 *
 * @section How How to create map
 *
 * Anybody may create a map, but must observe some rules. Firstly, only few characters
 * are allowed to use. Here they are: \n
 * * 'S' stands for a solid block \n
 * * 'B' stands for a barrel \n
 * * ' ' (empty space) stands for an empty space \n
 * * 'X' stands for a player1 \n
 * * 'Y' for a player2 \n
 *
 * Futhermore, a map must be FRAMED by SOLID BLOCKS and shape of a map must be a RECTANGLE.
 * \n \n
 * Game can be also modified by changing variables in the file 'constants.cpp' in the folder
 * '/src'. Read usage of all constants in this documentation.
 */

bool start();

int main(int argc, char* argv[])
{
  string mapAddress = DEFAULT_MAP;
  if(argc == 1)
  {
    cout<<"You will play the game on a default map. If you'd like to play your custom map, add an argument with a path to your map while launching the program.\nPress ENTER to continue."<<endl;
    getchar();
  }
  else mapAddress = argv[1];

  if(argc > 2)
  {
    cout<<"Too many arguments given. Run without an argument or give a path to your custom map as the only argument."<<endl;
    exit(0);
  }

  if(TIME_LIMIT > 600)
  {
    cout<<"Game that long would be boring."<<endl;
    exit(0);
  }

  initscr();
	noecho();

  start_color();
  if(!has_colors()) { endwin(); cout<<"Your terminal does not support colors."<<endl; exit(0); }

  init_pair(SOLID_COLOR, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(BARREL_COLOR, COLOR_WHITE, COLOR_GREEN);
  init_pair(BOMB_COLOR, COLOR_WHITE, COLOR_YELLOW);
  init_pair(BONUS_COLOR, COLOR_WHITE, COLOR_CYAN);
  init_pair(PLAYER1_COLOR, COLOR_WHITE, COLOR_BLUE);
	init_pair(PLAYER2_COLOR, COLOR_WHITE, COLOR_RED);

  cbreak();
	keypad(stdscr, true);
	curs_set(false);

  bool singleplayer = start();
  nodelay(stdscr, true);

  unique_ptr<CGame> game;
  try
  {
    if(singleplayer)
      game = make_unique<CGame>(make_shared<CHuman>(LIVES, true), make_shared<CComputer>(LIVES, false), mapAddress);
    else
      game = make_unique<CGame>(make_shared<CHuman>(LIVES, true), make_shared<CHuman>(LIVES, false), mapAddress);
  }
  catch (const invalid_argument & expr) { endwin(); cout<<expr.what()<<endl; exit(0); }

  while(game->isRunning())
  {
    game->update();
    game->draw();
    usleep(10000);
  }

  endwin();
  return 0;
}

bool start()
{
  mvprintw(2,7,"  ____   ____  __  __ ____  ______ _____  __  __          _   _ ");
  mvprintw(3,7," |  _ \\ / __ \\|  \\/  |  _ \\|  ____|  __ \\|  \\/  |   /\\   | \\ | |");
  mvprintw(4,7," | |_) | |  | | \\  / | |_) | |__  | |__) | \\  / |  /  \\  |  \\| |");
  mvprintw(5,7," |  _ <| |  | | |\\/| |  _ <|  __| |  _  /| |\\/| | / /\\ \\ | . ` |");
  mvprintw(6,7," | |_) | |__| | |  | | |_) | |____| | \\ \\| |  | |/ ____ \\| |\\  |");
  mvprintw(7,7," |____/ \\____/|_|  |_|____/|______|_|  \\_\\_|  |_/_/    \\_\\_| \\_|");

  mvprintw(10, 30, "made by Jan Steuer");
  attron(A_BOLD);
  mvprintw(13, 15, "BLUE PLAYER");
  mvprintw(13, 55, "RED PLAYER");
  attroff(A_BOLD);
  mvprintw(15, 15, "arrows");
  mvprintw(16, 15, "spacebar...bomb");
  mvprintw(17, 15, "b...remote bomb");
  mvprintw(18, 15, "q...quit game");
  mvprintw(15, 55, "w...up");
  mvprintw(16, 55, "s...down");
  mvprintw(17, 55, "a...left");
  mvprintw(18, 55, "d...right");
  mvprintw(19, 55, "z...bomb");
  mvprintw(20, 55, "x...remote bomb");
  mvprintw(23, 5, "press left arrow for singleplayer, or press right arrow for multiplayer");
  refresh();

  bool flag;
  do
  {
    int ch = getch();
    if(ch == KEY_LEFT)  { flag = true; break;  }
    if(ch == KEY_RIGHT) { flag = false; break; }
    else
    {
      clear();
      mvprintw(10, 20, "Please, press only left or right arrow.");
    }
  } while(true);

  clear();
  return flag;
}
