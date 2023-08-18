#ifndef SRC_3DSAPER_MAINWINDOW_H_
#define SRC_3DSAPER_MAINWINDOW_H_

#include <QDebug>
#include <QMainWindow>
#include <QStackedWidget>

#include "saper2d.h"
#include "saper3dview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  QStackedWidget *stackedWidget;
  GameView *game3D;
  MinesweeperGame *minesweeperGame;
  QWidget *sizeSelectWidget;
  QHBoxLayout *sizeSelectLayout;
  void startGame10x10();
  void startGame7x7();
  void startGame5x5();

 public slots:
  void switchToMinesweeperGame();
  void switchTo3D(bool is_victory);
  void switchToModeSelection();
  void receiveGameVictory();

 signals:
  void game2DResult(bool is_victory);
};
#endif  // SRC_3DSAPER_MAINWINDOW_H_
