#ifndef SRC_3DSAPER_SAPER2D_H_
#define SRC_3DSAPER_SAPER2D_H_

#include <QObject>
#include <QOpenGLWidget>

#include "cell.h"

class MinesweeperGame : public QOpenGLWidget {
  Q_OBJECT
 public:
  MinesweeperGame(QWidget *parent);

  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;

  void GameNew();
  void SetCountMines(int countMines);

 protected:
  void mousePressEvent(QMouseEvent *event);

 public slots:
  void gameFinished(bool is_victory);

 signals:
  void gameFinishedSignal(bool is_victory);

 private:
  void DrawField();
  void GameShow();
  void DrawFlag();
  void DrawDigits(int a);
  void DrawMine();
  void DrawFieldOpen();
  void OpenFields(int x, int y);
  bool IsCellInMap(int x, int y);
  void Line(float x1, float y1, float x2, float y2);

  static const int kMapSize = 10;
  static const int kDefaultNumbersMines = 10;
  Cell map[kMapSize][kMapSize];
  int map_w_;
  int map_h_;
  int mines_;
  int closed_cell_;
  bool failed_;
};

#endif  // SRC_3DSAPER_SAPER2D_H_
