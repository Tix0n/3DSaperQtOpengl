#include "saper3dview.h"

#include <GL/glu.h>

#include <QOpenGLBuffer>

GameView::GameView(QWidget *parent) : QGLWidget(parent), gameLogic() {
  setMouseTracking(true);
  mouse_pressed_ = false;
  mouse_moved_after_press_ = false;
  camera_rotation_x_ = 0.0;
  camera_rotation_y_ = 0.0;
  camera_distance_ = -150.0;
  max_camera_dist_ = -102;
  min_camera_dist_ = -192;
  spacing_ = 1.0;
  cube_size_ = 10.0;
  neutralizing_x_ = -1;
  neutralizing_y_ = -1;
  neutralizing_z_ = -1;
  victory_event_triggered_ = false;
}

void GameView::initializeGL() {
  GLfloat light_position[] = {1.0f, 1.0f, 1.0f,
                              0.0f};  // Directional light from upper-right
  GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};  // White light
  GLfloat light_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};  // Slight ambient light
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  qglClearColor(Qt::white);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
}

void GameView::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float fov = 45.0f;
  float grid_diagonal =
      sqrtf(powf(gameLogic.get_gridSize() * (cube_size_ + spacing_), 2) * 5);
  float z_near = grid_diagonal * 0.1f;
  float z_far = grid_diagonal * 2.0f;
  float aspect_ratio = float(w) / float(h);
  gluPerspective(fov, aspect_ratio, z_near, z_far);
}

void GameView::paintGL() {
  qglClearColor(Qt::white);
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  int grid_size = gameLogic.get_gridSize();
  float average = (grid_size * cube_size_ + spacing_ * (grid_size - 1)) / 2;
  glTranslatef(0.0f, 0.0f, camera_distance_);
  glRotatef(camera_rotation_x_, 1.0f, 0.0f, 0.0f);
  glRotatef(camera_rotation_y_, 0.0f, 1.0f, 0.0f);
  glTranslatef(-average, -average, -average);
  GameShow();
}

void GameView::GameShow() {
  int grid_size = gameLogic.get_gridSize();
  for (int x = 0; x < grid_size; ++x) {
    for (int y = 0; y < grid_size; ++y) {
      for (int z = 0; z < grid_size; ++z) {
        float center_x = x * (cube_size_ + spacing_) + cube_size_ / 2;
        float center_y = y * (cube_size_ + spacing_) + cube_size_ / 2;
        float center_z = z * (cube_size_ + spacing_) + cube_size_ / 2;
        if (gameLogic.get_gameGrid(x, y, z).deactivated) {
          DrawGreenMine(center_x, center_y, center_z);
        } else {
          if (gameLogic.get_gameGrid(x, y, z).is_open) {
            if (gameLogic.get_gameGrid(x, y, z).neighboring_mines > 0 &&
                !gameLogic.get_gameGrid(x, y, z).has_mine) {
              DrawDigits(center_x, center_y, center_z,
                         gameLogic.get_gameGrid(x, y, z).neighboring_mines);
            }
          } else {
            bool flag = false;
            if (gameLogic.get_gameGrid(x, y, z).is_flag) {
              flag = true;
            }
            DrawCube(center_x, center_y, center_z, cube_size_, flag);
          }
        }
      }
    }
  }
  bool all_mines_deactivated =
      gameLogic.get_deactivetedMines() == gameLogic.get_countMines();
  bool all_empty_cells_opened =
      gameLogic.get_closedCell() == gameLogic.get_countMines();
  if (all_mines_deactivated || all_empty_cells_opened) {
    if (!victory_event_triggered_) {
      victory_event_triggered_ = true;
      emit gameVictory();
    } else {
      victory_event_triggered_ = false;
    }
  }
}

void GameView::DrawDigits(float x, float y, float z, int digits) {
  glPushMatrix();

  glTranslatef(x, y, z);

  glRotatef(-camera_rotation_x_, 1.0f, 0.0f, 0.0f);
  glRotatef(-camera_rotation_y_, 0.0f, 1.0f, 0.0f);

  GLUquadricObj *quadric = gluNewQuadric();

  GLfloat material_diffuse_digits[] = {1.0f, 0.5f, 0.0f, 1.0f};
  GLfloat material_ambient_digits[] = {0.5f, 0.2f, 0.0f, 1.0f};
  SetMaterial(material_diffuse_digits, material_ambient_digits);

  float sphere_radius = 0.75;
  std::vector<std::pair<float, float>> positions = getDigitPositions(digits);

  for (const auto &position : positions) {
    glPushMatrix();
    glTranslatef(position.first, position.second, 0);
    gluSphere(quadric, sphere_radius, 10, 10);
    glPopMatrix();
  }

  gluDeleteQuadric(quadric);

  glPopMatrix();
}

