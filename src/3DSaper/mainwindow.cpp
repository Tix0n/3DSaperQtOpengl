#include "mainwindow.h"

#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  sizeSelectWidget = new QWidget(this);
  sizeSelectLayout = new QHBoxLayout(sizeSelectWidget);

  QPushButton *button5x5 = new QPushButton("5 x 5", this);
  QPushButton *button7x7 = new QPushButton("7 x 7", this);
  QPushButton *button10x10 = new QPushButton("10 x 10", this);

  QGraphicsDropShadowEffect *shadowEffect5x5 = new QGraphicsDropShadowEffect;
  QGraphicsDropShadowEffect *shadowEffect7x7 = new QGraphicsDropShadowEffect;
  QGraphicsDropShadowEffect *shadowEffect10x10 = new QGraphicsDropShadowEffect;

  shadowEffect5x5->setBlurRadius(10);
  shadowEffect5x5->setColor(QColor(0, 0, 0, 100));
  button5x5->setGraphicsEffect(shadowEffect5x5);

  shadowEffect7x7->setBlurRadius(10);
  shadowEffect7x7->setColor(QColor(0, 0, 0, 100));
  button7x7->setGraphicsEffect(shadowEffect7x7);

  shadowEffect10x10->setBlurRadius(10);
  shadowEffect10x10->setColor(QColor(0, 0, 0, 100));
  button10x10->setGraphicsEffect(shadowEffect10x10);

  QString buttonStyle =
      "QPushButton {"
      "    border: none;"
      "    padding: 20px;"
      "    color: white;"
      "    background: qradialgradient(cx:0.5, cy:0.5, radius:1, fx:0.5, "
      "fy:0.5, stop:0 #1e90ff, stop:1 #0050a0);"
      "    border-radius: 10px;"
      "}";

  button5x5->setStyleSheet(buttonStyle);
  button7x7->setStyleSheet(buttonStyle);
  button10x10->setStyleSheet(buttonStyle);

  const int buttonSize = 200;
  button5x5->setFixedSize(buttonSize, buttonSize);
  button7x7->setFixedSize(buttonSize, buttonSize);
  button10x10->setFixedSize(buttonSize, buttonSize);

  sizeSelectLayout->addWidget(button5x5);
  sizeSelectLayout->addWidget(button7x7);
  sizeSelectLayout->addWidget(button10x10);

  connect(button5x5, &QPushButton::clicked, this, &MainWindow::startGame5x5);
  connect(button7x7, &QPushButton::clicked, this, &MainWindow::startGame7x7);
  connect(button10x10, &QPushButton::clicked, this,
          &MainWindow::startGame10x10);

  stackedWidget = new QStackedWidget(this);

  stackedWidget->addWidget(sizeSelectWidget);

  game3D = new GameView(this);
  stackedWidget->addWidget(game3D);

  minesweeperGame = new MinesweeperGame(this);
  stackedWidget->addWidget(minesweeperGame);
  setFixedSize(800, 800);
  game3D->setFixedSize(800, 800);
  minesweeperGame->setFixedSize(800, 800);
  setWindowTitle("Menu");

  setCentralWidget(stackedWidget);

  connect(game3D, &GameView::mineOpened, this,
          &MainWindow::switchToMinesweeperGame);
  connect(minesweeperGame, &MinesweeperGame::gameFinishedSignal, this,
          &MainWindow::switchTo3D);
  connect(this, &MainWindow::game2DResult, game3D,
          &GameView::receiveGame2DResult);
  connect(game3D, &GameView::gameVictory, this,
          &MainWindow::receiveGameVictory);
}

void MainWindow::switchToMinesweeperGame() {
  minesweeperGame->GameNew();
  stackedWidget->setCurrentWidget(minesweeperGame);
  setWindowTitle("Minesweeper Game");
}

void MainWindow::switchTo3D(bool is_victory) {
  if (is_victory) {
    stackedWidget->setCurrentWidget(game3D);
    setWindowTitle("3dSaper");
    emit game2DResult(is_victory);
  } else {
    QMessageBox::information(this, "Defeat", "Try again!");
    switchToModeSelection();
  }
}

void MainWindow::receiveGameVictory() {
  QMessageBox::information(this, "Won", "Good job!");
  switchToModeSelection();
}

void MainWindow::switchToModeSelection() {
  setWindowTitle("Menu");
  stackedWidget->setCurrentWidget(sizeSelectWidget);
}

void MainWindow::startGame5x5() {
  game3D->SetStartingState(5);
  minesweeperGame->SetCountMines(10);
  stackedWidget->addWidget(game3D);
  stackedWidget->setCurrentWidget(game3D);
  setWindowTitle("3dSaper");
}

void MainWindow::startGame7x7() {
  game3D->SetStartingState(7);
  minesweeperGame->SetCountMines(15);
  stackedWidget->addWidget(game3D);
  stackedWidget->setCurrentWidget(game3D);
  setWindowTitle("3dSaper");
}

void MainWindow::startGame10x10() {
  game3D->SetStartingState(10);
  minesweeperGame->SetCountMines(20);
  stackedWidget->addWidget(game3D);
  stackedWidget->setCurrentWidget(game3D);
  setWindowTitle("3dSaper");
}

MainWindow::~MainWindow() {
  delete game3D;
  delete minesweeperGame;
  delete sizeSelectWidget;
  delete ui;
}
