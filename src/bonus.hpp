#ifndef __BONUS_HPP__
#define __BONUS_HPP__

#include "player.hpp"

class CPlayer;

/**
 * @brief Represents an environment for bonuses.
 *
 * This abstract class makes an environment for its derived clases,
 * which represent all bonuses possible to get in the game.
 */
class CBonus
{
public:
  /**
   * The constructor has two arguments, giving each bonus
   * coordinates of its position.
   * @param posX position of the bonus on axis x
   * @param posY position of the bonus on axis y
   */
  CBonus(unsigned short posX, unsigned short posY);

  /**
   * The print method renders the graphics of each bonus.
   */
  virtual void print() const = 0;

  /**
   * The method gives a bonus to a player, who takes the bonus first.
   * @param player a player who picked up the bonus first
   */
  virtual void getBonus(CPlayer* player) const = 0;

  /**
   * Each bonus preserves its position on a map.
   * @return returns position (axis x) of the bonus on a map
   */
  unsigned short getPosX() const;

  /**
   * Each bonus preserves its position on a map.
   * @return returns position (axis y) of the bonus on a map
   */
  unsigned short getPosY() const;
protected:
  ///position of (axis x) the bonus on a map
  unsigned short posX;

  ///position of (axis y) the bonus on a map
  unsigned short posY;
};

/**
 * @brief Represents a bonus, which increases a bomb explosion.
 *
 * This bonus increases a bomb explosion for that player who collects the bonus.
 * The more of these bonuses player collects, the bigger explosion his bombs have.
 */
class CBiggerExplosion : public CBonus
{
public:
  /**
   * The constructor has two arguments, giving each bonus
   * coordinates of its position.
   * @param posX position of the bonus on axis x
   * @param posY position of the bonus on axis y
   */
  CBiggerExplosion(unsigned short posX, unsigned short posY);

  void print() const override;

  void getBonus(CPlayer* player) const;
};

/**
 * @brief Represents a bonus, which allows player to walk through placed bombs.
 *
 * This bonus allows player, who picks up the bonus to go through placed bombs.
 * Once a player takes the bonus, it is valid till the end of game.
 */
class CBombPassage : public CBonus
{
public:
  /**
   * The constructor has two arguments, giving each bonus
   * coordinates of its position.
   * @param posX position of the bonus on axis x
   * @param posY position of the bonus on axis y
   */
  CBombPassage(unsigned short posX, unsigned short posY);

  void print() const override;

  void getBonus(CPlayer* player) const;
};

/**
 * @brief Represents a bonus, which adds live to a player.
 *
 * This bonus adds live to a player, who collects the bonus. Player
 * can get more lives if he picks up more of these bonuses.
 */
class CAddLive : public CBonus
{
public:
  /**
   * The constructor has two arguments, giving each bonus
   * coordinates of its position.
   * @param posX position of the bonus on axis x
   * @param posY position of the bonus on axis y
   */
  CAddLive(unsigned short posX, unsigned short posY);

  void print() const override;

  void getBonus(CPlayer* player) const;
};

/**
 * @brief Represents a bonus, which adds new bomb to a player's stack.
 *
 * This bonus adds new bomb with actual player's strength to a player's stack.
 * The more of these bonuses player collects, the more bombs he has.
 */
class CAddBomb : public CBonus
{
public:
  /**
   * The constructor has two arguments, giving each bonus
   * coordinates of its position.
   * @param posX position of the bonus on axis x
   * @param posY position of the bonus on axis y
   */
  CAddBomb(unsigned short posX, unsigned short posY);

  void print() const override;

  void getBonus(CPlayer* player) const;
};

/**
 * @brief Represents a bonus, which allows player to remote control one of his bombs.
 *
 * This bonus allows a player to get completely new bomb, which he can let explode
 * remotely. The player places a remote bomb and the bomb is there until he lets
 * it explode. The player can get only one remote bomb for each game.
 */
class CRemoteBomb : public CBonus
{
public:
  /**
   * The constructor has two arguments, giving each bonus
   * coordinates of its position.
   * @param posX position of the bonus on axis x
   * @param posY position of the bonus on axis y
   */
  CRemoteBomb(unsigned short posX, unsigned short posY);

  void print() const override;

  void getBonus(CPlayer* player) const;
};

#endif
