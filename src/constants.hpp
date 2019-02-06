#ifndef __CONSTANTS_HPP__
#define __CONSTANTS_HPP__

///path to a default map
extern const char* DEFAULT_MAP;

///path to a highscore file
extern const char* HIGHSCORE_FILE;

///time in seconds, when a map starts collapsing (optimal: 200, maximum 600)
extern const unsigned short TIME_LIMIT;

///default size of a bomb explosion (optimal: 2)
extern const unsigned short BASIC_BOMB_STRENGTH;

///default number of bombs at the beginning (optimal: 1)
extern const unsigned short TOTAL_BOMBS;

///default time, when a bomb explodes, 100 = 1 second (optimal: 200)
extern const unsigned short BOMB_TIME;

///default time of a duration of any explosion, 100 = 1 second (optimal: 50)
extern const unsigned short EXPLOSION_TIME;

///index of a color of a solid block
extern const unsigned short SOLID_COLOR;

///index of a color of a barrel block
extern const unsigned short BARREL_COLOR;

///index of a color of a placed bomb
extern const unsigned short BOMB_COLOR;

///index of a color of a bonus
extern const unsigned short BONUS_COLOR;

///index of a color of a player1
extern const unsigned short PLAYER1_COLOR;

///index of a color of a player2
extern const unsigned short PLAYER2_COLOR;

///default number of player's lives at the beginning (optimal: 4)
extern const unsigned short LIVES;

///probability with witch a new bonus is generated, 1:n (optimal: n = 8)
extern const unsigned short BONUS_CHANCE;

///char representing a "quit" key
extern const char QUIT;

///char representing a "player1 up" key
extern const char PLAYER1_UP;

///char representing a "player1 down" key
extern const char PLAYER1_DOWN;

///char representing a "player1 left" key
extern const char PLAYER1_LEFT;

///char representing a "player1 right" key
extern const char PLAYER1_RIGHT;

///char representing a "player1 bomb" key
extern const char PLAYER1_BOMB;

///char representing a "player1 remote-controlled bomb" key
extern const char PLAYER1_REMOTE;

///char representing a "player2 bomb" key
extern const char PLAYER2_BOMB;

///char representing a "player2 remote-controlled bomb" key
extern const char PLAYER2_REMOTE;

#endif
