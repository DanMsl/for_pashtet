#include "image.h"

Image::Image(const char* file, QWidget *parent) : QWidget(parent)
{
    this->load(file);
}

Image::Image(QWidget *parent) : QWidget(parent){
    this->new_image();
}

void Image::new_image() {
    int width = 600;
    int height = 400;

    size_t padding = 4 - (width * 3) % 4;
    if (padding == 4)
        padding = 0;
    int filesize = 54 + height * width * 3 + static_cast<int>(padding) * height;

    memset(&bfh, 0, sizeof(bfh));
    memset(&bih, 0, sizeof(bih));

    bih.biSize = sizeof(bih);
    bfh.bfType = 0x4d42;
    bfh.bfSize = filesize;
    bfh.bfOffBits  = 14 + bih.biSize;
    bfh.bfReserved =  0;
    bih.biPlanes = 1;
    bih.biWidth = width;
    bih.biHeight = height;
    bih.biBitCount = 24;
    bih.biCompression= 0;

    pixel_array = new PIXEL* [static_cast<unsigned long long>(bih.biHeight)];
    for (int i = 0; i < bih.biHeight; i++) {
        pixel_array[i] = new PIXEL [static_cast<unsigned long long>(bih.biWidth) + static_cast<unsigned long long>(padding)];
    }
    for (int i = 0; i < bih.biHeight; i++) {
        for (int j = 0; j < bih.biWidth; j++) {
            pixel_array[i][j].r = 255;
            pixel_array[i][j].b = 255;
            pixel_array[i][j].g = 255;
        }
    }
}

Image::~Image() {
    for (int i = 0; i < bih.biHeight; i++)
        delete [] pixel_array[i];
    delete []pixel_array;
}

int Image::load(const char *file){
    size_t padding = 0;
    FILE *f;
    fopen_s(&f, file, "rb");

    if(!f){
        std::cout<<"Open File Error"<<std::endl;
        return -1;
    }
    fread(&bfh, sizeof(bfh), 1, f);
    fread(&bih, sizeof(bih), 1, f);

    if(bfh.bfReserved != 0 ||
            (bih.biSize != 40 && bih.biSize != 108 && bih.biSize != 124) ||
            bih.biWidth < 1 || bih.biWidth > 10000 ||
            bih.biHeight < 1 || bih.biHeight > 10000 ||
            bih.biCompression != 0 ||
            bih.biBitCount != 24){
        fclose(f);
        return -1;
    }

    fseek(f, bfh.bfOffBits, SEEK_SET);

    if((bih.biWidth * 3) % 4)
        padding = 4 - (bih.biWidth * 3) % 4;

    pixel_array = new PIXEL *[static_cast<long long unsigned int>(bih.biHeight)];
    for(int i = 0; i < bih.biHeight; i++){
        pixel_array[i] = new PIXEL [static_cast<long long unsigned int>(bih.biWidth) + static_cast<unsigned long long int>(padding)];
    }

    for(int i = 0; i < bih.biHeight; i ++){
        int j;
        for(j = 0; j < bih.biWidth; j++){
            fread(&pixel_array[i][j], sizeof(PIXEL), 1, f);
        }
        if(padding)
            fread(&pixel_array[i][j], padding, 1, f);
    }

    fclose(f);
    return 0;
}


int Image::save(const char *file){
    size_t padding = 0;
    FILE *f;
    fopen_s(&f, file, "wb");

    if(!f)
        return -1;

    fwrite(&bfh, sizeof(bfh), 1, f);
    fwrite(&bih, sizeof(bih), 1, f);

    if((bih.biWidth * 3) % 4)
        padding = 4 - (bih.biWidth * 3) % 4;

    for(int i = 0; i < bih.biHeight; i ++){
        int j;
        for(j = 0; j < bih.biWidth; j++){
            fwrite(&pixel_array[i][j], sizeof(PIXEL), 1, f);
        }
        if(padding)
            fwrite(&pixel_array[i][j], padding, 1, f);
    }

    fclose(f);
    return 0;
}


