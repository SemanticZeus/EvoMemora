#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "flashcardview.h"
#include "flashcardedit.h"
#include "flashcardeditwindow.h"
#include "flashcardaddnewwindow.h"
#include "leitner.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void flashCardPathUpdated();

private slots:
    void openAction();
    void updateFlashCardsList();
protected:
    virtual void closeEvent(QCloseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void setupMenu();
    void restorePreviousSettings();
    void setupToolBar();

private:
    FlashCardView *flashCardView;
    FlashCardAddNewWindow *flashCardAddNewWindow;
    FlashCardEditWindow *flashCardEditWindow;
    QString flashCardPath;
    QPushButton *nextButton;
    QPushButton *previousButton;
    QString rootFolder;
    Leitner *leitner;
};

#endif // MAINWINDOW_H
