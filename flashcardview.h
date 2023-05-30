#ifndef FLASHCARDVIEW_H
#define FLASHCARDVIEW_H

#include <QWidget>
#include <QtWidgets>
#include "imagewidget.h"
#include "textwidget.h"
#include "flashcard.h"

class FlashCardView : public QTabWidget
{
    Q_OBJECT
public:
    explicit FlashCardView(int width = 800, int height = 900, QWidget *parent = nullptr);
    bool loadFlashCard(QString root, QString filename);
    FlashCard getFlashCard();
    void scale(qreal k);
    QString getRoot() { return flashcard.getRoot(); }
    QString getName() { return flashcard.getName(); }
    void reset();

public slots:
    void reloadFlashCard();

protected:
    void loadFaceWidget(int face);

private:
    QWidget *frontWidget;
    QWidget *backWidget;
    QList<ImageWidget *> imageList;
    QList<TextWidget *> textList;
    qreal totalScale = 1;
    FlashCard flashcard;
    //double margin = 1.05;
};

#endif // FLASHCARDVIEW_H
