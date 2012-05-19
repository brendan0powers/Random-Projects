
#include "QOpenCVWidget.h"

// Constructor
QOpenCVWidget::QOpenCVWidget(QWidget *parent) : QWidget(parent) {
    layout = new QVBoxLayout;
    imagelabel = new QLabel;
    QImage dummy(100,100,QImage::Format_RGB32);
    image = dummy;
    layout->addWidget(imagelabel);
    for (int x = 0; x < 100; x ++) {
        for (int y =0; y < 100; y++) {
            image.setPixel(x,y,qRgb(x, y, y));
        }
    }
    imagelabel->setPixmap(QPixmap::fromImage(image));

    setLayout(layout);
}

QOpenCVWidget::~QOpenCVWidget(void) {
    
}

void QOpenCVWidget::putImage(IplImage *cvimage) {

    imagelabel->setPixmap(QPixmap::fromImage(image));    
}

