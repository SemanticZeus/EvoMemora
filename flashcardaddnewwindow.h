#ifndef FLASHCARDADDNEWWINDOW_H
#define FLASHCARDADDNEWWINDOW_H

#include <QWidget>
#include "flashcardeditwidget.h"
#include "flashcardmanager.h"

class FlashCardAddNewWindow : public QWidget
{
    Q_OBJECT
public:
    FlashCardAddNewWindow(FlashcardManager *flashcardManager, QWidget *parent = nullptr);
    void setRoot(QString root);
    QString getRoot();
    QToolBar *getToolBar() { return mainToolBar; }

signals:
    void homeActionTriggered();

protected:
    void setupToolBar();

private:
    FlashCardEditWidget *flashCardEditWidget;
    QToolBar *mainToolBar;
    QString root;
    QAction *homeAction;
    QAction *addAction;
    FlashcardManager *flashcardManager;
};

#endif // FLASHCARDADDNEWWINDOW_H
