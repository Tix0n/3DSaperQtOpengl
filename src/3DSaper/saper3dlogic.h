#ifndef SRC_3DSAPER_SAPER3DLOGIC_H_
#define SRC_3DSAPER_SAPER3DLOGIC_H_

#include <QDebug>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "cell.h"

class GameLogic {
 public:
  GameLogic();
  void InitializeGame();
  void OpenField(int x, int y, int z);
  bool IsCellInMap(int x, int y, int z) const;
  void MineClearance(int x, int y, int z);
  void SetStartingState(int new_size);
  void SetReverseFlag(int x, int y, int z);

  int get_gridSize() const;
  int get_closedCell() const;
  int get_deactivetedMines() const;
  int get_countMines() const;
  const Cell& get_gameGrid(int x, int y, int z) const;

 private:
  int grid_size_;
  int closed_cell_;
  int deactiveted_mines_;
  int count_mines_;
  bool end_game_;
  static const int kMapSize = 10;
  Cell gameGrid[kMapSize][kMapSize][kMapSize];
};

#endif  // SRC_3DSAPER_SAPER3DLOGIC_H_
