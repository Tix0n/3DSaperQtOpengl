#ifndef SRC_3DSAPER_SAPER3DVIEW_H_
#define SRC_3DSAPER_SAPER3DVIEW_H_

#include <QWidget>
#include <QtOpenGL>
#include <QtWidgets/QWidget>
#include <iostream>

#include "saper3dlogic.h"

class GameView : public QGLWidget {
  Q_OBJECT
 public:
  GameView(QWidget *parent = nullptr);
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();
  void SetStartingState(int newSize);

 protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

 signals:
  void mineOpened();
  void gameVictory();

 public slots:
  void receiveGame2DResult(bool is_victory);

 private:
  GameLogic gameLogic;
  void DrawCube(float x, float y, float z, float size, bool flag);
  void GameShow();
  void DrawDigits(float x, float y, float z, int digits);
  std::vector<std::pair<float, float>> getDigitPositions(int digits);
  void DrawGreenMine(int x, int y, int z);
  void DrawSpike(GLfloat material_diffuse[], GLfloat material_ambient[],
                 float size, bool xAxis);
  void SetMaterial(GLfloat diffuse[], GLfloat ambient[]);

  bool victory_event_triggered_;
  bool mouse_pressed_;
  QPoint last_mouse_pos_;
  float camera_distance_;
  float camera_rotation_x_;
  float camera_rotation_y_;
  int max_camera_dist_;
  int min_camera_dist_;
  bool mouse_moved_after_press_;
  float spacing_;
  float cube_size_;
  int neutralizing_x_;
  int neutralizing_y_;
  int neutralizing_z_;
};
#endif  // SRC_3DSAPER_SAPER3DVIEW_H_
