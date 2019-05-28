#ifndef RECTANGLEWINDOW_H
#define RECTANGLEWINDOW_H

#include <QSpinBox>
#include <QBoxLayout>
#include <QDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QColorDialog>
#include <QColor>


class RectangleWindow : public QDialog
{
    Q_OBJECT
public:
    explicit RectangleWindow(QDialog *parent = nullptr);
    ~RectangleWindow();

signals:
    void do_rectangle(int, bool, QColor, QColor);

private slots:
    void do_rectangle();
    void set_color_line();
    void set_color_fill();

private:
    QSpinBox *width_line_input;
    QPushButton *button_ok;
    QPushButton *button_cancel;
    QPushButton *button_line_color;
    QPushButton *button_fill_color;
    QCheckBox *fill;
    QLabel *text;
    QColor *color_line;
    QColor *color_fill;
    QHBoxLayout *horizontal_layout_2;
    QVBoxLayout *vertical_layout_0;
    QVBoxLayout *vertical_layout_1;
    QVBoxLayout *vertical_layout_2;
    QHBoxLayout *horizontal_layout_0;
    QHBoxLayout *horizontal_layout_1;
};

#endif // RECTANGLEWINDOW_H
