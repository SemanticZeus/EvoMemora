#include "snapshot.h"
#include <QApplication>
#include <QSpacerItem>

SnapShotWidget::SnapShotWidget(QWidget *parent) : QWidget(parent), isDrawing(false), rubberBand(nullptr) {
    resetButton = new QPushButton("Reset", this);
    connect(resetButton, &QPushButton::released, this, &SnapShotWidget::resetSnapshot);
    cropButton = new QPushButton("Crop Image", this);
    connect(cropButton, &QPushButton::released, this, &SnapShotWidget::crop);
    mainLayout = new QVBoxLayout(this);
    this->setVisible(false);
    screen = QGuiApplication::primaryScreen();
    originalScreenShot = screen->grabWindow(0);
    imageLabel = new QLabel(this);
    originalScreenShot.save("tmp.jpeg");
    originalScreenShot.load("tmp.jpeg");
    imageLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(resetButton);
    buttonsLayout->addWidget(cropButton);
    newSnapShot = new QPushButton("Take a new SnapShot", this);
    screenComboBox = new QComboBox(this);
    for (int i=0;i<QGuiApplication::screens().length();i++) {
        QString item = QString("Screen: %1").arg(i);
        screenComboBox->addItem(item);
    }
    connect(newSnapShot, &QPushButton::released, this, &SnapShotWidget::takeNewSnapShot);
    connect(newSnapShot, &QPushButton::released, this, &SnapShotWidget::updateGeometry);
    buttonsLayout->addWidget(newSnapShot);
    buttonsLayout->addWidget(screenComboBox);
    buttonsLayout->addStretch();
    mainLayout->addItem(buttonsLayout);
    mainLayout->addWidget(imageLabel);
    this->setVisible(true);
    this->setLayout(mainLayout);
    latestScreenShot = originalScreenShot;
    screenShot = originalScreenShot.scaled(QSize(500, 500), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(screenShot);
    imageLabel->setMinimumSize(500,400);

    buttonBox = new QDialogButtonBox(
                QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, [=]() {
        this->close();
        emit accepted(this);
    });
    connect(buttonBox, &QDialogButtonBox::rejected, [=]() {
        this->close();
        emit rejected(this);
    });
}

void SnapShotWidget::takeNewSnapShot()
{
    int i = screenComboBox->currentIndex();
    this->setVisible(false);
    QThread::msleep(500);
    screen = QGuiApplication::screens().at(i);
    originalScreenShot = screen->grabWindow(0);
    originalScreenShot.save("tmp.jpeg");
    originalScreenShot.load("tmp.jpeg");
    latestScreenShot = originalScreenShot;
    screenShot = latestScreenShot.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    qDebug() << "imageLabel size = " << imageLabel->size() << ", originalScreenShot size = " << originalScreenShot.size();
    imageLabel->setPixmap(screenShot);
    this->setVisible(true);
}

void SnapShotWidget::crop()
{
    if (rubberBand==nullptr) return;
    float scale = (float)latestScreenShot.width()/screenShot.width();
    float sh = (float)latestScreenShot.height()/screenShot.height();
    if (scale > sh) scale = sh;
    int y_start = imageLabel->mapToParent(QPoint(0,0)).y();
    int x_start = imageLabel->mapToParent(QPoint(0,0)).x();
    if (imageLabel->height()>screenShot.height()) {
        y_start += (imageLabel->height()-screenShot.height())/2;
    }
    qDebug() << y_start << rubberBand->geometry();
    QRect geometry = rubberBand->geometry();
    int new_x = (geometry.x()-x_start)*scale;
    int new_y = (geometry.y()-y_start)*scale;
    int w = geometry.width()*scale;
    int h = geometry.height()*scale;
    geometry.setRect(new_x, new_y, w, h);
    qDebug() << geometry;

    latestScreenShot = latestScreenShot.copy(geometry);
    screenShot = latestScreenShot.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(screenShot);
    isDrawing = false;
    rubberBand->hide();
    delete rubberBand;
    rubberBand = nullptr;
}

void SnapShotWidget::resetSnapshot()
{
    latestScreenShot = originalScreenShot;
    screenShot = originalScreenShot.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(screenShot);
    isDrawing = false;
    if (rubberBand) {
        rubberBand->hide();
        delete rubberBand;
        rubberBand = nullptr;
    }
}

void SnapShotWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
}

void SnapShotWidget::resizeEvent(QResizeEvent *event) {
    // Call the base class implementation
    QWidget::resizeEvent(event);
    QSize size = imageLabel->size();
    screenShot = latestScreenShot.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    qDebug() << "screenShot.size = " << screenShot.size() << "latestScreenShot.size = " << latestScreenShot.size();
    qDebug() << this->size() << "imageLable->size() = " << imageLabel->size();
    imageLabel->setPixmap(screenShot);
    imageLabel->updateGeometry();
}

void SnapShotWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (isDrawing) {
            endPoint = event->pos();
            isDrawing = false;
            rubberBand->hide();
            delete rubberBand;
            rubberBand = nullptr;
            update();
        } else {
            startPoint = event->pos();
            isDrawing = true;
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
            rubberBand->setGeometry(QRect(startPoint, QSize()));
            rubberBand->show();
        }
    }
}

void SnapShotWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isDrawing) {
        endPoint = event->pos();
        rubberBand->setGeometry(QRect(startPoint, endPoint).normalized());
        update();
    }
}

QPixmap SnapShotWidget::getPixmap() const
{
    return latestScreenShot;
}
