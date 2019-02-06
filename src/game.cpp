#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ncurses.h>
#include <algorithm>
#include <iterator>
#include <set>
#include <queue>

#include "game.hpp"

CGame::CGame(shared_ptr<CPlayer> player1, shared_ptr<CPlayer> player2, const string &mapAddress)
: player1(player1), player2(player2)
{
  ifstream file;
  file.open(mapAddress);

  if(file.fail()) throw invalid_argument("It is not possible to open the map. Maybe bad path given?");
  vector<char> content(istreambuf_iterator<char>(file), (istreambuf_iterator<char>()));
  file.close();

  if(!content.size()) throw invalid_argument("Empty map given.");

  createMap(content);
  setHighscore();
  srand(time(NULL));

  cycle = 0;
  running = true;
  height = map.size();
  width = map[0].size();
  if(!testmap()) throw invalid_argument("Players are not able to get to each other.");
}

void CGame::update()
{
  int ch = getch();
  if(ch == QUIT)          running = false;
  if(isEnd())             return;

  player1->handleInput(ch, map, bonuses, player2);
  player2->handleInput(ch, map, bonuses, player1);

  player1->updateBomb(map, bonuses);
  player2->updateBomb(map, bonuses);

  player1->updateRemote(map, bonuses);
  player2->updateRemote(map, bonuses);

  player1->clearBomb(map);
  player2->clearBomb(map);

  player1->checkBombCollision(map);
  player2->checkBombCollision(map);

  player1->checkBonusCollision(bonuses);
  player2->checkBonusCollision(bonuses);

  player1->setScore(map, player2);
  player2->setScore(map, player1);

  collapseMap();
  cycle++;
}

void CGame::draw() const
{
  printMap();
  player1->print(PLAYER1_COLOR);
  player2->print(PLAYER2_COLOR);
  printLives();
  printTableBonuses();
  printBonuses();
  printHighscore();
  refresh();
}

bool CGame::isRunning() const { return running; }

void CGame::printMap() const
{
  for(size_t i = 0; i < height; i++)
  {
    for(size_t j = 0; j < width; j++)
      map[i][j]->print();
  }
}

void CGame::createMap(vector<char> &content)
{
  vector<unique_ptr<CBlock>> row;
  unsigned short n_player1 = 0;
  unsigned short n_player2 = 0;
  unsigned short index = 0;
  unsigned short x = 0;
  unsigned short y = 0;

  for(const auto it : content)
  {
    switch(it)
    {
      case 'S':
        row.push_back(make_unique<CSolidBlock>(x++, y, "solid", index++));
        break;

      case ' ':
        row.push_back(make_unique<CEmptyBlock>(x++, y, "empty", index++));
        break;

      case 'B':
        row.push_back(make_unique<CBarrelBlock>(x++, y, "barrel", index++));
        break;

      case 'X':
        n_player1++;
        row.push_back(make_unique<CEmptyBlock>(x, y, "empty", index++));
        player1->setPosition(x++, y);
        break;

      case 'Y':
        n_player2++;
        row.push_back(make_unique<CEmptyBlock>(x, y, "empty", index++));
        player2->setPosition(x++, y);
        break;

      case '\n':
        map.push_back(move(row));
        row.clear();
        y++;
        x = 0;
        break;

      default:
        stringstream error;
        error << "Bad character in map at position [" << y+1 << ", " << x+1 << "].";
        throw invalid_argument(error.str());
        break;
    }
  }
  if(n_player1 != 1 || n_player2 != 1) throw invalid_argument("Bad number of players. Place exactly two different players.");
}

bool CGame::testmap() const
{
  set<int> visited;
  queue<int> q;

  q.push(map[player1->getPosY()][player1->getPosX()]->getId());
  visited.insert(map[player1->getPosY()][player1->getPosX()]->getId());

  while(!q.empty())
  {
    bool up, down, left, right;
    up = down = left = right = false;
    const unsigned short node = q.front();
    unsigned short y = node/width;
    unsigned short x = node-y*width;
    if(map[y][x]->getPosX() == player2->getPosX()
    && map[y][x]->getPosY() == player2->getPosY()) return true;
    q.pop();

    //checks if the node is not out of map
    if(y == 0) up = true;
    if(y == height - 1) down = true;
    if(x == 0) left = true;
    if(x == width - 1) right = true;
    if((up || down || left || right) && map[y][x]->function() != "solid")
      throw invalid_argument("There are not proper borders in your map.");

    if(!up && map[y-1][x]->function() != "solid"
    && !visited.count(map[y-1][x]->getId()))
    {
      q.push(map[y-1][x]->getId());
      visited.insert(map[y-1][x]->getId());
    }

    if(!down && map[y+1][x]->function() != "solid"
    && !visited.count(map[y+1][x]->getId()))
    {
      q.push(map[y+1][x]->getId());
      visited.insert(map[y+1][x]->getId());
    }

    if(!left && map[y][x-1]->function() != "solid"
    && !visited.count(map[y][x-1]->getId()))
    {
      q.push(map[y][x-1]->getId());
      visited.insert(map[y][x-1]->getId());
    }

    if(!right && map[y][x+1]->function() != "solid"
    && !visited.count(map[y][x+1]->getId()))
    {
      q.push(map[y][x+1]->getId());
      visited.insert(map[node/width][x+1]->getId());
    }
  }
  return false;
}