QPixmap Image::getPixMap(){
    QColor colors;
    QImage *img = new QImage(bih.biWidth, bih.biHeight, QImage::Format_RGB32);

    for(int y = 0; y < bih.biHeight; y++){
        for(int x = 0; x < bih.biWidth; x++){
            colors.setRed(static_cast<int>(pixel_array[bih.biHeight - y - 1][x].r));
            colors.setGreen(static_cast<int>(pixel_array[bih.biHeight - y - 1][x].g));
            colors.setBlue(static_cast<int>(pixel_array[bih.biHeight - y - 1][x].b));
            img->setPixel(x, y, colors.rgb());
        }
    }

    QPixmap pixmap;
    pixmap.convertFromImage(*img);

    delete img;

    return pixmap;

}

int Image::getWidth(){
    return bih.biWidth;
}

int Image::getHeight(){
    return bih.biHeight;
}

void Image::setPixel(PIXEL pix, int x, int y){
    pixel_array[bih.biHeight - y - 1][x] = pix;
}

void Image::draw_circle(int width_line, bool isFill, QColor color_line, QColor color_fill, QPoint center, int radius) {
    int thickness = width_line;
    PIXEL pix;
    for(int y = 0; y < getHeight(); y ++){
        for(int x = 0; x < getWidth(); x++){
            if(((x-center.x())*(x-center.x()) + (y-center.y())*(y-center.y()) < radius*radius)&&
                    ((x-center.x())*(x-center.x()) + (y-center.y())*(y-center.y()) > (radius - thickness)*(radius - thickness))){
                pix.b = static_cast<unsigned char>(color_line.blue());
                pix.g = static_cast<unsigned char>(color_line.green());
                pix.r = static_cast<unsigned char>(color_line.red());
                setPixel(pix, x, y);
            }
            if(((x-center.x())*(x-center.x()) + (y-center.y())*(y-center.y()) < (radius - thickness+1)*(radius - thickness+1))&& isFill){
                pix.b = static_cast<unsigned char>(color_fill.blue());
                pix.g = static_cast<unsigned char>(color_fill.green());
                pix.r = static_cast<unsigned char>(color_fill.red());
                setPixel(pix, x, y);
            }

        }
    }
    emit update_image(getPixMap());
}

void Image::rectangle(QPoint *start_point, QPoint *end_point, int width_line, bool isFill,
                      QColor line_color, QColor fill_color) {
    PIXEL pix;
    int max_width_line;
    if (abs(start_point->x() - end_point->x()) > abs(start_point->y() - end_point->y()))
        max_width_line = abs(start_point->y() - end_point->y());
    else
        max_width_line = abs(start_point->x() - end_point->x());

    if (width_line > max_width_line)
        width_line = max_width_line;

    int max_x, max_y, min_x, min_y;
    if (start_point->x() > end_point->x()) {
        max_x = start_point->x();
        min_x = end_point->x();
    }
    else {
        min_x = start_point->x();
        max_x = end_point->x();
    }

    if (start_point->y() > end_point->y()) {
        max_y = start_point->y();
        min_y = end_point->y();
    }
    else {
        min_y = start_point->y();
        max_y = end_point->y();
    }

    for (int y = min_y; y < min_y + width_line; y++) {
        for (int x = min_x; x < max_x; x++) {
            pix.b = static_cast<unsigned char>(line_color.blue());
            pix.g = static_cast<unsigned char>(line_color.green());
            pix.r = static_cast<unsigned char>(line_color.red());
            setPixel(pix, x, y);
        }
    }
    for (int y = max_y - width_line + 1; y <= max_y; y++) {
        for (int x = min_x; x < max_x; x++) {
            pix.b = static_cast<unsigned char>(line_color.blue());
            pix.g = static_cast<unsigned char>(line_color.green());
            pix.r = static_cast<unsigned char>(line_color.red());
            setPixel(pix, x, y);
        }
    }
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x < min_x + width_line; x++) {
            pix.b = static_cast<unsigned char>(line_color.blue());
            pix.g = static_cast<unsigned char>(line_color.green());
            pix.r = static_cast<unsigned char>(line_color.red());
            setPixel(pix, x, y);
        }
    }
    for (int y = min_y; y <= max_y; y++) {
        for (int x = max_x - width_line; x < max_x; x++) {
            pix.b = static_cast<unsigned char>(line_color.blue());
            pix.g = static_cast<unsigned char>(line_color.green());
            pix.r = static_cast<unsigned char>(line_color.red());
            setPixel(pix, x, y);
        }
    }

    if (isFill) {
        for (int y = min_y + width_line; y <= max_y - width_line; y++) {
            for (int x = min_x + width_line; x < max_x - width_line; x++) {
                pix.b = static_cast<unsigned char>(fill_color.blue());
                pix.g = static_cast<unsigned char>(fill_color.green());
                pix.r = static_cast<unsigned char>(fill_color.red());
                setPixel(pix, x, y);
            }
        }
    }

    emit update_image(getPixMap());
}