std::vector<std::pair<float, float>> GameView::getDigitPositions(int digits) {
  static const std::vector<std::vector<std::pair<float, float>>> position_data =
      {
          {{0, 0}},                                                      // 1
          {{-1.25, 0}, {1.25, 0}},                                       // 2
          {{-1.25, -1.25}, {1.25, 1.25}, {0, 0}},                        // 3
          {{-1.25, -1.25}, {-1.25, 1.25}, {1.25, 1.25}, {1.25, -1.25}},  // 4
          {{0, 0},
           {-1.25, -1.25},
           {-1.25, 1.25},
           {1.25, 1.25},
           {1.25, -1.25}},  // 5
          {{-1.25, -1.25},
           {-1.25, 1.25},
           {1.25, 1.25},
           {1.25, -1.25},
           {1.25, 0},
           {-1.25, 0}},  // 6
          {{-1.25, -1.25},
           {-1.25, 1.25},
           {1.25, 1.25},
           {1.25, -1.25},
           {1.25, 0},
           {-1.25, 0},
           {0, 0}},  // 7
          {{-1.25, -1.25},
           {-1.25, 1.25},
           {1.25, 1.25},
           {1.25, -1.25},
           {1.25, 0},
           {-1.25, 0},
           {0, 1.25},
           {0, -1.25}},  // 8
          {{-1.25, -1.25},
           {-1.25, 1.25},
           {1.25, 1.25},
           {1.25, -1.25},
           {1.25, 0},
           {-1.25, 0},
           {0, 1.25},
           {0, -1.25},
           {0, 0}}  // 9
      };

  if (digits > 0 && digits - 1 < static_cast<int>(position_data.size())) {
    return position_data[digits - 1];
  }
  return {};
}

void GameView::DrawCube(float x, float y, float z, float size, bool flag) {
  float half_size = size / 2.0f;
  float ver_cub[] = {
      -half_size, -half_size, half_size,  -half_size, half_size,  half_size,
      half_size,  half_size,  half_size,  half_size,  -half_size, half_size,

      -half_size, -half_size, -half_size, -half_size, half_size,  -half_size,
      half_size,  half_size,  -half_size, half_size,  -half_size, -half_size};

  GLfloat normals[] = {0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
                       0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,

                       0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                       0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f};

  GLuint indices[] = {0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 4, 0, 3, 4, 3, 7,
                      1, 5, 6, 1, 6, 2, 1, 0, 4, 1, 4, 5, 3, 2, 6, 3, 6, 7};

  GLfloat material_diffuse_cube[] = {0.0f, 0.6f, 1.0f, 1.0f};
  GLfloat material_ambient_cube[] = {0.0f, 0.2f, 0.4f, 1.0f};
  if (flag) {
    material_diffuse_cube[0] = 1.0f;
    material_diffuse_cube[1] = 0.0f;
    material_diffuse_cube[2] = 0.0f;

    material_ambient_cube[0] = 0.4f;
    material_ambient_cube[1] = 0.0f;
    material_ambient_cube[2] = 0.0f;
  }
  GLfloat material_specular_cube[] = {0.3f, 0.3f, 0.3f, 1.0f};
  GLfloat shininess_cube = 32.0f;

  SetMaterial(material_diffuse_cube, material_ambient_cube);
  glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_cube);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess_cube);

  glVertexPointer(3, GL_FLOAT, 0, ver_cub);
  glEnableClientState(GL_VERTEX_ARRAY);

  glNormalPointer(GL_FLOAT, 0, normals);
  glEnableClientState(GL_NORMAL_ARRAY);

  glPushMatrix();
  glTranslatef(x, y, z);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

  glPopMatrix();

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void GameView::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
    mouse_pressed_ = true;
    last_mouse_pos_ = event->pos();
    mouse_moved_after_press_ = false;
  }
}

void GameView::mouseMoveEvent(QMouseEvent *event) {
  if (mouse_pressed_) {
    mouse_moved_after_press_ = true;
    int delta_x = event->x() - last_mouse_pos_.x();
    int delta_y = event->y() - last_mouse_pos_.y();
    camera_rotation_y_ += delta_x * 0.5f;
    camera_rotation_x_ += delta_y * 0.5f;
    last_mouse_pos_ = event->pos();
    update();
  }
}

