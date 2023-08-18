#include "saper3dlogic.h"

GameLogic::GameLogic() {
  grid_size_ = 1;
  closed_cell_ = grid_size_ * grid_size_ * grid_size_;
  srand(static_cast<unsigned int>(time(nullptr)));
  end_game_ = false;
  deactiveted_mines_ = 0;
  count_mines_ = 0;
  InitializeGame();
}

void GameLogic::InitializeGame() {
  deactiveted_mines_ = 0;
  end_game_ = false;
  for (int x = 0; x < grid_size_; ++x) {
    for (int y = 0; y < grid_size_; ++y) {
      for (int z = 0; z < grid_size_; ++z) {
        gameGrid[x][y][z].has_mine = false;
        gameGrid[x][y][z].is_flag = false;
        gameGrid[x][y][z].is_open = false;
        gameGrid[x][y][z].neighboring_mines = 0;
        gameGrid[x][y][z].deactivated = false;
      }
    }
  }
  std::fill(&gameGrid[0][0][0],
            &gameGrid[0][0][0] + kMapSize * kMapSize * kMapSize,
            Cell{false, false, false, false, 0});
  srand(static_cast<unsigned int>(time(nullptr)));
  for (int i = 0; i < count_mines_; ++i) {
    int x = rand() % grid_size_;
    int y = rand() % grid_size_;
    int z = rand() % grid_size_;

    if (gameGrid[x][y][z].has_mine) {
      i--;
    } else {
      gameGrid[x][y][z].has_mine = true;

      for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
          for (int dz = -1; dz <= 1; ++dz) {
            int new_x = x + dx;
            int new_y = y + dy;
            int new_z = z + dz;

            if (IsCellInMap(new_x, new_y, new_z)) {
              gameGrid[new_x][new_y][new_z].neighboring_mines++;
            }
          }
        }
      }
    }
  }
}

bool GameLogic::IsCellInMap(int x, int y, int z) const {
  return x >= 0 && x < grid_size_ && y >= 0 && y < grid_size_ && z >= 0 &&
         z < grid_size_;
}

void GameLogic::OpenField(int x, int y, int z) {
  if (!IsCellInMap(x, y, z) || gameGrid[x][y][z].is_open) return;

  gameGrid[x][y][z].is_open = true;
  closed_cell_--;

  if (gameGrid[x][y][z].neighboring_mines == 0) {
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        for (int dz = -1; dz <= 1; ++dz) {
          int new_x = x + dx;
          int new_y = y + dy;
          int new_z = z + dz;

          if (IsCellInMap(new_x, new_y, new_z) &&
              !gameGrid[new_x][new_y][new_z].has_mine) {
            OpenField(new_x, new_y, new_z);
          }
        }
      }
    }
  }
  if (gameGrid[x][y][z].has_mine) {
    end_game_ = true;
    for (int i = 0; i < grid_size_; ++i) {
      for (int j = 0; j < grid_size_; ++j) {
        for (int k = 0; k < grid_size_; ++k) {
          gameGrid[j][i][k].is_open = true;
        }
      }
    }
  }
}

void GameLogic::MineClearance(int x, int y, int z) {
  gameGrid[x][y][z].has_mine = false;
  gameGrid[x][y][z].is_open = true;
  gameGrid[x][y][z].deactivated = true;
  deactiveted_mines_++;
  end_game_ = false;
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      for (int dz = -1; dz <= 1; ++dz) {
        int new_x = x + dx;
        int new_y = y + dy;
        int new_z = z + dz;
        if (IsCellInMap(new_x, new_y, new_z) &&
            !gameGrid[new_x][new_y][new_z].has_mine) {
          gameGrid[new_x][new_y][new_z].neighboring_mines--;
        }
      }
    }
  }
}

int GameLogic::get_gridSize() const { return grid_size_; }

int GameLogic::get_closedCell() const { return closed_cell_; }

int GameLogic::get_deactivetedMines() const { return deactiveted_mines_; }

int GameLogic::get_countMines() const { return count_mines_; }

const Cell& GameLogic::get_gameGrid(int x, int y, int z) const {
  return gameGrid[x][y][z];
}

void GameLogic::SetReverseFlag(int x, int y, int z) {
  gameGrid[x][y][z].is_flag = !gameGrid[x][y][z].is_flag;
}

void GameLogic::SetStartingState(int new_size) {
  grid_size_ = new_size;
  if (new_size == 5)
    count_mines_ = 8;
  else if (new_size == 7)
    count_mines_ = 24;
  else if (new_size == 10)
    count_mines_ = 80;

  closed_cell_ = new_size * new_size * new_size;
  end_game_ = false;

  InitializeGame();
}
