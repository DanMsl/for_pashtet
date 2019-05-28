#ifndef TURNWINDOW_H
#define TURNWINDOW_H

#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QBoxLayout>

class TurnWindow : public QDialog
{
    Q_OBJECT
public:
    explicit TurnWindow(QDialog *parent = nullptr);
    ~TurnWindow();

signals:
    void make_turn(int angle);

public slots:
    void set_angle_90();
    void set_angle_180();
    void set_angle_270();
    void do_turn();

private:
    QPushButton *button_ok;
    QPushButton *button_cancel;
    QRadioButton *radio_button_90;
    QRadioButton *radio_button_180;
    QRadioButton *radio_button_270;
    QHBoxLayout *horizontal_layout_0;
    QHBoxLayout *horizontal_layout_1;
    QVBoxLayout *main_layout;
    int angle;
};

#endif // TURNWINDOW_H