void GameView::mouseReleaseEvent(QMouseEvent *event) {
  if (!mouse_moved_after_press_) {
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat win_x, win_y, win_z;
    GLdouble pos_x, pos_y, pos_z;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    win_x = (float)last_mouse_pos_.x();
    win_y = (float)viewport[3] - (float)last_mouse_pos_.y();
    glReadPixels((int)win_x, (int)win_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT,
                 &win_z);

    gluUnProject(win_x, win_y, win_z, modelview, projection, viewport, &pos_x,
                 &pos_y, &pos_z);

    int index_x = static_cast<int>(pos_x / (spacing_ + cube_size_));
    int index_y = static_cast<int>(pos_y / (spacing_ + cube_size_));
    int index_z = static_cast<int>(pos_z / (spacing_ + cube_size_));

    if (event->button() == Qt::LeftButton &&
        !gameLogic.get_gameGrid(index_x, index_y, index_z).has_mine) {
      if (gameLogic.IsCellInMap(index_x, index_y, index_z) &&
          !gameLogic.get_gameGrid(index_x, index_y, index_z).is_flag) {
        gameLogic.OpenField(index_x, index_y, index_z);
        update();
      }
    } else if (event->button() == Qt::RightButton) {
      if (gameLogic.IsCellInMap(index_x, index_y, index_z)) {
        gameLogic.SetReverseFlag(index_x, index_y, index_z);
        update();
      }
    }
    if (event->button() == Qt::LeftButton) {
      if (gameLogic.IsCellInMap(index_x, index_y, index_z)) {
        if (gameLogic.get_gameGrid(index_x, index_y, index_z).has_mine) {
          neutralizing_x_ = index_x;
          neutralizing_y_ = index_y;
          neutralizing_z_ = index_z;
          emit mineOpened();
        }
      }
    }
  }
  mouse_pressed_ = false;
}

void GameView::wheelEvent(QWheelEvent *event) {
  int delta = event->angleDelta().y();
  camera_distance_ += delta * 0.1f;
  if (camera_distance_ - max_camera_dist_ > 1e-6)
    camera_distance_ = max_camera_dist_;
  if (min_camera_dist_ - camera_distance_ > 1e-6)
    camera_distance_ = min_camera_dist_;
  update();
}

void GameView::receiveGame2DResult(bool is_victory) {
  if (is_victory) {
    gameLogic.MineClearance(neutralizing_x_, neutralizing_y_, neutralizing_z_);
    update();
  }
}

void GameView::DrawGreenMine(int x, int y, int z) {
  float size = cube_size_ / 2.0;
  glPushMatrix();

  glTranslatef(x, y, z);
  GLfloat material_diffuse_mine[] = {0.0f, 0.5f, 0.0f, 1.0f};
  GLfloat material_ambient_mine[] = {0.0f, 0.2f, 0.0f, 1.0f};
  SetMaterial(material_diffuse_mine, material_ambient_mine);

  GLUquadricObj *body = gluNewQuadric();
  gluSphere(body, size / 2.0f, 20, 20);
  gluDeleteQuadric(body);

  DrawSpike(material_diffuse_mine, material_ambient_mine, size, true);
  DrawSpike(material_diffuse_mine, material_ambient_mine, size, false);

  glPopMatrix();
}

void GameView::DrawSpike(GLfloat material_diffuse[], GLfloat material_ambient[],
                         float size, bool xAxis) {
  SetMaterial(material_diffuse, material_ambient);
  GLUquadricObj *spike = gluNewQuadric();
  float rotation_axis[3] = {0.0f, 0.0f, 0.0f};
  rotation_axis[xAxis ? 0 : 1] = 1.0f;
  for (int i = 0; i < 8; ++i) {
    glPushMatrix();
    glRotatef(i * 45.0f, rotation_axis[0], rotation_axis[1], rotation_axis[2]);
    glTranslatef(0.0f, 0.0f, size / 2.0f);
    gluCylinder(spike, size * 0.1f, size * 0.05f, size * 0.3f, 10, 1);
    glPopMatrix();
  }
  gluDeleteQuadric(spike);
}

void GameView::SetMaterial(GLfloat diffuse[], GLfloat ambient[]) {
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
}

void GameView::SetStartingState(int new_size) {
  gameLogic.SetStartingState(new_size);
  camera_distance_ = -150.0;
  if (new_size == 7) {
    max_camera_dist_ = -140;
    min_camera_dist_ = -230;
    camera_distance_ = -200;
  }
  if (new_size == 10) {
    max_camera_dist_ = -198;
    min_camera_dist_ = -288;
    camera_distance_ = -250;
  }
  update();
}
