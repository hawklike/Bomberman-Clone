#ifndef __BOMB_HPP__
#define __BOMB_HPP__

#include "block.hpp"
#include "bonus.hpp"

class CBonus;

/**
 * @brief Represents a bomb.
 *
 * This class represents a bomb placed by a player. After certain time,
 * the bomb explodes in four directions and may hurt a player. The explosion
 * destroys barrels.
 */
class CBomb
{
public:
  /**
   * @typedef matrix of unique pointers to CBlock
   */
  typedef vector<vector<unique_ptr<CBlock>>> BLOCK_MAP;

  /**
   * @typedef vector of unique pointers to CBonus
   */
  typedef vector<unique_ptr<CBonus>> BONUS_VECTOR;

  /**
   * The constructor has single argument, creating bomb of given strength.
   * @param strength represents size of an explosion
   */
  CBomb(unsigned short strength);

  /**
   * This method renders an explosion into four directions (north, south, east, west)
   * that destroys barrels in a path. Does not break through solid blocks. With some
   * probability generates a bonus.
   * @param map matrix of blocks that defines a corridor to an explosion
   * @param bonuses when a bonus is generated, then it is added to the vector of bonuses
   */
  void explode(BLOCK_MAP &map, BONUS_VECTOR &bonuses) const;

  /**
   * This method clears an explosion in a way that blocks after the explosion
   * turns into empty blocks.
   * @param map matrix of blocks that defines a corridor to an explosion
   */
  void clearExplosion(BLOCK_MAP &map) const;

  ///position of the bomb (axis x) on a map
  unsigned short posX;

  ///position of the bomb (axis y) on a map
  unsigned short posY;

  ///strength of the bomb
  unsigned short strength;

  ///indicates if the bomb is about to explode
  bool activated = false;

  ///time till the bomb explodes
  unsigned timer = 0;

  ///indicates if the bomb has exploded
  bool explosion = false;

  ///time till the explosion ends
  unsigned explosionTimer = 0;

private:
  /**
   * This method generates with a certain probability a bonus at the spot of a barrel.
   * Randomly chooses one of five bonuses.
   * @param bonuses vector of bonuses, where is new bonus added
   * @param posX position (axis x) where is new bonus loaded
   * @param posY position (axis y) where is new bonus loaded
   */
  void randomBonus(BONUS_VECTOR &bonuses, const unsigned short posX, const unsigned short posY) const;
};

#endif
