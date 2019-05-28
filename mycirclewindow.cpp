#include "mycirclewindow.h"

MyCircleWindow::MyCircleWindow(QDialog *parent) : QDialog(parent) {
    button_ok = new QPushButton;
    button_cancel = new QPushButton;
    fill = new QCheckBox;
    width_line_input = new QSpinBox;
    vertical_layout_0 = new QVBoxLayout;
    horizontal_layout_0 = new QHBoxLayout;
    horizontal_layout_1 = new QHBoxLayout;
    horizontal_layout_2 = new QHBoxLayout;
    vertical_layout_1 = new QVBoxLayout;
    vertical_layout_2 = new QVBoxLayout;
    button_line_color = new QPushButton;
    button_fill_color = new QPushButton;
    text = new QLabel;
    color_line = new QColor;
    color_fill = new QColor;

    setFixedSize(300, 150);
    text->setText("Line width:");
    button_line_color->setText("Set line color");
    button_fill_color->setText("Set fill color");
    fill->setText("Fill");
    setWindowTitle("Circle");
    setModal(true);
    button_ok->setText("Ok");
    button_cancel->setText("Cancel");
    width_line_input->setRange(1, 100);
    width_line_input->setValue(10);
    color_line->setRgb(0, 0, 0);
    color_fill->setRgb(0, 0, 0);

    vertical_layout_2->addWidget(button_line_color);
    vertical_layout_2->addWidget(button_fill_color);
    horizontal_layout_0->addWidget(button_cancel);
    horizontal_layout_0->addWidget(button_ok);
    horizontal_layout_2->addWidget(text);
    horizontal_layout_2->addWidget(width_line_input);
    vertical_layout_1->addLayout(horizontal_layout_2);
    vertical_layout_1->addWidget(fill);
    horizontal_layout_1->addLayout(vertical_layout_1);
    horizontal_layout_1->addLayout(vertical_layout_2);
    vertical_layout_0->addLayout(horizontal_layout_1);
    vertical_layout_0->addLayout(horizontal_layout_0);
    setLayout(vertical_layout_0);

    connect(button_ok, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(button_line_color, SIGNAL(clicked()), this, SLOT(set_line_color()));
    connect(button_fill_color, SIGNAL(clicked()), this, SLOT(set_fill_color()));
    connect(button_ok, SIGNAL(clicked()), this, SLOT(draw_circle()));
    //connect(this, SIGNAL())
}


void MyCircleWindow::set_line_color() {
    *color_line = QColorDialog::getColor();
}


void MyCircleWindow::set_fill_color() {
    *color_fill = QColorDialog::getColor();
}

void MyCircleWindow::draw_circle() {
    bool isFill = false;
    if (fill->isChecked())
        isFill = true;
    this->close();
    emit draw_circle(width_line_input->value() + 1, isFill, *color_line, *color_fill);
}

MyCircleWindow::~MyCircleWindow(){
    delete button_ok;
    delete button_cancel;
    delete button_fill_color;
    delete button_line_color;
    delete width_line_input;
    delete horizontal_layout;
    delete horizontal_layout_0;
    delete horizontal_layout_1;
    delete horizontal_layout_2;
    delete vertical_layout_0;
    delete vertical_layout_1;
    delete vertical_layout_2;
    delete fill;
    delete text;
    delete color_fill;
    delete color_line;
}