void Image::turn(QPoint* start_point, QPoint* end_point, int angle) {
    int min_x, max_x, min_y, max_y, x0, y0, sin_a, cos_a;
    sin_a = 0;
    cos_a = 1;
    switch (angle) {
    case 90:
        cos_a = 0;
        sin_a = 1;
        break;
    case 180:
        cos_a = -1;
        sin_a = 0;
        break;
    case 270:
        cos_a = 0;
        sin_a = -1;
        break;
    }
    if (start_point->x() < end_point->x()) {
        min_x = start_point->x();
        max_x = end_point->x();
    }
    else {
        min_x = end_point->x();
        max_x = start_point->x();
    }
    if (start_point->y() < end_point->y()) {
        min_y = start_point->y();
        max_y = end_point->y();
    }
    else {
        min_y = end_point->y();
        max_y = start_point->y();
    }
    x0 = min_x + (max_x - min_x) / 2;
    y0 = min_y + (max_y - min_y) / 2;

    size_t padding = 0;
    if ((bih.biWidth * 3) % 4)
        padding = 4 - (bih.biWidth * 3) % 4;
    PIXEL** tmp_rgb = new PIXEL*[static_cast<unsigned long long>(bih.biHeight)];
    for (int i = 0; i < static_cast<int>(bih.biHeight); i++) {
        tmp_rgb[i] = new PIXEL [static_cast<unsigned long long>(bih.biWidth) + padding];
    }
    int max_x_tmp = 0;
    int min_x_tmp = static_cast<int>(bih.biWidth) - 1;
    int max_y_tmp = 0;
    int min_y_tmp = static_cast<int>(bih.biHeight) - 1;
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            PIXEL tmp;
            tmp.r = pixel_array[static_cast<int>(bih.biHeight) - y - 1][x].r;
            tmp.g = pixel_array[static_cast<int>(bih.biHeight) - y - 1][x].g;
            tmp.b = pixel_array[static_cast<int>(bih.biHeight) - y - 1][x].b;
            if (angle != 180) {
                pixel_array[static_cast<int>(bih.biHeight) - y - 1][x].r = 255;
                pixel_array[static_cast<int>(bih.biHeight) - y - 1][x].g = 255;
                pixel_array[static_cast<int>(bih.biHeight) - y - 1][x].b = 255;
            }

            int y_tmp = static_cast<int>(bih.biHeight) - (y0 + (y - y0) * cos_a + (x - x0) * sin_a) - 1;
            if ((max_y - min_y)%2 != 0)
                y_tmp -= 1;
            int x_tmp = x0 + (x - x0) * cos_a - (y - y0) * sin_a;
            if ((max_x - min_x)%2 !=0)
                x_tmp += 1;
            if (y_tmp >= 0 && y_tmp < static_cast<int>(bih.biHeight) && x_tmp >= 0 && x_tmp < static_cast<int>(bih.biWidth)) {
                tmp_rgb[y_tmp][x_tmp].r = tmp.r;
                tmp_rgb[y_tmp][x_tmp].g = tmp.g;
                tmp_rgb[y_tmp][x_tmp].b = tmp.b;
                if (y_tmp < min_y_tmp)
                    min_y_tmp = y_tmp;
                if (y_tmp > max_y_tmp)
                    max_y_tmp = y_tmp;
                if (x_tmp < min_x_tmp)
                    min_x_tmp = x_tmp;
                if (x_tmp > max_x_tmp)
                    max_x_tmp = x_tmp;
            }
        }
    }

    for (int y = min_y_tmp; y <= max_y_tmp; y++) {
        for (int x = min_x_tmp; x <= max_x_tmp; x++) {
            if (x >= 0 && x < static_cast<int>(bih.biWidth) && y >= 0 && y < static_cast<int>(bih.biHeight)) {
                pixel_array[y][x].r = tmp_rgb[y][x].r;
                pixel_array[y][x].g = tmp_rgb[y][x].g;
                pixel_array[y][x].b = tmp_rgb[y][x].b;
            }
        }
    }

    for (int i = 0; i < static_cast<int>(bih.biHeight) - 1; i++) {
        delete tmp_rgb[i];
    }
    delete[] tmp_rgb;

    emit update_image(this->getPixMap());
}

