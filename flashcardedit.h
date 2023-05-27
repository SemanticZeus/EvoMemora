#ifndef FLASHCARDEDIT_H
#define FLASHCARDEDIT_H

#include <QMainWindow>
#include <QtWidgets>
#include "snapshot.h"
#include "textwidget.h"
#include "imagewidget.h"
#include "flashcard.h"

class FlashCardEdit : public QTabWidget {
    Q_OBJECT
public:
    FlashCardEdit(QWidget *parent = nullptr);
    void scale(qreal k);
    QSize getFlashCardSize();
    void changeSize(QSize d);
    void resizeTo(int width, int height);
    bool save();
    bool loadFlashCard(QString root, QString name);
    bool loadFlashCard(FlashCard& flashcard);
    int getCurrentRow() { return currentRow; }
    void updateFlashCard();
    QString getRoot() { return flashcard.getRoot(); }
    void setRoot(QString r) { flashcard.setRootFolder(r); }
    QString getName() { return flashcard.getName(); }

protected:
    QPixmap generateThumbnail(QWidget *widget, const QSize& size);
    void loadFaceWidget(int face);

public slots:
    void newFlashCard();
    void addNewRow();
    void removeRow(int index);
    void removeCurrentRow() { removeRow(currentRow); }
    void takeSnapShot();
    void addText();
    void handleWidgetClick(QWidget* sender);

private:
    FlashCard flashcard;
    SnapShotWidget *snapShotWidget;
    int screenIndex = 0;
    QSplitter *frontWidget;
    QSplitter *backWidget;
    int currentRow;
    QFont frontFont, BackFont;
    QSize widgetSize;
    qreal totalScaleFactor = 1;
    QString folder;
    QPoint mouseLocation;
    QString id;
    QList<TextWidget*> textList;
    QList<ImageWidget*> imageList;
};

class RowWidget : public QSplitter
{
    Q_OBJECT
public:
    RowWidget(QWidget* parent = nullptr): QSplitter(Qt::Horizontal, parent) {
        QLabel* label = new QLabel("Empty Row");
        addWidget(label);
        setChildrenCollapsible(false);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QFont font;
        font.setPixelSize(17);
        // Set the font for the label
        label->setFont(font);
    }

    void addNextWidget(QWidget* w) {
        if (isEmpty) {
            isEmpty = false;
            widget(0)->hide();
        }
        addWidget(w);
    }

signals:
    void widgetClicked(QWidget* sender);

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);

        if (isActive) {
            painter.setPen(QPen(Qt::red, 2));
            painter.drawRect(rect().adjusted(0, 0, -1, -1));
            //painter.fillRect(rect(), Qt::blue); // Draw a blue background when active
        } else {
            painter.setPen(QPen(Qt::black, 2));
            painter.drawRect(rect().adjusted(0, 0, -1, -1));
            //painter.fillRect(rect(), Qt::white); // Draw a white background when inactive
        }

        QSplitter::paintEvent(event);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            emit widgetClicked(this);
        }
        QWidget::mousePressEvent(event);
    }
public slots:
    void childWidgetClicked()
    {
        emit widgetClicked(this);
    }

public:
    QSize sizeHint() const override {
        return QSize(200, 150); // Set the minimum size hint to (200, 150)
    }

    void setActive(bool a) {
        isActive = a;
        update();
    }
    bool active() {
        return isActive;
    }

private:
    bool isActive = false;
    bool isEmpty = true;
};

#endif // FLASHCARDEDIT_H
