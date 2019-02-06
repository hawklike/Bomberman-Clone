#ifndef __COMPUTER_HPP__
#define __COMPUTER_HPP__

#include "player.hpp"

/**
 * @brief Represents computer playing the game.
 *
 * This class allows to play the game against non-human opponent.
 * The non-player character is able to: \n
 * * defend itself against an explosion of a placed bomb \n
 * * attack a player if he is nearby \n
 * * place a bomb if there is a barrel \n
 * * go towards a bonus if the bonus is seen in one of four directions (north, south, west, east) \n
 * * go towards a player if the player is seen in one of four directions (north, south, west, east) \n
 * * place more bombs, if it has more bombs in its stack
 */
class CComputer : public CPlayer
{
public:
  /**
   * The constructor has two arguments, giving the player number of lives and
   * boolean identification if the player is first or not (i.e is blue, or red).
   * @param lives number of lives the player starts with
   * @param first TRUE if the player is blue, else FALSE
   */
  CComputer(unsigned short lives, bool first);

  void handleInput(int ch, BLOCK_MAP &map, BONUS_VECTOR &bonuses, const shared_ptr<CPlayer> player);

private:
  ///number of current cycle, works for time management
  unsigned short cycle = 0;

  /**
   * Checks if the NPC is in a range of any explosion, if so,
   * it tries to move to a safe position.
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @param player pointer to CPlayer in order to find out player's position
   * @return returns TRUE if the NPC is in range of any explosion, else returns FALSE
   */
  bool defend(const BLOCK_MAP &map, const shared_ptr<CPlayer> player);

  /**
   * If an opponent is in a NPC's bomb range, the NPC will place a bomb.
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @param human pointer to CPlayer in order to find out opponent's position
   */
  void attack(BLOCK_MAP &map, const shared_ptr<CPlayer> human) const;

  /**
   * If there is an opponent visible by the NPC (only in certain distance), the NPC will
   * chase the opponent. Otherwise the NPC moves to a randomly selected block, which
   * is empty and next to a NPC's current position.
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @param human pointer to CPlayer in order to find out opponent's position
   */
  void move(const BLOCK_MAP &map, const shared_ptr<CPlayer> human);

  /**
   * If the NPC has last life, this method will generate bonus CAddLive next to a NPC's
   * current position. The bonus will be generated only once for whole game.
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @param bonuses vector of unique pointers to CBonus, represents bonuses
   */
  void getLife(const BLOCK_MAP &map, BONUS_VECTOR &bonuses) const;

  /**
   * If there is a barrel next to a NPC's position, the NPC will place a bomb
   * in order to destroy the barrel. Only places the bomb if it does not go towards
   * bonus or it is not in a defense mode.
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @param defense boolean argument, if the NPC's defended itself at the last moment
   * @return returns TRUE if the NPC is about to place a bomb, else returns FALSE
   */
  bool destroy(BLOCK_MAP &map, bool defense) const;

  /**
   * If there is a bonus in straight line of any of four directions (N, S, E, W),
   * the NPC will go that direction so that it could get the bonus.
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @param bonuses vector of unique pointers to CBonus, represents bonuses
   * @return returns TRUE if the NPC is trying to get a bonus, else returns FALSE
   */
  bool getBonus(const BLOCK_MAP &map, const BONUS_VECTOR &bonuses);

  /**
   * This method checks if a block at given coordinates has any empty blocks around it.
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @param x position of a block (axis x) on the map
   * @param y position of a block (axis y) on the map
   * @return returns TRUE if a block is dead end, else returns FALSE
   */
  bool isDeadEnd(const BLOCK_MAP &map, unsigned short x, unsigned short y) const;

  /**
   * This method indicates whether the NPC stays at a block, which is located in any bomb range.
   * @param x position of a player (axis x) on the map
   * @param y position of a player (axis y) on the map
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @param player a pointer to CPlayer, used for gaining player's position
   * @return returns TRUE if a player is located in any bomb range, else returns FALSE
   */
  bool isDanger(unsigned short x, unsigned short y, const BLOCK_MAP &map, const CPlayer* player) const;

  /**
   * This generic function allows a player to move towards a target.
   * @param map matrix of unique pointers to CBlock, represents a game map
   * @tparam targetPtr a pointer to a target, where to player moves
   */
  template<typename T>
  void goToTarget(const BLOCK_MAP &map, const T &targetPtr)
  {
    if(posX == targetPtr->getPosX())
    {
      if(posY > targetPtr->getPosY())
      {
        if(testMove(posX, posY-1, map)) posY -= 1;
      }
      else if(testMove(posX, posY+1, map)) posY += 1;
    }
    else
    {
      if(posX > targetPtr->getPosX())
      {
        if(testMove(posX-1, posY, map)) posX -= 1;
      }
      else if(testMove(posX+1, posY, map)) posX += 1;
    }
  }

  bool isFirst() const;
};

#endif
