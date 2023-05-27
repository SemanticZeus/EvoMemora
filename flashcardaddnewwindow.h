#ifndef FLASHCARDADDNEWWINDOW_H
#define FLASHCARDADDNEWWINDOW_H

#include <QWidget>
#include "flashcardeditwidget.h"

class FlashCardAddNewWindow : public QWidget
{
    Q_OBJECT
public:
    FlashCardAddNewWindow(QWidget *parent = nullptr);
    void setRoot(QString root);
    QString getRoot();
    QVector<QString>& getNames();
    void clearNames() { names.clear(); }

signals:
    void newFlashcardsAdded();

protected:
    void setupToolBar();
    void closeEvent(QCloseEvent *event);

private:
    FlashCardEditWidget *flashCardEditWidget;
    QToolBar *mainToolBar;
    QString root;
    QVector<QString> names;
};

#endif // FLASHCARDADDNEWWINDOW_H
