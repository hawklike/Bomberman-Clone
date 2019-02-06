#ifndef __BLOCK_HPP__
#define __BLOCK_HPP__

#include <string>

#include "constants.hpp"
using namespace std;

/**
 * @brief Represents an environment for blocks.
 *
 * This abstract class makes an environment for its derived classes,
 * which represent each block on a game map.
 */

class CBlock
{
public:
  /**
   * The constructor has four arguments, giving each block
   * coordinates of its position, name of its function and id.
   * @param posX position on axis x
   * @param posY position on axis y
   * @param name function of the the block
   * @param id identification number
   */
  CBlock(unsigned short posX, unsigned short posY, const string &name, unsigned short id);

  /**
   * Each block has its own function.
   * @return returns function of the block
   */
  string function() const;

  /**
   * Each block has its own identification number.
   * @return returns id of the block
   */
  unsigned short getId() const;

  /**
   * Each block has its own position on a map stored.
   * @return returns position (on axis x) of the block on a map
   */
  unsigned short getPosX() const;

  /**
   * Each block has its own position on a map stored.
   * @return returns position (on axis y) of the block on a map
   */
  unsigned short getPosY() const;

  /**
   * The print method renders the graphics of each block.
   */
  virtual void print() const = 0;

  /**
   * Each block holds information if it can be visited by a player.
   * @return returns TRUE if a player has access to the block, else returns FALSE
   */
  virtual bool getAccess() const = 0;
protected:
  ///position of the block (axis x) on a map
  unsigned short posX;

  ///position of the block (axis y) on a map
  unsigned short posY;

  ///identification number
  unsigned short id;

  ///function of the block
  string name;
};

/**
 * @brief Represents a block of an empty space.
 *
 * This block represents an empty space, which is made avaliable
 * to a player. Player is allowed to place a bomb at the block,
 * stand at the block or pick up a bonus at the block.
 */
class CEmptyBlock : public CBlock
{
public:
  /**
   * The constructor has four arguments, giving each block
   * coordinates of its position, name of its function and id.
   * @param posX position on axis x
   * @param posY position on axis y
   * @param name function of the block
   * @param id identification number
   */
  CEmptyBlock(unsigned short posX, unsigned short posY, const string &name, unsigned short id);
private:
  void print() const;

  bool getAccess() const;
};

/**
 * @brief Represents a block of a barrel.
 *
 * This block represents a barrel, which can be detonated by a bomb. Player
 * is not allowed to stand at the block.
 */
class CBarrelBlock : public CBlock
{
public:
  /**
   * The constructor has four arguments, giving each block
   * coordinates of its position, name of its function and id.
   * @param posX position on axis x
   * @param posY position on axis y
   * @param name function of the block
   * @param id identification number
   */
  CBarrelBlock(unsigned short posX, unsigned short posY, const string &name, unsigned short id);
private:
  void print() const;

  bool getAccess() const;
};

/**
 * @brief Represents a block of a solid object.
 *
 * This block represents solid block, as a rule it forms boundaries.
 * Cannot be detonated by a bomb and access is not allowed to a player.
 */
class CSolidBlock : public CBlock
{
public:
  /**
   * The constructor has four arguments, giving each block
   * coordinates of its position, name of its function and id.
   * @param posX position on axis x
   * @param posY position on axis y
   * @param name function of the block
   * @param id identification number
   */
  CSolidBlock(unsigned short posX, unsigned short posY, const string &name, unsigned short id);
private:
  void print() const;

  bool getAccess() const;
};

/**
 * @brief Represents a block of a placed bomb.
 *
 * This block represents placed bomb, which is about to explode.
 * If a player has a 'bomb passage' bonus, he can stand at the block.
 */
class CPlacedBomb : public CBlock
{
public:
  /**
   * The constructor has four arguments, giving each block
   * coordinates of its position, name of its function and id.
   * @param posX position on axis x
   * @param posY position on axis y
   * @param name function of the block
   * @param id identification number
   */
  CPlacedBomb(unsigned short posX, unsigned short posY, const string &name, unsigned short id);
protected:
  void print() const;

  bool getAccess() const;
};

/**
 * @brief Represents a block of an explosion.
 *
 * This block represents an explosion made by a bomb. It looks the same
 * as a placed bomb, but a player has access to this block anytime.
 */
class CExplosion : public CPlacedBomb
{
public:
  /**
   * The constructor has four arguments, giving each block
   * coordinates of its position, name of its function and id.
   * @param posX position on axis x
   * @param posY position on axis y
   * @param name function of the block
   * @param id identification number
   */
  CExplosion(unsigned short posX, unsigned short posY, const string &name, unsigned short id);
private:
  bool getAccess() const;
};

#endif