void Image::drawFirstVariantOfFrame(QColor color, int frameWidth){
    this->drawSquare(0, 0, static_cast<int>(bih.biWidth) - 1, frameWidth/4, color, 1, 1);

    for(int x = 0; x < (static_cast<int>(bih.biWidth)- 1); x += frameWidth){
        this->drawSquare(x, 0, x + frameWidth/4, frameWidth, color, 1, 1);
        this->drawSquare(x, frameWidth * 3 / 4, x + frameWidth * 3 / 4, frameWidth, color, 1, 1);
    }

    this->drawSquare(0, 0, frameWidth, frameWidth, color, 1, 1);

    this->drawSquare(0, static_cast<int>(bih.biHeight) - frameWidth/4 - 1,
                     static_cast<int>(bih.biWidth) - 1, static_cast<int>(bih.biHeight) - 1, color, 1, 1);

    for(int x = static_cast<int>(bih.biWidth) - 1; x > 0; x -= frameWidth){
        this->drawSquare(x - frameWidth/4, static_cast<int>(bih.biHeight) - frameWidth - 1,
                         x, static_cast<int>(bih.biHeight) - 1, color, 1, 1);
        this->drawSquare(x - frameWidth * 3 / 4, static_cast<int>(bih.biHeight) - frameWidth - 1,
                         x, static_cast<int>(bih.biHeight) - frameWidth * 3 / 4 - 1, color, 1, 1);
    }

    this->drawSquare(static_cast<int>(bih.biWidth) - 1 - frameWidth, 0,
                     static_cast<int>(bih.biWidth) - 1, frameWidth, color, 1, 1);

    this->drawSquare(0, 0, frameWidth / 4, static_cast<int>(bih.biHeight) - 1, color, 1, 1);

    for(int y = 0; y < (static_cast<int>(bih.biHeight)- 1); y += frameWidth){
        this->drawSquare(static_cast<int>(bih.biWidth)- 1 - frameWidth, y,
                         static_cast<int>(bih.biWidth)- 1, y + frameWidth / 4, color, 1, 1);
        this->drawSquare(static_cast<int>(bih.biWidth)- 1 - frameWidth, y,
                         static_cast<int>(bih.biWidth)- 1 - frameWidth * 3 / 4, y + frameWidth * 3 / 4, color, 1, 1);
    }

    this->drawSquare(static_cast<int>(bih.biWidth) - 1 - frameWidth, static_cast<int>(bih.biHeight) - 1 - frameWidth,
                     static_cast<int>(bih.biWidth) - 1, static_cast<int>(bih.biHeight) - 1, color, 1, 1);

    this->drawSquare(static_cast<int>(bih.biWidth) - frameWidth/4 - 1, 0,
                     static_cast<int>(bih.biWidth) - 1, static_cast<int>(bih.biHeight) - 1, color, 1, 1);

    for(int y = static_cast<int>(bih.biHeight) - 1; y > 0; y -= frameWidth){
        this->drawSquare(0, y - frameWidth/4,
                         frameWidth, y, color, 1, 1);
        this->drawSquare(frameWidth * 3 / 4, y - frameWidth * 3 / 4,
                         frameWidth, y, color, 1, 1);
    }

    this->drawSquare(0, static_cast<int>(bih.biHeight) - 1 - frameWidth,
                     frameWidth, static_cast<int>(bih.biHeight) - 1, color, 1, 1);
}

