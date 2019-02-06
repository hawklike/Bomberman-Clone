#include <ncurses.h>

#include "bonus.hpp"

CBonus::CBonus(unsigned short posX, unsigned short posY)
: posX(posX), posY(posY) {}

unsigned short CBonus::getPosX() const { return posX; }

unsigned short CBonus::getPosY() const { return posY; }

CBiggerExplosion::CBiggerExplosion(unsigned short posX, unsigned short posY)
: CBonus(posX, posY) {}

void CBiggerExplosion::print() const
{
  attron(COLOR_PAIR(BONUS_COLOR) | A_BOLD);
  mvaddch(posY, posX, 'E');
  attroff(COLOR_PAIR(BONUS_COLOR) | A_BOLD);
}

void CBiggerExplosion::getBonus(CPlayer* player) const
{
  player->incStrengthBomb();
}

CBombPassage::CBombPassage(unsigned short posX, unsigned short posY)
: CBonus(posX, posY) {}

void CBombPassage::print() const
{
  attron(COLOR_PAIR(BONUS_COLOR) | A_BOLD);
  mvaddch(posY, posX, 'P');
  attroff(COLOR_PAIR(BONUS_COLOR) | A_BOLD);
}

void CBombPassage::getBonus(CPlayer* player) const
{
  player->setPassBombs();
}

CAddLive::CAddLive(unsigned short posX, unsigned short posY)
: CBonus(posX, posY) {}

void CAddLive::print() const
{
  attron(COLOR_PAIR(BONUS_COLOR) | A_BOLD);
  mvaddch(posY, posX, 'L');
  attroff(COLOR_PAIR(BONUS_COLOR) | A_BOLD);
}

void CAddLive::getBonus(CPlayer* player) const
{
  player->incLives();
}

CAddBomb::CAddBomb(unsigned short posX, unsigned short posY)
: CBonus(posX, posY) {}

void CAddBomb::print() const
{
  attron(COLOR_PAIR(BONUS_COLOR) | A_BOLD);
  mvaddch(posY, posX, 'B');
  attroff(COLOR_PAIR(BONUS_COLOR) | A_BOLD);
}

void CAddBomb::getBonus(CPlayer* player) const
{
  player->incNumBombs();
}

CRemoteBomb::CRemoteBomb(unsigned short posX, unsigned short posY)
: CBonus(posX, posY) {}

void CRemoteBomb::print() const
{
  attron(COLOR_PAIR(BONUS_COLOR) | A_BOLD);
  mvaddch(posY, posX, 'R');
  attroff(COLOR_PAIR(BONUS_COLOR) | A_BOLD);
}

void CRemoteBomb::getBonus(CPlayer* player) const
{
  player->setRemoteBomb();
}
