#ifndef CELL_H
#define CELL_H

struct Cell {
  bool has_mine;
  bool is_flag;
  bool is_open;
  bool deactivated;
  int neighboring_mines;
};

#endif  // CELL_H
