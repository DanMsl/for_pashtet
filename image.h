#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>
#include <QColor>
#include <cstdio>
#include <iostream>

#pragma pack(2)
typedef struct BITMAPFILEHEADER {
    int16_t bfType;
    int32_t bfSize;
    int32_t bfReserved;
    int32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct BITMAPINFOHEADER{
    int32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    int16_t biPlanes;
    int16_t biBitCount;
    int32_t biCompression;
    int32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    int32_t biClrUsed;
    int32_t biClrImportant;
}BITMAPINFOHEADER;

typedef struct PIXEL{
    unsigned char b;
    unsigned char g;
    unsigned char r;
}PIXEL;

#pragma pack()

class Image : public QWidget
{
    Q_OBJECT
public:
    explicit Image(QWidget *parent = nullptr);
    explicit Image(const char* file, QWidget *parent = nullptr);
    ~Image();

    int load(const char* file);
    int save(const char* file);
    void new_image();
    QPixmap getPixMap();
    int getHeight();
    int getWidth();
    void setPixel(PIXEL pix, int x, int y);
    void drawSquare(int x1, int y1, int x2, int y2, QColor color, int width_line, int filled);
    void drawLine(int x1, int x2, int y1, int y2, QColor color, int width_line);
    void drawBigPixel(int x0, int y0, int size, QColor color);

private:
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    PIXEL **pixel_array;

signals:
    void update_image(QPixmap pixmap);

public slots:
    void drawFirstVariantOfFrame(QColor color, int frameWidth);
    void drawSecondVariantOfFrame(QColor color, int frameWidth);
    void drawThirdVariantOfFrame(QColor color, int frameWidth);
    void draw_circle(int width_line, bool isFill, QColor color_line, QColor color_fill, QPoint center, int radius);
    void rectangle(QPoint *start_point, QPoint *end_point, int width_line, bool isFill, QColor line_color, QColor fill_color);
    void turn(QPoint *start_point, QPoint* end_point, int angle);
};

#endif // IMAGE_H
