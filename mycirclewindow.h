#ifndef MYCIRCLEWINDOW_H
#define MYCIRCLEWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QBoxLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QColorDialog>

class MyCircleWindow : public QDialog
{
    Q_OBJECT
public:
    MyCircleWindow(QDialog *parent = nullptr);
    ~MyCircleWindow();

signals:
    void draw_circle(int width_line, bool isFill, QColor color_line, QColor color_fill);

private:
    QSpinBox *width_line_input;
    QPushButton *button_line_color;
    QPushButton *button_fill_color;
    QCheckBox *fill;
    QLabel *text;
    QColor *color_line;
    QColor *color_fill;
    QPushButton *button_ok;
    QPushButton *button_cancel;
    QHBoxLayout *horizontal_layout;
    QHBoxLayout *horizontal_layout_2;
    QVBoxLayout *vertical_layout_0;
    QVBoxLayout *vertical_layout_1;
    QVBoxLayout *vertical_layout_2;
    QHBoxLayout *horizontal_layout_0;
    QHBoxLayout *horizontal_layout_1;

private slots:
    void draw_circle();
    void set_line_color();
    void set_fill_color();
};

#endif // MYCIRCLEWINDOW_H
