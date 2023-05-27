#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QtWidgets>

class ImageWidget : public QLabel
{
    Q_OBJECT
public:
    ImageWidget(QPixmap p, int width, int height);
    void scale(float s);
    QSize getSize() { return pixmap.size(); }
    QPixmap getPixmap() { return originalPixmap; }

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QPixmap originalPixmap;
    QPixmap pixmap;
    qreal totalScale = 1;
    qreal initialWidth, initialHeight;
};

#endif // IMAGEWIDGET_H
