#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(950, 500);

    central_widget = new QWidget;
    horizontal_layout_0 = new QHBoxLayout;
    circle_window = new MyCircleWindow;
    rectangle_window = new RectangleWindow;
    turn_window = new TurnWindow;
    frame_window = new FrameWindow;
    img = new Image();
    view = new mygraphicsview(img->getPixMap());
    button_1 = new QPushButton;
    button_2 = new QPushButton;
    button_3 = new QPushButton;
    button_4 = new QPushButton;
    vertical_layout = new QVBoxLayout;

    button_1->setText("Circle");
    button_2->setText("Rectangle");
    button_3->setText("Turn");
    button_4->setText("Frame");

    vertical_layout->addWidget(button_1);
    vertical_layout->addWidget(button_2);
    vertical_layout->addWidget(button_3);
    vertical_layout->addWidget(button_4);
    horizontal_layout_0->addLayout(vertical_layout);
    horizontal_layout_0->addWidget(view);
    central_widget->setLayout(horizontal_layout_0);
    this->setCentralWidget(central_widget);

    //Circle
    connect(button_1, SIGNAL(clicked()), circle_window, SLOT(exec()));
    connect(circle_window, SIGNAL(draw_circle(int, bool, QColor, QColor)), view, SLOT(draw_circle(int, bool, QColor, QColor)));
    connect(view, SIGNAL(draw_circle(int, bool, QColor, QColor, QPoint, int)),
            img, SLOT(draw_circle(int, bool, QColor, QColor, QPoint, int)));
    connect(img, SIGNAL(update_image(QPixmap)), view, SLOT(update_image(QPixmap)));

    //Rectangle
    connect(button_2, SIGNAL(clicked()), rectangle_window, SLOT(exec()));
    connect(view, SIGNAL(rectangle(QPoint *, QPoint *, int, bool, QColor, QColor)),
            img, SLOT(rectangle(QPoint *, QPoint *, int, bool, QColor, QColor)));
    connect(rectangle_window, SIGNAL(do_rectangle(int, bool, QColor, QColor)),
            view, SLOT(do_rectangle(int, bool, QColor, QColor)));

    //Flip
    connect(button_3, SIGNAL(clicked()), turn_window, SLOT(exec()));
    connect(view, SIGNAL(turn(QPoint*, QPoint*, int)), img, SLOT(turn(QPoint*, QPoint*, int)));
    connect(turn_window, SIGNAL(make_turn(int)), view, SLOT(make_turn(int)));

    //Frame
    connect(button_4, SIGNAL(clicked()), frame_window, SLOT(exec()));
    connect(frame_window, SIGNAL(drawfirstFrame(QColor, int)), img, SLOT(drawFirstVariantOfFrame(QColor, int)));
    connect(frame_window, SIGNAL(drawsecondFrame(QColor, int)), img, SLOT(drawSecondVariantOfFrame(QColor, int)));
    connect(frame_window, SIGNAL(drawthirdFrame(QColor, int)), img, SLOT(drawThirdVariantOfFrame(QColor, int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(nullptr, tr("Choose file!"), "", tr("Image (*.bmp)"));

    img->load(filename.toLocal8Bit());
    emit img->update_image(img->getPixMap());

}

void MainWindow::on_actionNew_triggered()
{
    img->new_image();
    img->update_image(img->getPixMap());
}

void MainWindow::on_actionSave_triggered()
{
    img->save(QFileDialog::getOpenFileName(nullptr, tr("Choose file!"), "", tr("Image (*.bmp)")).toLocal8Bit());
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(nullptr, "Справка", "Данная программа написана в качестве\n"
                                                    "курсовой работы по программированию за 2 семестр\n"
                                                    "студентом группы 8304 Масалыкиным Даниилом.\n"
                                                    "-----------------------------------------------------------------\n"
                                                    "New - создаёт новое изображение размера 600х400\n"
                                                    "Open - открывает выбранное изображение в формате bmp версии 3\n"
                                                    "Save - сохраняет открытое изображение в файл bmp версии 3\n"
                                                    "------------------------------------------------------------------\n"
                                                    "Circle - рисует круг по выбранным параметрам\n"
                                                    "Rectangle - рисует прямоугольник по выбранным параметрам\n"
                                                    "Frame - создаёт одну из трёх рамок на выбор\n"
                                                    "Turn - поворачивает выбранную область на выбранный угол\n");
}
