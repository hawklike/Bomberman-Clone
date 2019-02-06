#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "player.hpp"

/**
 * @brief Represents whole game itself.
 *
 * This class represents a basis of the whole program. It is mainly made up by two methods,
 * update and draw method. These two methods power the whole game. Futhermore the class
 * loads maps from files and saves a highscore into a file.
 */
class CGame
{
public:
  /**
   * The constructor has three arguments, giving the game two players and path to a map file.
   * Each player stores information, whether it is a human or a computer. Also loads data
   * from file, where is stored a map in specific format. From the data, blocks representing
   * tiles of a map are created in the createMap method.
   * @param player1 first player
   * @param player2 second player
   * @param mapAddress path to a map file
   */
  CGame(shared_ptr<CPlayer> player1, shared_ptr<CPlayer> player2, const string &mapAddress);

  /**
   * This method updates whole game.
   */
  void update();

  /**
   * The method draws a map, complementary game information and all graphic changes
   * which has happened in the update method.
   */
  void draw() const;

  /**
   * Checks if the game is still running.
   * @return returns TRUE if the game is running, else returns FALSE
   */
  bool isRunning() const;

private:
  /**
   * When game's time limit has passed, it starts collapsing a map. When a player
   * is detected outside decreasing map, he is killed and the game ends.
   */
  void collapseMap();

  /**
   * Prints all blocks that create a map together.
   */
  void printMap() const;

  /**
   * Prints a table of actual players lives.
   */
  void printLives() const;

  /**
   * Prints a table of bonus information.
   */
  void printTableBonuses() const;

  /**
   * Prints bonuses on a map.
   */
  void printBonuses() const;

  /**
   * Stores a highscore from a given file specified in constants.cpp.
   */
  void setHighscore();

  /**
   * Compares a highscore with the biggest score in a current game. If new score
   * is higher, then it is saved into a highscore file specified in constants.cpp.
   */
  void getHighscore() const;

  /**
   * From a data given in the vector content, blocks representing tiles of a map are created.
   * Then positions of both players are set.
   * @param content vector of chars representing blocks and players
   */
  void createMap(vector<char> &content);

  /**
   * This method tests if players are able to get to each other. The method uses algorithm BFS.
   * This method also tests if borders are corretly set. If not, an exception is thrown.
   * @return returns TRUE if a player1 is able to get to a player2, else returns FALSE
   */
  bool testmap() const;

  /**
   * Prints a highscore.
   */
  void printHighscore() const;

  /**
   * The method computes an exact score of a player based on his own score in
   * a current game and time, which has passed from the beginning of present game.
   * @param  timer number of cycles from the beginning of current game
   * @param  player pointer to player in order to use his score
   * @return returns a calculated score of a player
   */
  unsigned short getScore(const short timer, const shared_ptr<CPlayer> player) const;

  /**
   * Checks if the game is over. If so, puts down information about a winner.
   * @return returns TRUE if the game is over, else returns FALSE
   */
  bool isEnd() const;

  ///pointer to player1
  shared_ptr<CPlayer> player1;

  ///pointer to player2
  shared_ptr<CPlayer> player2;

  ///matrix of unique pointers to CBlock, represents game map
  vector<vector<unique_ptr<CBlock>>> map;

  ///vector of unique pointers to CBonus, represents bonuses
  vector<unique_ptr<CBonus>> bonuses;

  ///width of a map
  unsigned short width;

  ///height of a map
  unsigned short height;

  ///number of cycles from the start of a game
  unsigned short cycle;

  ///highest score ever achieved
  unsigned short highscore;

  ///bolean if a game is running
  bool running;
};

#endif
