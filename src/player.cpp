#include <ncurses.h>
#include <cmath>

#include "player.hpp"

CPlayer::CPlayer(unsigned short lives, bool first)
: lives(lives), first(first)
{
  totalBombs = TOTAL_BOMBS;
  strengthBomb = BASIC_BOMB_STRENGTH;
  for(int i = 0; i < TOTAL_BOMBS; i++)
    bombs.push_back(make_shared<CBomb>(strengthBomb));
}

void CPlayer::setPosition(unsigned short x, unsigned short y)
{
  posX = x;
  posY = y;
}

unsigned short CPlayer::getLives() const { return lives;      }
unsigned short CPlayer::getPosX()  const { return posX;       }
unsigned short CPlayer::getPosY()  const { return posY;       }
unsigned short CPlayer::getScore() const { return score;      }
vector<shared_ptr<CBomb>> CPlayer::getBombs() const { return bombs;      }
shared_ptr<CBomb> CPlayer::getRemoteBomb()    const { return remoteBomb; }

void CPlayer::incStrengthBomb()
{
  strengthBomb++;
  for(auto &it : bombs)
  {
    it->strength = strengthBomb;
  }
  if(remoteBombActive) remoteBomb->strength = strengthBomb;
}

void CPlayer::setPassBombs() { passBombs = true; }
void CPlayer::incLives()     { lives++;          }
void CPlayer::setRemoteBomb()
{
  if(!remoteBombActive)
  {
    remoteBomb = make_shared<CBomb>(strengthBomb);
    remoteBombActive = true;
  }
}

bool CPlayer::isRemoteActive() const { return remoteBombActive; }

void CPlayer::incNumBombs()
{
  totalBombs++;
  bombs.push_back(make_shared<CBomb>(strengthBomb));
}

void CPlayer::print(const int COLOR) const
{
  attron(COLOR_PAIR(COLOR));
  mvaddch(posY, posX, ' ');
  attroff(COLOR_PAIR(COLOR));
}

void CPlayer::placeBomb(BLOCK_MAP &map) const
{
  for(auto &it : bombs)
  {
    if(!it->activated)
    {
      it->activated = true;
      it->posX = posX;
      it->posY = posY;
      unsigned short id = map[posY][posX]->getId();
      map[posY][posX] = make_unique<CPlacedBomb>(posX, posY, "bomb", id);
      break;
    }
  }
}

void CPlayer::placeRemoteBomb(BLOCK_MAP &map)
{
  if(!remoteBombActive) return;
  if(placeRemote % 2)
  {
    remoteBomb->posX = posX;
    remoteBomb->posY = posY;
    remoteBomb->activated = true;
    unsigned short id = map[posY][posX]->getId();
    map[posY][posX] = make_unique<CPlacedBomb>(posX, posY, "bomb", id);
  }
  else remoteBomb->explosion = true;
  placeRemote++;
}

void CPlayer::updateRemote(BLOCK_MAP &map, BONUS_VECTOR &bonuses)
{
  if(!remoteBombActive) return;
  if (remoteBomb->activated && remoteBomb->explosion)
  {
    remoteBomb->explode(map, bonuses);
  }

  if(remoteBomb->activated && map[remoteBomb->posY][remoteBomb->posX]->function() == "fire")
  {
    remoteBomb->explode(map, bonuses);
    remoteBomb->explosion = true;
    placeRemote++;
  }
}

void CPlayer::updateBomb(BLOCK_MAP &map, BONUS_VECTOR &bonuses) const
{
  for(auto &it : bombs)
  {
    if(it->activated)
    {
      if(it->timer == BOMB_TIME || map[it->posY][it->posX]->function() == "fire")
      {
        it->timer = 0;
        it->explosion = true;
        it->explode(map, bonuses);
      }
      ++it->timer;
      if(it->explosion) it->explode(map, bonuses);
    }
  }
}

void CPlayer::checkEndMapBorders(BLOCK_MAP &map)
{
  if(map[posY][posX]->function() == "solid") lives = 0;
}

void CPlayer::checkBombCollision(BLOCK_MAP &map)
{
  if(map[posY][posX]->function() == "fire") lifeDetection++;
  else
  {
    lives -= ceil(lifeDetection/(BOMB_TIME+10.0));
    lifeDetection = 0;
  }
}

void CPlayer::setScore(const BLOCK_MAP &map, const shared_ptr<CPlayer> player)
{
  for(const auto &it : bombs) setScore(it, map, player);
  if(remoteBombActive) setScore(remoteBomb, map, player);
}

void CPlayer::setScore(const shared_ptr<CBomb> bomb, const BLOCK_MAP &map, const shared_ptr<CPlayer> player)
{
  if(bomb->explosion)
  {
    if(testCollision(map, player->getPosX(), player->getPosY(), bomb->posX, bomb->posY, bomb->strength)) score++;
  }
}

void CPlayer::clearBomb(BLOCK_MAP &map) const
{
  for(auto &it : bombs)
  {
    clearBomb(it, map);
  }

  if(remoteBombActive)
  {
    clearBomb(remoteBomb, map);
  }
}

void CPlayer::clearBomb(shared_ptr<CBomb> bomb, BLOCK_MAP &map) const
{
  if(bomb->explosion)
  {
    if(bomb->explosionTimer == EXPLOSION_TIME)
    {
      bomb->explosion = false;
      bomb->activated = false;
      bomb->explosionTimer = 0;
      bomb->clearExplosion(map);
    }
    ++bomb->explosionTimer;
  }
}

void CPlayer::checkBonusCollision(BONUS_VECTOR &bonuses)
{
  size_t tmp = 0;
  for(auto &it : bonuses)
  {
    if(posX == it->getPosX() && posY == it->getPosY())
    {
      it->getBonus(this);
      bonuses.erase(bonuses.begin()+tmp);
      break;
    }
    tmp++;
  }
}

bool CPlayer::testCollision(const BLOCK_MAP &map, unsigned short playerPosX, unsigned short playerPosY, unsigned short objectPosX, unsigned short objectPosY, unsigned short strength) const
{
  for(size_t n = 0; n < strength; ++n)
  {
    if(map[objectPosY-n][objectPosX]->function() == "solid") break;
    if(objectPosY-n == playerPosY && objectPosX == playerPosX) return true;
  }

  for(size_t s = 0; s < strength; ++s)
  {
    if(map[objectPosY+s][objectPosX]->function() == "solid") break;
    if(objectPosY+s == playerPosY && objectPosX == playerPosX) return true;
  }

  for(size_t w = 0; w < strength; ++w)
  {
    if(map[objectPosY][objectPosX-w]->function() == "solid") break;
    if(objectPosY == playerPosY && objectPosX-w == playerPosX) return true;
  }

  for(size_t e = 0; e < strength; ++e)
  {
    if(map[objectPosY][objectPosX+e]->function() == "solid") break;
    if(objectPosY == playerPosY && objectPosX+e == playerPosX) return true;
  }
  return false;
}

bool CPlayer::testMove(unsigned short x, unsigned short y, const BLOCK_MAP &map) const
{
  if(passBombs && map[y][x]->function() == "bomb") return true;
  else return map[y][x]->getAccess();
}
