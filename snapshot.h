#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <QtWidgets>

class SnapShotWidget : public QWidget {
    Q_OBJECT
public:
    SnapShotWidget(QWidget *parent = nullptr);
    ~SnapShotWidget() { qDebug() << "destructor"; }
    QPixmap getPixmap() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void resetSnapshot();
    void crop();
    void takeNewSnapShot();

signals:
    void accepted(SnapShotWidget *);
    void rejected(SnapShotWidget *);

private:
    QPoint startPoint;
    QPoint endPoint;
    bool isDrawing;
    QRubberBand* rubberBand;
    QScreen *screen;
    QPixmap originalScreenShot;
    QPixmap screenShot;
    QPixmap latestScreenShot;
    QVBoxLayout* mainLayout;
    QPushButton *cropButton;
    QPushButton *resetButton;
    QPushButton *newSnapShot;
    QLabel *imageLabel;
    QHBoxLayout *buttonsLayout;
    QComboBox *screenComboBox;
    QDialogButtonBox *buttonBox;
};

#endif // SNAPSHOT_H
