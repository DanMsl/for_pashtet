#ifndef FRAMEWINDOW_H
#define FRAMEWINDOW_H

#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QBoxLayout>
#include <QColorDialog>
#include <QSpinBox>
#include <QLabel>

class FrameWindow : public QDialog
{
    Q_OBJECT
public:
    explicit FrameWindow(QDialog *parent = nullptr);

signals:
    void drawfirstFrame(QColor, int);
    void drawsecondFrame(QColor, int);
    void drawthirdFrame(QColor, int);

public slots:
    void frames();
    void setcolor();
    void setFirstFrame();
    void setSecondFrame();
    void setThirdFrame();

private:
    QPushButton *button_ok;
    QPushButton *button_cancel;
    QPushButton *button_color;
    QRadioButton *firstFrame;
    QRadioButton *secondFrame;
    QRadioButton *thirdFrame;
    QSpinBox *width_line;
    QLabel *text;
    QHBoxLayout *horizontal_layout_1;
    QHBoxLayout *horizontal_layout_2;
    QHBoxLayout *horizontal_layout_3;
    QVBoxLayout *main_layout;
    int frame_number = 1;
    QColor *color;

};

#endif // FRAMEWINDOW_H
