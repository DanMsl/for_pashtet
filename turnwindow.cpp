#include "turnwindow.h"

TurnWindow::TurnWindow(QDialog *parent) : QDialog(parent)
{
    setFixedSize(300, 150);
    setWindowTitle("Turn");
    setModal(true);

    button_ok = new QPushButton;
    button_cancel = new QPushButton;
    horizontal_layout_0 = new QHBoxLayout;
    horizontal_layout_1 = new QHBoxLayout;
    main_layout = new QVBoxLayout;
    radio_button_90 = new QRadioButton;
    radio_button_180 = new QRadioButton;
    radio_button_270 = new QRadioButton;

    button_ok->setText("Ok");
    button_cancel->setText("Cancel");
    radio_button_90->setText("90 angle");
    radio_button_180->setText("180 angle");
    radio_button_270->setText("270 angle");
    horizontal_layout_0->addWidget(button_cancel);
    horizontal_layout_0->addWidget(button_ok);
    horizontal_layout_1->addWidget(radio_button_90);
    horizontal_layout_1->addWidget(radio_button_180);
    horizontal_layout_1->addWidget(radio_button_270);
    main_layout->addLayout(horizontal_layout_1);
    main_layout->addLayout(horizontal_layout_0);
    setLayout(main_layout);

    connect(button_ok, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(radio_button_90, SIGNAL(clicked()), this, SLOT(set_angle_90()));
    connect(radio_button_180, SIGNAL(clicked()), this, SLOT(set_angle_180()));
    connect(radio_button_270, SIGNAL(clicked()), this, SLOT(set_angle_270()));
    connect(button_ok, SIGNAL(clicked()), this, SLOT(do_turn()));
}


void TurnWindow::set_angle_90(){
    angle = 90;
}

void TurnWindow::set_angle_180(){
    angle = 180;
}

void TurnWindow::set_angle_270(){
    angle = 270;
}

void TurnWindow::do_turn(){
    emit make_turn(angle);
}

TurnWindow::~TurnWindow(){
    delete button_ok;
    delete button_cancel;
    delete radio_button_90;
    delete radio_button_180;
    delete radio_button_270;
    delete horizontal_layout_0;
    delete horizontal_layout_1;
    delete main_layout;
}
