#include "imagewidget.h"

ImageWidget::ImageWidget(QPixmap p, int width, int height) {
    originalPixmap = p;
    if (width > 0 && height > 0)
        pixmap = originalPixmap.scaled(width, height, Qt::KeepAspectRatio);
    else if (width > 0)
        pixmap = originalPixmap.scaledToWidth(width);
    setPixmap(pixmap);
    height = pixmap.height();
    initialWidth = width;
    initialHeight = height;
    setAlignment(Qt::AlignCenter);
    //setFixedSize(width, height);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ImageWidget::resizeEvent(QResizeEvent* event)
{
    pixmap = originalPixmap.scaled(event->size().width(), event->size().height()
                                          ,Qt::KeepAspectRatio);
    setPixmap(pixmap);
}

void ImageWidget::scale(float s)
{
    totalScale*=s;
    int w = initialWidth*totalScale;
    int h = initialHeight*totalScale;
    //pixmap = originalPixmap.scaled(w, h, Qt::KeepAspectRatio);
    setFixedSize(w,h);
}