void CGame::collapseMap()
{
  enum Direction { NORTH, SOUTH, EAST, WEST };
  static short x = 0;
  static short y = 0;
  static short row = 0;
  static short column = 0;
  static Direction direct = EAST;

  if(cycle/100 > TIME_LIMIT && !(cycle % 20))
  {
    unsigned short id = map[y][x]->getId();
    map[y][x] = make_unique<CPlacedBomb>(x, y, "solid", id);
    player1->checkEndMapBorders(map);
    player2->checkEndMapBorders(map);

    switch(direct)
    {
      case EAST:
        x++;
        if(x == width - column)
        {
          direct = SOUTH;
          column++;
          x = width - column;
        }
        break;

      case SOUTH:
        y++;
        if(y == height - row)
        {
          direct = WEST;
          row++;
          y = height - row;
        }
        break;

      case WEST:
        x--;
        if(x < column - 1)
        {
          direct = NORTH;
          x = column - 1;
        }
        break;

      case NORTH:
        y--;
        if(y == row)
        {
          direct = EAST;
        }
        break;
    }
  }
}

void CGame::printLives() const
{
  attron(A_BOLD);
  mvprintw(1, width+8, "LIVES");
  attroff(A_BOLD);
  mvprintw(2, width+21, " ");
  mvprintw(2, width+22, " ");
  mvprintw(3, width+21, " ");
  mvprintw(3, width+22, " ");
  mvprintw(2, width+8, "Blue player: ");
  mvprintw(2, width+21, "%d", player1->getLives());
  mvprintw(3, width+8, "Red player: ");
  mvprintw(3, width+21, "%d", player2->getLives());
}

void CGame::printBonuses() const
{
  for(auto &it : bonuses) it->print();
}

void CGame::printTableBonuses() const
{
  attron(A_BOLD);
  mvprintw(5, width+8, "BONUSES");
  attroff(A_BOLD);
  mvprintw(6, width+8, "E....extends explosion");
  mvprintw(7, width+8, "P....you can walk through planted bomb");
  mvprintw(8, width+8, "L....adds life");
  mvprintw(9, width+8, "B....adds new bomb to your stack");
  mvprintw(10, width+8, "R....now you have remote bomb");
}

void CGame::setHighscore()
{
  ifstream file;
  file.open(HIGHSCORE_FILE);
  if(file.fail()) throw invalid_argument("It is not possible to open the highscore table. \nMaybe it was deleted in the \'examples\' folder?");
  file>>highscore;
  file.close();
}

void CGame::getHighscore() const
{
  short timer = TIME_LIMIT - cycle/100;
  unsigned short biggerScore = max(getScore(timer, player1), getScore(timer, player2));
  if(biggerScore > highscore)
  {
    ofstream file;
    file.open(HIGHSCORE_FILE);
    file<<biggerScore;
    file.close();
  }
}

void CGame::printHighscore() const
{
  attron(A_BOLD);
  mvprintw(height+1, 5, "HIGHSCORE:");
  mvprintw(height+2, 5, "TIME:");
  mvprintw(1, width+30, "SCORE");
  attroff(A_BOLD);
  mvprintw(height+1, 16, "%d", highscore);

  short timer = TIME_LIMIT - cycle/100;
  if(timer <= 0) timer = 0;

  mvdelch(2, width+43);
  mvprintw(2, width+30, "Blue player: ");
  mvprintw(2, width+43, "%d", getScore(timer, player1));

  mvdelch(3, width+43);
  mvprintw(3, width+30, "Red player: ");
  mvprintw(3, width+43, "%d", getScore(timer, player2));

  mvdelch(height+2, 11);
  mvprintw(height+2, 11, "%d", timer);
}

unsigned short CGame::getScore(const short timer, const shared_ptr<CPlayer> player) const
{
  short score = 2 * timer + (100 / EXPLOSION_TIME * player->getScore() + 99) / 100 * 200;
  if(score <= 0) score = 0;
  return score;
}

bool CGame::isEnd() const
{
  bool end = false;
  if(!player1->getLives())
  {
    mvprintw(12, width+8, "Red player is the winner!");
    end = true;

  }
  if(!player2->getLives())
  {
    mvprintw(12, width+8, "Blue player is the winner!");
    end = true;
  }
  if(!(player1->getLives() || player2->getLives()))
  {
    mvprintw(12, width+8, "No one is the winner. It's a draw!");
    end = true;
  }
  if(end)
  {
    mvprintw(13, width+8, "Press \'q\' to exit.");
    getHighscore();
    nodelay(stdscr, false);
    return true;
  }
  return end;
}
