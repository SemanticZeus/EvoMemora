#ifndef FLASHCARDEDITWINDOW_H
#define FLASHCARDEDITWINDOW_H

#include <QtWidgets>
#include "flashcardeditwidget.h"

class FlashCardEditWindow : public QWidget
{
Q_OBJECT
public:
    FlashCardEditWindow(QWidget *parent = nullptr);
    void loadFlashCard(const FlashCard &flashcard);

signals:
    void updateView();

protected:
    void setupToolBar();

private:
    FlashCardEditWidget *flashCardEditWidget;
    QToolBar *saveToolBar;
};

#endif // FLASHCARDEDITWINDOW_H
