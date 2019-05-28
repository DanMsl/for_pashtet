#include "framewindow.h"

FrameWindow::FrameWindow(QDialog *parent) : QDialog(parent)
{
    setFixedSize(350, 150);
    setModal(true);
    setWindowTitle("Frame");

    button_ok = new QPushButton;
    button_cancel = new QPushButton;
    button_color = new QPushButton;
    firstFrame = new QRadioButton;
    secondFrame = new QRadioButton;
    thirdFrame = new QRadioButton;
    width_line = new QSpinBox;
    text = new QLabel("Line width");
    horizontal_layout_1 = new QHBoxLayout;
    horizontal_layout_2 = new QHBoxLayout;
    horizontal_layout_3 = new QHBoxLayout;
    main_layout = new QVBoxLayout;
    color = new QColor;
    color->setRed(0);
    color->setBlue(0);
    color->setGreen(0);

    button_ok->setText("Ok");
    button_cancel->setText("Cancel");
    button_color->setText("Set color");
    firstFrame->setText("First frame");
    secondFrame->setText("Second frame");
    thirdFrame->setText("Third frame");
    width_line->setRange(1, 50);
    horizontal_layout_1->addWidget(firstFrame);
    horizontal_layout_1->addWidget(secondFrame);
    horizontal_layout_1->addWidget(thirdFrame);
    horizontal_layout_2->addWidget(button_cancel);
    horizontal_layout_2->addWidget(button_ok);
    horizontal_layout_3->addWidget(button_color);
    horizontal_layout_3->addWidget(text);
    horizontal_layout_3->addWidget(width_line);
    main_layout->addLayout(horizontal_layout_1);
    main_layout->addLayout(horizontal_layout_3);
    main_layout->addLayout(horizontal_layout_2);
    setLayout(main_layout);

    connect(button_ok, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(firstFrame, SIGNAL(clicked()), this, SLOT(setFirstFrame()));
    connect(secondFrame, SIGNAL(clicked()), this, SLOT(setSecondFrame()));
    connect(thirdFrame, SIGNAL(clicked()), this, SLOT(setThirdFrame()));
    connect(button_ok, SIGNAL(clicked()), this, SLOT(frames()));
    connect(button_color, SIGNAL(clicked()), this, SLOT(setcolor()));
}

void FrameWindow::setFirstFrame(){
    frame_number = 1;
}

void FrameWindow::setSecondFrame(){
    frame_number = 2;
}

void FrameWindow::setThirdFrame(){
    frame_number = 3;
}

void FrameWindow::frames(){
    switch (frame_number) {
    case 1: emit drawfirstFrame(*color, width_line->value());
        break;
    case 2: emit drawsecondFrame(*color, width_line->value());
        break;
    case 3: emit drawthirdFrame(*color, width_line->value());
        break;

    }
}

void FrameWindow::setcolor(){
    *color = QColorDialog::getColor();
}
