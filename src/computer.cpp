#include <random>

#include "computer.hpp"

CComputer::CComputer(unsigned short lives, bool first)
: CPlayer(lives, first) {}

void CComputer::handleInput(int ch, BLOCK_MAP &map, BONUS_VECTOR &bonuses, const shared_ptr<CPlayer> human)
{
  static bool defense = false;
  if(!(cycle % 15))
  {
    attack(map, human);
    if(getBonus(map, bonuses));
    else if(destroy(map, defense));
    else move(map, human);
    destroy(map, defense);
    defense = defend(map, human);
  }

  if(lives == 1) getLife(map, bonuses);
  cycle++;
}

bool CComputer::defend(const BLOCK_MAP &map, const shared_ptr<CPlayer> player)
{
  bool dangerOther = false;
  bool dangerSelf = false;

  dangerOther = isDanger(posX, posY, map, player.get());
  dangerSelf  = isDanger(posX, posY, map, this);

  if(dangerOther || dangerSelf)
  {
    if (testMove(posX-1, posY, map)
    && (!isDanger(posX-1, posY, map, player.get()))
    && (!isDanger(posX-1, posY, map, this)))
      posX -= 1;

    else if(testMove(posX+1, posY, map)
    && (!isDanger(posX+1, posY, map, player.get()))
    && (!isDanger(posX+1, posY, map, this)))
      posX += 1;

    else if(testMove(posX, posY-1, map)
    && (!isDanger(posX, posY-1, map, player.get()))
    && (!isDanger(posX, posY-1, map, this)))
      posY -= 1;

    else if(testMove(posX, posY+1, map)
    && (!isDanger(posX, posY+1, map, player.get()))
    && (!isDanger(posX, posY+1, map, this)))
      posY += 1;

    else move(map, player);
    return true;
  }

  unsigned short risk = 20;
  if(totalBombs == 3) risk = 35;
  if(totalBombs > 3 ) risk = 40;
  if(!(cycle % risk) || totalBombs == 1) return false;
  else return true;
}

void CComputer::attack(BLOCK_MAP &map, const shared_ptr<CPlayer> human) const
{
  if(testCollision(map, human->getPosX(), human->getPosY(), posX, posY, strengthBomb))
    placeBomb(map);
}

void CComputer::getLife(const BLOCK_MAP &map, BONUS_VECTOR &bonuses) const
{
  static bool help = true;
  if(help)
  {
    if(testMove(posX, posY+1, map))
      bonuses.push_back(make_unique<CAddLive>(posX, posY+1));
    else if(testMove(posX, posY-1, map))
      bonuses.push_back(make_unique<CAddLive>(posX, posY-1));
    else if(testMove(posX+1, posY, map))
      bonuses.push_back(make_unique<CAddLive>(posX+1, posY));
    else if(testMove(posX-1, posY, map))
      bonuses.push_back(make_unique<CAddLive>(posX-1, posY));
    help = false;
  }
}

void CComputer::move(const BLOCK_MAP &map, const shared_ptr<CPlayer> human)
{
  if(testCollision(map, posX, posY, human->getPosX(), human->getPosY(), 5))
  {
    if(!(rand()%3)) goToTarget<shared_ptr<CPlayer>>(map, human);
    return;
  }

  enum Direction { NORTH, SOUTH, EAST, WEST, NONE};
  Direction rndDirection = static_cast<Direction> (rand() % 4);
  switch(rndDirection)
  {
    case EAST:
      if(testMove(posX-1, posY, map))
      {
        if(isDeadEnd(map, posX-1, posY)) break;
        posX -= 1;
      }
      break;

    case WEST:
      if(testMove(posX+1, posY, map))
      {
        if(isDeadEnd(map, posX+1, posY)) break;
        posX += 1;
      }
      break;

    case NORTH:
      if(testMove(posX, posY-1, map))
      {
        if(isDeadEnd(map, posX, posY-1)) break;
        posY -= 1;
      }
      break;

    case SOUTH:
      if(testMove(posX, posY+1, map))
      {
        if(isDeadEnd(map, posX, posY+1)) break;
        posY += 1;
      }
      break;
    case NONE:
      break;
  }
}

bool CComputer::destroy(BLOCK_MAP &map, bool defense) const
{
  if(defense) return false;
  if(map[posY][posX-1]->function() == "barrel") { placeBomb(map); return true; }
  if(map[posY][posX+1]->function() == "barrel") { placeBomb(map); return true; }
  if(map[posY-1][posX]->function() == "barrel") { placeBomb(map); return true; }
  if(map[posY+1][posX]->function() == "barrel") { placeBomb(map); return true; }
  return false;
}

bool CComputer::getBonus(const BLOCK_MAP &map, const BONUS_VECTOR &bonuses)
{
  for(const auto &it : bonuses)
  {
    if(testCollision(map, posX, posY, it->getPosX(), it->getPosY(), 5))
    {
      goToTarget<unique_ptr<CBonus>>(map, it);
      return true;
    }
  }
  return false;
}

bool CComputer::isDeadEnd(const BLOCK_MAP &map, unsigned short x, unsigned short y) const
{
  if (map[y][x+1]->function() == "empty" ||
      map[y][x-1]->function() == "empty" ||
      map[y+1][x]->function() == "empty" ||
      map[y-1][x]->function() == "empty"  )
      return false;
  else return true;
}

bool CComputer::isDanger(unsigned short x, unsigned short y, const BLOCK_MAP &map, const CPlayer* player) const
{
  for(const auto &it : player->getBombs())
  {
    if(it->activated) return testCollision(map, x, y, it->posX, it->posY, it->strength);
  }

  if(player->isRemoteActive())
  {
    auto rmtBomb = player->getRemoteBomb();
    if(rmtBomb->activated)
      return testCollision(map, x, y, rmtBomb->posX, rmtBomb->posY, rmtBomb->strength);
  }
  return false;
}

bool CComputer::isFirst() const { return first; }