void Image::drawSecondVariantOfFrame(QColor color, int frameWidth){
    int lineWidth = 1;

    if(frameWidth / 20 > 1){
        lineWidth = frameWidth / 20;
    }

    this->drawSquare(0, 0, static_cast<int>(bih.biWidth) - 1, frameWidth/4, color, 1, 1);
    this->drawSquare(0, static_cast<int>(bih.biHeight) - frameWidth/4 - 1,
                     static_cast<int>(bih.biWidth) - 1, static_cast<int>(bih.biHeight) - 1, color, 1, 1);
    this->drawSquare(0, 0, frameWidth / 4, static_cast<int>(bih.biHeight) - 1, color, 1, 1);
    this->drawSquare(static_cast<int>(bih.biWidth) - frameWidth/4 - 1, 0,
                     static_cast<int>(bih.biWidth) - 1, static_cast<int>(bih.biHeight) - 1, color, 1, 1);
    this->drawSquare(0, 0, static_cast<int>(bih.biWidth) - 1, static_cast<int>(bih.biHeight) - 1,
                     QColor::fromRgb(0, 0, 0, 255), lineWidth, 0);
    this->drawSquare(frameWidth / 4, frameWidth / 4,
                     static_cast<int>(bih.biWidth) - 1 - frameWidth / 4,static_cast<int>(bih.biHeight) - 1 - frameWidth/4,
                     QColor::fromRgb(0, 0, 0, 255), lineWidth, 0);

    for(int x = 0, rectSize = frameWidth; x < (static_cast<int>(bih.biWidth)- 1) / 2 && rectSize > frameWidth / 4 + 3; x += frameWidth){
        this->drawSquare(x, 0, x + frameWidth, rectSize, color, 1, 1);
        this->drawSquare(x, 0, x + frameWidth, rectSize,
                         QColor::fromRgb(0, 0, 0, 255), lineWidth, 0);

        this->drawSquare(static_cast<int>(bih.biWidth)- 1 - frameWidth - x, 0,
                         static_cast<int>(bih.biWidth)- 1 - x, rectSize, color, 1, 1);
        this->drawSquare(static_cast<int>(bih.biWidth)- 1 - frameWidth - x, 0,
                         static_cast<int>(bih.biWidth)- 1 - x, rectSize,
                         QColor::fromRgb(0, 0, 0, 255), lineWidth, 0);

        this->drawSquare(x, static_cast<int>(bih.biHeight)- 1 - rectSize,
                         x + frameWidth, static_cast<int>(bih.biHeight)- 1, color, 1, 1);
        this->drawSquare(x, static_cast<int>(bih.biHeight)- 1 - rectSize,
                         x + frameWidth, static_cast<int>(bih.biHeight)- 1,
                         QColor::fromRgb(0, 0, 0, 255), lineWidth, 0);

        this->drawSquare(static_cast<int>(bih.biWidth)- 1 - frameWidth - x, static_cast<int>(bih.biHeight)- 1 - rectSize,
                         static_cast<int>(bih.biWidth)- 1 - x, static_cast<int>(bih.biHeight)- 1, color, 1, 1);
        this->drawSquare(static_cast<int>(bih.biWidth)- 1 - frameWidth - x, static_cast<int>(bih.biHeight)- 1 - rectSize,
                         static_cast<int>(bih.biWidth)- 1 - x, static_cast<int>(bih.biHeight)- 1,
                         QColor::fromRgb(0, 0, 0, 255), lineWidth, 0);

        rectSize -= rectSize / 4;
    }

    for(int y = 0, rectSize = frameWidth; y < (static_cast<int>(bih.biHeight)- 1) / 2 && rectSize > frameWidth / 4 + 3; y += frameWidth){
        this->drawSquare(0, y, rectSize, y + frameWidth, color, 1, 1);
        this->drawSquare(0, y, rectSize, y + frameWidth,
                         QColor::fromRgb(0, 0, 0, 255), lineWidth, 0);

        this->drawSquare(0, static_cast<int>(bih.biHeight)- 1 - frameWidth - y,
                         rectSize, static_cast<int>(bih.biHeight)- 1 - y, color, 1, 1);
        this->drawSquare(0, static_cast<int>(bih.biHeight)- 1 - frameWidth - y,
                         rectSize, static_cast<int>(bih.biHeight)- 1 - y,
                         QColor::fromRgb(0, 0, 0, 255), lineWidth, 0);

        this->drawSquare(static_cast<int>(bih.biWidth)- 1 - rectSize, y,
                         static_cast<int>(bih.biWidth)- 1 , y + frameWidth, color, 1, 1);
        this->drawSquare(static_cast<int>(bih.biWidth)- 1 - rectSize, y,
                         static_cast<int>(bih.biWidth)- 1 , y + frameWidth,
                         QColor::fromRgb(0, 0, 0, 255), lineWidth, 0);

        this->drawSquare(static_cast<int>(bih.biWidth)- 1 - rectSize, static_cast<int>(bih.biHeight)- 1 - frameWidth - y,
                         static_cast<int>(bih.biWidth)- 1, static_cast<int>(bih.biHeight)- 1 - y, color, 1, 1);
        this->drawSquare(static_cast<int>(bih.biWidth)- 1 - rectSize, static_cast<int>(bih.biHeight)- 1 - frameWidth - y,
                         static_cast<int>(bih.biWidth)- 1, static_cast<int>(bih.biHeight)- 1 - y,
                         QColor::fromRgb(0, 0, 0, 255), lineWidth, 0);

        rectSize -= rectSize / 4;
    }
}

