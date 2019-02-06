#include <ncurses.h>

#include "block.hpp"

CBlock::CBlock(unsigned short posX, unsigned short posY, const string &name, unsigned short id)
: posX(posX), posY(posY), id(id), name(name) {}

string CBlock::function() const { return name; }

unsigned short CBlock::getId()   const { return id;   }
unsigned short CBlock::getPosX() const { return posX; }
unsigned short CBlock::getPosY() const { return posY; }


CEmptyBlock::CEmptyBlock(unsigned short posX, unsigned short posY, const string &name, unsigned short id)
: CBlock(posX, posY, name, id) {}

void CEmptyBlock::print() const
{
	mvaddch(posY, posX, ' ');
}

bool CEmptyBlock::getAccess() const { return true; }


CBarrelBlock::CBarrelBlock(unsigned short posX, unsigned short posY, const string &name, unsigned short id)
: CBlock(posX, posY, name, id) {}

void CBarrelBlock::print() const
{
  attron(COLOR_PAIR(BARREL_COLOR));
	mvaddch(posY, posX, ' ');
	attroff(COLOR_PAIR(BARREL_COLOR));
}

bool CBarrelBlock::getAccess() const { return false; }


CSolidBlock::CSolidBlock(unsigned short posX, unsigned short posY, const string &name, unsigned short id)
: CBlock(posX, posY, name, id) {}

void CSolidBlock::print() const
{
  attron(COLOR_PAIR(SOLID_COLOR));
  mvaddch(posY, posX, ' ');
  attroff(COLOR_PAIR(SOLID_COLOR));
}

bool CSolidBlock::getAccess() const { return false; }


CPlacedBomb::CPlacedBomb(unsigned short posX, unsigned short posY, const string &name, unsigned short id)
: CBlock(posX, posY, name, id) {}

void CPlacedBomb::print() const
{
  attron(COLOR_PAIR(BOMB_COLOR));
  mvaddch(posY, posX, ' ');
  attroff(COLOR_PAIR(BOMB_COLOR));
}

bool CPlacedBomb::getAccess() const { return false; }


CExplosion::CExplosion(unsigned short posX, unsigned short posY, const string &name, unsigned short id)
: CPlacedBomb(posX, posY, name, id) {}

bool CExplosion::getAccess() const { return true; }
