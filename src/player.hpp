#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <vector>
#include <memory>

#include "bomb.hpp"

class CBomb;
class CBonus;

/**
 * @brief Represents an environment for players in the game.
 *
 * This abstract class makes an environment for its derived clases,
 * which represent both possible game modes, a singleplayer and a multiplayer.
 * When a singleplayer is selected, human player plays the game against a NPC implemented
 * in the class CComputer, otherwise both human players play the game
 * against each other using the class CHuman.
 */
class CPlayer
{
public:
  /**
   * @typedef a matrix of unique pointers to CBlock
   */
  typedef vector<vector<unique_ptr<CBlock>>> BLOCK_MAP;

  /**
   * @typedef a vector of unique pointers to CBonus
   */
  typedef vector<unique_ptr<CBonus>> BONUS_VECTOR;

  /**
   * The constructor has two arguments, giving the player number of lives and
   * boolean identification if the player is first or not (i.e blue, or red).
   * @param lives number of lives the player starts with
   * @param first TRUE if the player is blue, else FALSE
   */
  CPlayer(unsigned short lives, bool first);

  /**
   * This method sets position of the player on a map.
   * @param x position of the player (axis x) on a map
   * @param y position of the player (axis y) on a map
   */
  void setPosition(unsigned short x, unsigned short y);

  /**
   * Each player has lives. This method returns current number of player's lives.
   * @return returns current number of player's lives
   */
  unsigned short getLives() const;

  /**
   * Each player stores position on a map.
   * @return returns position of the player (axis x) on a map
   */
  unsigned short getPosX()  const;

  /**
   * Each player stores position on a map.
   * @return returns position of the player (axis y) on a map
   */
  unsigned short getPosY()  const;

  /**
   * Each player stores current score in a game.
   * @return returns present score of the player
   */
  unsigned short getScore() const;

  /**
   * Each player stores his bombs in a vector.
   * @return returns a vector of pointers to CBomb, representing player's bombs
   */
  vector<shared_ptr<CBomb>> getBombs() const;

  /**
   * Each player may store a remote-controlled bomb, depends on whether he
   * has picked up the bonus CRemoteBomb in a game.
   * @return returns a pointer to CBomb, representing player's remote-controlled bomb
   */
  shared_ptr<CBomb> getRemoteBomb()    const;

  /**
   * Increases strength of all player's bombs, including a remote-controlled bomb.
   */
  void incStrengthBomb();

  /**
   * Allows the player walk through placed bombs.
   */
  void setPassBombs();

  /**
   * Adds new life to the player.
   */
  void incLives();

  /**
   * Adds a new bomb to a player's stack.
   */
  void incNumBombs();

  /**
   * Allows the player place a remote-controlled bomb.
   */
  void setRemoteBomb();

  /**
   * Indicates whether the player is able to use a remote-controlled bomb.
   * @return returns TRUE if the player has got the bonus CRemoteBomb, else returns FALSE
   */
  bool isRemoteActive() const;

  /**
   * The print method renders the graphics of each player.
   * @param COLOR color of the player
   */
  void print(const int COLOR) const;

  /**
   * Tests, if the player is killed by decreasing map after timer is set to zero.
   * @param map matrix of unique pointers to CBlock, represents a game map
   */
  void checkEndMapBorders(BLOCK_MAP &map);

  /**
   * This method lets a remote-controlled bomb explode. If not yet exploded bomb
   * is hit by another bomb's explosion, the bomb will explode as well.
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @param bonuses vector of unique pointers to CBonus, represents bonuses
   */
  void updateRemote(BLOCK_MAP &map, BONUS_VECTOR &bonuses);

  /**
   * This method lets a bomb, whose timer reached a time of an explosion, to explode.
   * If not yet exploded bomb is hit by another bomb's explosion,
   * the bomb will explode as well.
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @param bonuses vector of unique pointers to CBonus, represents bonuses
   */
  void updateBomb(BLOCK_MAP &map, BONUS_VECTOR &bonuses) const;

  /**
   * If the player is hit by a bomb explosion, this method will dicrease player's lives.
   * @param map matrix of unique pointers to CBlock, represents a game map
   */
  void checkBombCollision(BLOCK_MAP &map);

  /**
   * If any player's bomb hits an opponent, the method will increase a player's
   * score by certain value. Unlike the second function of the same name,
   * this one is more general.
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @param player shared pointer to an opponent
   */
  void setScore(const BLOCK_MAP &map, const shared_ptr<CPlayer> player);

