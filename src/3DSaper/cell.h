#ifndef SRC_CELL_H_
#define SRC_CELL_H_

struct Cell {
  bool has_mine;
  bool is_flag;
  bool is_open;
  bool deactivated;
  int neighboring_mines;
};

#endif  // SRC_CELL_H_
