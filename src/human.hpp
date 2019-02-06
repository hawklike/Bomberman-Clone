#ifndef __HUMAN_HPP__
#define __HUMAN_HPP__

#include "player.hpp"

/**
 * @brief Represents real human player.
 *
 * This class represents behaviour and movement of a real player. Player is able to
 * place bombs, move from one empty block to another and pick up bonuses. Movement
 * depends on pressed keys by the player.
 */
class CHuman : public CPlayer
{
public:
  /**
   * The constructor has two arguments, giving the player number of lives and
   * boolean identification if the player is first or not (i.e blue, or red).
   * @param lives number of lives the player starts with
   * @param first TRUE if the player is blue, else FALSE
   */
  CHuman(unsigned short lives, bool first);

  void handleInput(int ch, BLOCK_MAP &map, BONUS_VECTOR &bonuses, const shared_ptr<CPlayer> player);

private:
  /**
   * The method controls player's movement on axis x.
   * @param dx direction (axis x), where to the player is heading
   * @param map matrix of unique pointers to CBlock, represents game map
   */
  void controlX(short int dx, const BLOCK_MAP &map);

  /**
   * The method controls player's movement on axis y.
   * @param dx direction (axis y), where to the player is heading
   * @param map matrix of unique pointers to CBlock, represents game map
   */
  void controlY(short int dy, const BLOCK_MAP &map);

  bool isFirst() const;
};

#endif