  /**
   * The method clears a bomb explosion after a certain time is passed. Unlike the second
   * function of the same name, this one is more general.
   * @param map matrix of unique pointers to CBlock, represents a game map
   */
  void clearBomb(BLOCK_MAP &map) const;

  /**
   * Checks whether the player go through a bonus, if so, the bonus will effect the player.
   * @param bonuses vector of unique pointers to CBonus, represents bonuses
   */
  void checkBonusCollision(BONUS_VECTOR &bonuses);

  /**
   * Indicates whether the player is first, i.e blue or red.
   * @return returns TRUE if the player is blue, else returns FALSE
   */
  virtual bool isFirst() const = 0;

  /**
   * This method processes input of the player and handles consequences.
   * @param ch code of a pressed key
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @param bonuses vector of unique pointers to CBonus, represents bonuses
   * @param player shared pointer to CPlayer, represents an opponent
   */
  virtual void handleInput(int ch, BLOCK_MAP &map, BONUS_VECTOR &bonuses, const shared_ptr<CPlayer> player) = 0;

protected:
  /**
   * Checks if the player has access on a given block.
   * @param  x position of a given block (axis x) on a map
   * @param  y position of a given block (axis y) on a map
   * @param  map matrix of unique pointers to CBlock, represents a game map
   * @return returns TRUE if the player is allowed to move on given block, else returns FALSE
   */
  bool testMove(unsigned short x, unsigned short y, const BLOCK_MAP &map) const;

  /**
   * This method tests if the player has direct or indirect collision with a object located
   * at given coordinates. In practise the method monitors whether the player is able to
   * see the object in straight line to distance given in a parameter strength.
   * @param  map matrix of unique pointers to CBlock, represents a game map
   * @param  playerPosX position of the player (axis x) on a map
   * @param  playerPosY position of the player (axis y) on a map
   * @param  objectPosX position of an object (axis x) on a map
   * @param  objectPosY position of an object (axis y) on a map
   * @param  strength a distance to which the method checks a collision
   * @return returns TRUE if the player sees an object, else returns FALSE
   */
  bool testCollision(const BLOCK_MAP &map, unsigned short playerPosX, unsigned short playerPosY, unsigned short objectPosX, unsigned short objectPosY, unsigned short strength) const;

  /**
   * If the player is able to place remote-controlled bombs, this method places a bomb at
   * player's position after a certain key is pressed. If the key is pressed for
   * the second time, the remote bomb will explode.
   * @param map matrix of unique pointers to CBlock, represents a game map
   */
  void placeRemoteBomb(BLOCK_MAP &map);

  /**
   * This method places only an inactive bomb at a player's current position and
   * makes the bomb activated.
   * @param map matrix of unique pointers to CBlock, represents a game map
   */
  void placeBomb(BLOCK_MAP &map) const;

  /**
   * Checks whether the player go through a bonus, if so, the bonus will effect the player.
   * @param bomb pointer to a bomb which is about to be cleared
   * @param bonuses vector of unique pointers to CBonus, represents bonuses
   */
  void clearBomb(shared_ptr<CBomb> bomb, BLOCK_MAP &map) const;

  /**
   * If any player's bomb hits an opponent, the method will increase a player's
   * score by a certain value.
   * @param bomb pointer to CBomb, indicates an explosion
   * @param map atrix of unique pointers to CBlock, represents a game map
   * @param player shared pointer to an opponent in order to find out his position on a map
   */
  void setScore(const shared_ptr<CBomb> bomb, const BLOCK_MAP &map, const shared_ptr<CPlayer> player);

  ///player's position (axis x) on a map
  unsigned short posX;

  ///player's position (axis y) on a map
  unsigned short posY;

  ///number, which indicates a size of an explosion of all player's bombs
  unsigned short strengthBomb;

  ///number of player's lives
  unsigned short lives;

  ///total number of normal player's bombs
  unsigned short totalBombs;

  ///extra variable to count lives while explosion hits the player
  unsigned short lifeDetection = 0;

  ///extra variable to indicate how many times has been a "placeRemoteBomb" key pressed
  unsigned short placeRemote = 1;

  ///player's score
  unsigned short score = 0;

  ///boolean flag, which indicates if the player is able to walk through placed bombs
  bool passBombs = false;

  ///boolean flag, which indicates if the player is able to place a remote-controlled bombs
  bool remoteBombActive = false;

  ///boolean if the player is red or blue
  bool first;

  ///player's repository of bombs
  vector<shared_ptr<CBomb>> bombs;

  ///remote-controlled bomb
  shared_ptr<CBomb> remoteBomb;
};

#endif
