#include "saper2d.h"

#include <QMouseEvent>
#include <QTimer>
#include <iostream>

MinesweeperGame::MinesweeperGame(QWidget *parent) : QOpenGLWidget(parent) {
  map_w_ = kMapSize;
  map_h_ = kMapSize;
  mines_ = kDefaultNumbersMines;
  srand(static_cast<unsigned int>(time(nullptr)));
  GameNew();
}

void MinesweeperGame::initializeGL() { glClearColor(1.0, 1.0, 1.0, 1.0); }

void MinesweeperGame::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  GameShow();
}

void MinesweeperGame::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScalef(2.0 / map_w_, 2.0 / map_h_, 0);
  glTranslatef(-map_w_ * 0.5, -map_h_ * 0.5, 0);
}

void MinesweeperGame::GameNew() {
  closed_cell_ = map_w_ * map_h_;
  failed_ = false;
  std::fill(&map[0][0], &map[0][0] + map_w_ * map_h_,
            Cell{false, false, false, false, 0});
  srand(static_cast<unsigned int>(time(nullptr)));
  for (int i = 0; i < mines_; ++i) {
    int x = rand() % map_w_;
    int y = rand() % map_h_;
    if (map[x][y].has_mine) {
      i--;
    } else {
      map[x][y].has_mine = true;
      for (int dx = -1; dx < 2; dx++) {
        for (int dy = -1; dy < 2; dy++) {
          if (IsCellInMap(x + dx, y + dy))
            map[x + dx][y + dy].neighboring_mines += 1;
        }
      }
    }
  }
}

void MinesweeperGame::GameShow() {
  for (int i = 0; i < map_h_; ++i) {
    for (int j = 0; j < map_w_; ++j) {
      glPushMatrix();
      glTranslatef(j, i, 0);
      if (map[j][i].is_open) {
        DrawFieldOpen();
        if (map[j][i].has_mine) {
          DrawMine();
        } else if (map[j][i].neighboring_mines > 0) {
          DrawDigits(map[j][i].neighboring_mines);
        }
      } else {
        DrawField();
        if (map[j][i].is_flag) {
          DrawFlag();
        }
      }
      glPopMatrix();
    }
  }
}

void MinesweeperGame::DrawField() {
  glBegin(GL_TRIANGLE_STRIP);

  glColor3f(0.8, 0.8, 0.8);
  glVertex2f(0, 1);
  glColor3f(0.7, 0.7, 0.7);
  glVertex2f(1, 1);
  glVertex2f(0, 0);
  glColor3f(0.6, 0.6, 0.6);
  glVertex2f(1, 0);

  glEnd();
}

void MinesweeperGame::Line(float x1, float y1, float x2, float y2) {
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
}

void MinesweeperGame::DrawDigits(int a) {
  glLineWidth(3);
  glColor3f(1, 1, 0);
  glBegin(GL_LINES);

  if (a != 1 && a != 4 && a != 7) Line(0.3, 0.85, 0.7, 0.85);
  if (a != 0 && a != 1 && a != 7) Line(0.3, 0.5, 0.7, 0.5);
  if (a != 1 && a != 4 && a != 7) Line(0.3, 0.15, 0.7, 0.15);

  if (a != 5 && a != 6) Line(0.7, 0.5, 0.7, 0.85);
  if (a != 2) Line(0.7, 0.5, 0.7, 0.15);

  if (a != 1 && a != 2 && a != 3 && a != 7) Line(0.3, 0.5, 0.3, 0.85);
  if (a == 0 || a == 2 || a == 6 || a == 8) Line(0.3, 0.5, 0.3, 0.15);

  glEnd();
}

void MinesweeperGame::DrawMine() {
  glBegin(GL_TRIANGLE_FAN);

  glColor3f(0, 0, 0);
  glVertex2f(0.3, 0.3);
  glVertex2f(0.7, 0.3);
  glVertex2f(0.7, 0.7);
  glVertex2f(0.3, 0.7);

  glEnd();
}

void MinesweeperGame::DrawFieldOpen() {
  glBegin(GL_TRIANGLE_STRIP);

  glColor3f(0.3, 0.7, 0.3);
  glVertex2f(0, 1);
  glColor3f(0.3, 0.6, 0.3);
  glVertex2f(1, 1);
  glVertex2f(0, 0);
  glColor3f(0.3, 0.5, 0.3);
  glVertex2f(1, 0);

  glEnd();
}

void MinesweeperGame::DrawFlag() {
  glBegin(GL_TRIANGLES);

  glColor3f(1, 0, 0);
  glVertex2f(0.25, 0.75);
  glVertex2f(0.85, 0.5);
  glVertex2f(0.25, 0.25);

  glEnd();
  glLineWidth(5);

  glBegin(GL_LINES);

  glColor3f(0, 0, 0);
  glVertex2f(0.25, 0.75);
  glVertex2f(0.25, 0.0);

  glEnd();
}

void MinesweeperGame::OpenFields(int x, int y) {
  if (!IsCellInMap(x, y) || map[x][y].is_open) return;
  map[x][y].is_open = true;
  closed_cell_--;
  if (map[x][y].neighboring_mines == 0)
    for (int dx = -1; dx < 2; dx++)
      for (int dy = -1; dy < 2; dy++)
        if (IsCellInMap(x + dx, y + dy)) OpenFields(x + dx, y + dy);

  if (map[x][y].has_mine) {
    failed_ = true;
    for (int i = 0; i < map_h_; ++i)
      for (int j = 0; j < map_w_; ++j) map[j][i].is_open = true;
  }
}

bool MinesweeperGame::IsCellInMap(int x, int y) {
  return (x >= 0 && y >= 0 && x < map_w_ && y < map_h_);
}

void MinesweeperGame::mousePressEvent(QMouseEvent *event) {
  float x_opengl = 2.0 * event->x() / width() - 1.0;
  float y_opengl = 1.0 - 2.0 * event->y() / height();
  int cellX = (int)((x_opengl + 1.0) * map_w_ * 0.5);
  int cellY = (int)((y_opengl + 1.0) * map_h_ * 0.5);
  if (failed_) {
    gameFinished(false);
  } else {
    if (event->button() == Qt::LeftButton) {
      if (IsCellInMap(cellX, cellY) && !map[cellX][cellY].is_flag)
        OpenFields(cellX, cellY);
      update();
    }
    if (event->button() == Qt::RightButton) {
      if (IsCellInMap(cellX, cellY)) {
        map[cellX][cellY].is_flag = !map[cellX][cellY].is_flag;
      }
      update();
    }
  }
  if (mines_ == closed_cell_ && !failed_) {
    gameFinished(true);
  }
}

void MinesweeperGame::gameFinished(bool is_victory) {
  emit gameFinishedSignal(is_victory);
}

void MinesweeperGame::SetCountMines(int countMines) {
  mines_ = countMines;
  GameNew();
}
