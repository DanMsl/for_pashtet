#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QPoint>
#include <QColor>
#include <QMouseEvent>
#include <QGraphicsItemGroup>
#include <iostream>

class mygraphicsview : public QGraphicsView
{
    Q_OBJECT
public:
    void deleteItemsFromGroup(QGraphicsItemGroup *group);
    explicit mygraphicsview(QPixmap pixmap, QGraphicsView *parent = nullptr);

signals:
    void draw_circle(int width_line, bool isFill, QColor color_line, QColor color_fill, QPoint center, int radius);
    void rectangle(QPoint *start_point, QPoint *end_point, int width_line, bool isFill,
                       QColor color_lines, QColor color_fill);
    void turn(QPoint* start_point, QPoint* end_point, int angle);


private slots:
    void update_image(QPixmap pixmap);
    void draw_circle(int width_line, bool isFill, QColor color_line, QColor color_fill);
    void do_rectangle(int width_line, bool isFill, QColor line_color, QColor fill_color);
    void make_turn(int angle);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QGraphicsItemGroup *group;
    QPixmap *pixmap;
    int width_line;
    bool isCircle = false;
    bool isRectangle = false;
    bool isTurn = false;
    QColor fill_color;
    QColor line_color;
    QPoint *start_point;
    QPoint *end_point;
    bool isFill;
    QPoint *pos;
    QGraphicsScene *scene;
    int *diameter;
    int angle;
};

#endif // MYGRAPHICSVIEW_H
