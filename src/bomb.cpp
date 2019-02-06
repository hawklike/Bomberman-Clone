#include <string>
#include <random>

#include "bomb.hpp"

CBomb::CBomb(unsigned short strength)
: strength(strength) {}

void CBomb::explode(BLOCK_MAP &map, BONUS_VECTOR &bonuses) const
{
  //explode NORTH
  for(size_t n = 0; n < strength; ++n)
  {
    if(map[posY-n][posX]->function() == "solid") break;
    if(map[posY-n][posX]->function() == "barrel")
    {
      if(!(rand() % BONUS_CHANCE)) randomBonus(bonuses, posX, posY-n);
    }
    unsigned short id = map[posY-n][posX]->getId();
    map[posY-n][posX] = make_unique<CExplosion>(posX, posY-n, "fire", id);
  }

  //explode SOUTH
  for(size_t s = 0; s < strength; ++s)
  {
    if(map[posY+s][posX]->function() == "solid") break;
    if(map[posY+s][posX]->function() == "barrel")
    {
      if(!(rand() % BONUS_CHANCE)) randomBonus(bonuses, posX, posY+s);
    }
    unsigned short id = map[posY+s][posX]->getId();
    map[posY+s][posX] = make_unique<CExplosion>(posX, posY+s, "fire", id);
  }

  //explode WEST
  for(size_t w = 0; w < strength; ++w)
  {
    if(map[posY][posX-w]->function() == "solid") break;
    if(map[posY][posX-w]->function() == "barrel")
    {
      if(!(rand() % BONUS_CHANCE)) randomBonus(bonuses, posX-w, posY);
    }
    unsigned short id = map[posY][posX-w]->getId();
    map[posY][posX-w] = make_unique<CExplosion>(posX-w, posY, "fire", id);
  }

  //explode EAST
  for(size_t e = 0; e < strength; ++e)
  {
    if(map[posY][posX+e]->function() == "solid") break;
    if(map[posY][posX+e]->function() == "barrel")
    {
      if(!(rand() % BONUS_CHANCE)) randomBonus(bonuses, posX+e, posY);
    }
    unsigned short id = map[posY][posX+e]->getId();
    map[posY][posX+e] = make_unique<CExplosion>(posX+e, posY, "fire", id);
  }
}

void CBomb::clearExplosion(BLOCK_MAP &map) const
{
  //explode NORTH
  for(size_t n = 0; n < strength; ++n)
  {
    if(map[posY-n][posX]->function() == "solid") break;
    unsigned short id = map[posY-n][posX]->getId();
    map[posY-n][posX] = make_unique<CEmptyBlock>(posX, posY-n, "empty", id);
  }

  //explode SOUTH
  for(size_t s = 0; s < strength; ++s)
  {
    if(map[posY+s][posX]->function() == "solid") break;
    unsigned short id = map[posY+s][posX]->getId();
    map[posY+s][posX] = make_unique<CEmptyBlock>(posX, posY+s, "empty", id);
  }

  //explode WEST
  for(size_t w = 0; w < strength; ++w)
  {
    if(map[posY][posX-w]->function() == "solid") break;
    unsigned short id = map[posY][posX-w]->getId();
    map[posY][posX-w] = make_unique<CEmptyBlock>(posX-w, posY, "empty", id);
  }

  //explode EAST
  for(size_t e = 0; e < strength; ++e)
  {
    if(map[posY][posX+e]->function() == "solid") break;
    unsigned short id = map[posY][posX+e]->getId();
    map[posY][posX+e] = make_unique<CEmptyBlock>(posX+e, posY, "empty", id);
  }
}

void CBomb::randomBonus(BONUS_VECTOR &bonuses, const unsigned short posX, const unsigned short posY) const
{
  enum Bonuses { EXPLOSION, WALKTHROUGH, LIVE, BOMB, REMOTE };
  Bonuses rndBonus = static_cast<Bonuses> (rand() % 5);
  switch(rndBonus)
  {
    case EXPLOSION:
      bonuses.push_back(make_unique<CBiggerExplosion>(posX, posY));
      break;
    case WALKTHROUGH:
      bonuses.push_back(make_unique<CBombPassage>(posX, posY));
      break;
    case LIVE:
      bonuses.push_back(make_unique<CAddLive>(posX, posY));
      break;
    case BOMB:
      bonuses.push_back(make_unique<CAddBomb>(posX, posY));
      break;
    case REMOTE:
      bonuses.push_back(make_unique<CRemoteBomb>(posX, posY));
      break;
  }
}
