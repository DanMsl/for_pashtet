#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFileDialog>
#include <QToolBar>
#include <QColorDialog>
#include <QMessageBox>
#include "image.h"
#include "mycirclewindow.h"
#include "mygraphicsview.h"
#include "rectanglewindow.h"
#include "turnwindow.h"
#include "framewindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

private:
    QWidget *central_widget;
    QHBoxLayout *horizontal_layout_0;
    QVBoxLayout *vertical_layout;
    MyCircleWindow *circle_window;
    RectangleWindow *rectangle_window;
    TurnWindow *turn_window;
    FrameWindow *frame_window;
    Ui::MainWindow *ui;
    Image *img;
    mygraphicsview *view;
    QGraphicsScene *scene;
    QPushButton *button_1;
    QPushButton *button_2;
    QPushButton *button_3;
    QPushButton *button_4;
};

#endif // MAINWINDOW_H
