#include "mygraphicsview.h"
#include <QDebug>

mygraphicsview::mygraphicsview(QPixmap pixmap, QGraphicsView *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene;
    start_point = new QPoint;
    end_point = new QPoint;
    diameter = new int;
    pos = new QPoint;
    group = new QGraphicsItemGroup;
    this->pixmap = new QPixmap;
    scene->addPixmap(pixmap);
    this->setScene(scene);
    scene->addItem(group);
    isCircle = false;

    *this->pixmap = pixmap;
    this->setMinimumWidth(200);
    this->setMinimumHeight(200);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    scene->setSceneRect(0, 0, pixmap.width(), pixmap.height());
}

void mygraphicsview::update_image(QPixmap pixmap) {
    scene->removeItem(group);
    scene->clear();
    *this->pixmap = pixmap;
    scene->setSceneRect(0, 0, pixmap.width(), pixmap.height());
    scene->addPixmap(pixmap);
    scene->addItem(group);
}

void mygraphicsview::draw_circle(int width_line, bool isFill, QColor color_line, QColor color_fill) {
    this->isRectangle = false;
    this->isTurn = false;
    this->width_line = width_line;
    this->isFill = isFill;
    this->fill_color = color_fill;
    this->line_color = color_line;
    this->isCircle = true;
}

void mygraphicsview::mousePressEvent(QMouseEvent *event) {
    *pos = mygraphicsview::mapToScene(event->pos()).toPoint();
    if (isCircle || isRectangle || isTurn)
        *start_point = *pos;
}

void mygraphicsview::mouseMoveEvent(QMouseEvent *event){
    *pos = mygraphicsview::mapToScene(event->pos()).toPoint();
    int x = pos->x();
    int y = pos->y();

    if (pos->x() >= this->pixmap->width())
        x = this->pixmap->width() - 1;
    if (pos->x() < 0)
        x = 0;
    if (pos->y() >= this->pixmap->height())
        y = this->pixmap->height() - 1;
    if (pos->y() < 0)
        y = 0;

    if (x > y && pos->x() < this->pixmap->width() && pos->y() < this->pixmap->height()) {
        *diameter = abs(x - start_point->x());
    }
    else if (pos->x() < this->pixmap->width() && pos->y() < this->pixmap->height()){
        *diameter = abs(y - start_point->y());
    }

    if (isCircle) {
        if (start_point->x() >= 0 &&
                start_point->x() < this->pixmap->width() &&
                start_point->y() >= 0 &&
                start_point->y() < this->pixmap->height() &&
                start_point->x() < x &&
                start_point->y() < y &&
                start_point->x() + *diameter < this->pixmap->width() &&
                start_point->y() + *diameter < this->pixmap->height()) {

            deleteItemsFromGroup(group);
            QPen penBlack(Qt::black);

            group->addToGroup((scene->addEllipse(start_point->x(), start_point->y(), *diameter, *diameter, penBlack)));
        }
    }
    if (isRectangle || isTurn) {
           if (start_point->x() >= 0 &&
                   start_point->x() < this->pixmap->width() &&
                   start_point->y() >= 0 &&
                   start_point->y() < this->pixmap->height()) {

               deleteItemsFromGroup(group);
               QPen penBlack(Qt::black);
               group->addToGroup(scene->addLine(start_point->x(), start_point->y(), x, start_point->y(), penBlack));
               group->addToGroup(scene->addLine(start_point->x(), start_point->y(), start_point->x(), y, penBlack));
               group->addToGroup(scene->addLine(start_point->x(), y, x, y, penBlack));
               group->addToGroup(scene->addLine(x, start_point->y(), x, y, penBlack));
           }
       }


}

void mygraphicsview::mouseReleaseEvent(QMouseEvent *event) {
    *pos = mygraphicsview::mapToScene(event->pos()).toPoint();
    deleteItemsFromGroup(group);
    int x = pos->x();
    int y = pos->y();

    if (pos->x() >= this->pixmap->width())
        x = this->pixmap->width() - 1;
    if (pos->x() < 0)
        x = 0;
    if (pos->y() >= this->pixmap->height())
        y = this->pixmap->height() - 1;
    if (pos->y() < 0)
        y = 0;
    end_point->setX(x);
    end_point->setY(y);

    if(isCircle) {
        if (start_point->x() >= 0 &&
                start_point->x() < this->pixmap->width() &&
                start_point->y() >= 0 &&
                start_point->y() < this->pixmap->height() &&
                start_point->x() < x &&
                start_point->y() < y) {
            this->isCircle = false;
            QPoint center;
            center.setX(start_point->x() + *diameter/2);
            center.setY(start_point->y() + *diameter/2);

            emit draw_circle(width_line, isFill, line_color, fill_color, center, *diameter/2);

        }
    }
    else if(isRectangle || isTurn) {
           if (start_point->x() >= 0 &&
                   start_point->x() < this->pixmap->width() &&
                   start_point->y() >= 0 &&
                   start_point->y() < this->pixmap->height()) {
               if(isRectangle){
               this->isRectangle = false;
               emit rectangle(start_point, end_point, width_line, isFill, line_color, fill_color);
               }
               if(isTurn){
                   this->isTurn = false;
                   emit turn(start_point, end_point, angle);
               }
           }
   }
}




void mygraphicsview::deleteItemsFromGroup(QGraphicsItemGroup *group) {
    foreach(QGraphicsItem *item, scene->items(group->boundingRect())) {
        if(item->group() == group) {
            delete item;
        }
    }
}


void mygraphicsview::do_rectangle(int width_line, bool isFill, QColor line_color, QColor fill_color) {
    this->isCircle = false;
    this->line_color = line_color;
    this->fill_color = fill_color;
    this->width_line = width_line;
    this->isRectangle = true;
    this->isFill = isFill;
}

void mygraphicsview::make_turn(int angle){
    isCircle = false;
    isRectangle = false;
    isTurn = true;
    this->angle = angle;
    if(angle > 0)
        this->isTurn = true;
}
