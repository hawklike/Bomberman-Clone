#include <ncurses.h>

#include "human.hpp"

CHuman::CHuman(unsigned short lives, bool first)
: CPlayer(lives, first) {}

void CHuman::handleInput(int ch, BLOCK_MAP &map, BONUS_VECTOR &bonuses, const shared_ptr<CPlayer> player)
{
  if(!player->isFirst())
  {
    if(ch == KEY_LEFT)        controlX(-1, map);
    if(ch == KEY_RIGHT)       controlX(+1, map);
    if(ch == KEY_UP)          controlY(-1, map);
    if(ch == KEY_DOWN)        controlY(+1, map);
    if(ch == PLAYER2_BOMB)    placeBomb(map);
    if(ch == PLAYER2_REMOTE)  placeRemoteBomb(map);
  }
  else
  {
    if(ch == PLAYER1_LEFT)    controlX(-1, map);
    if(ch == PLAYER1_RIGHT)   controlX(+1, map);
    if(ch == PLAYER1_UP)      controlY(-1, map);
    if(ch == PLAYER1_DOWN)    controlY(+1, map);
    if(ch == PLAYER1_BOMB)    placeBomb(map);
    if(ch == PLAYER1_REMOTE)  placeRemoteBomb(map);
  }
}

void CHuman::controlX(short int dx, const BLOCK_MAP &map)
{
  if(testMove(posX + dx, posY, map)) posX += dx;
}

void CHuman::controlY(short int dy, const BLOCK_MAP &map)
{
  if(testMove(posX, posY + dy, map)) posY += dy;
}

bool CHuman::isFirst() const { return first; }