void Image::drawThirdVariantOfFrame(QColor color, int frameWidth){
    int xStep = static_cast<int>(bih.biWidth) / 100;
    int yStep = static_cast<int>(bih.biHeight) / 100;
    int lineWidth;
    int xFrameWidth = frameWidth;
    int yFrameWidth = frameWidth;

    if(yFrameWidth > static_cast<int>(bih.biHeight) / 2 || xFrameWidth > static_cast<int>(bih.biWidth) / 2){
        yFrameWidth = static_cast<int>(bih.biHeight) / 2;
        xFrameWidth = static_cast<int>(bih.biHeight) / 2;
    }

    if(bih.biWidth > bih.biHeight){
        lineWidth = bih.biHeight / 50;
    }
    else{
        lineWidth = bih.biWidth / 50;
    }

    if(lineWidth < 1){
        lineWidth = 1;
    }

    drawSquare(0, 0, static_cast<int>(bih.biWidth)- 1, static_cast<int>(bih.biHeight)- 1, color, lineWidth, 0);//check!!!

    for(int x = 0, y = 0; x < xFrameWidth && y < yFrameWidth;){
        drawLine(0, yFrameWidth - y, x, 0, color, lineWidth);

        drawLine(static_cast<int>(bih.biWidth)- 1, yFrameWidth - y,
                 static_cast<int>(bih.biWidth)- 1 - x, 0, color, lineWidth);

        drawLine(0, static_cast<int>(bih.biHeight) - yFrameWidth + y, x, static_cast<int>(bih.biHeight)- 1, color, lineWidth);

        drawLine(static_cast<int>(bih.biWidth)- 1, static_cast<int>(bih.biHeight) - yFrameWidth + y,
                 static_cast<int>(bih.biWidth)- 1 - x, static_cast<int>(bih.biHeight)- 1, color, lineWidth);

        x += xStep;
        y += yStep;
    }
}

void Image::drawSquare(int x1, int y1, int x2, int y2, QColor color, int width_line, int filled){
    QPoint *start;
    start = new QPoint;
    QPoint *end;
    end = new QPoint;
    start->setX(x1);
    start->setY(y1);
    end->setX(x2);
    end->setY(y2);
    bool isFill;
    if(filled)
        isFill = true;
    else
        isFill = false;
    rectangle(start, end, width_line, isFill, color, color);
}

void Image::drawLine(int x1, int x2, int y1, int y2, QColor color, int width_line) {
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;

    int error = deltaX - deltaY;

    drawBigPixel(x2, y2, width_line, color);
    while(x1 != x2 || y1 != y2)
    {
        drawBigPixel(x1, y1, width_line, color);
        const int error2 = error * 2;

        if(error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
}

void Image::drawBigPixel(int x0, int y0, int size, QColor color){
    PIXEL pix;
    pix.b = static_cast<unsigned char>(color.blue());
    pix.g = static_cast<unsigned char>(color.green());
    pix.r = static_cast<unsigned char>(color.red());
    int x1;
    int x2;
    int y1;
    int y2;

    if(size % 2 == 0){
        x1 = x0 - abs(size)/2 + 1;
        y1 = y0 - abs(size)/2 + 1;
    }
    else{
        x1 = x0 - abs(size)/2;
        y1 = y0 - abs(size)/2;
    }

    x2 = x0 + abs(size)/2;
    y2 = y0 + abs(size)/2;

    for(int y = y1; y <= y2; y++){
        for(int x = x1; x <= x2; x++){
            if(x >= 0 && y >= 0 && x < static_cast<int>(bih.biWidth) && y < static_cast<int>(bih.biHeight)){
                this->setPixel(pix, x, y);
            }
        }
    }
}
