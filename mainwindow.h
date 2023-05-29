#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "flashcardview.h"
#include "flashcardedit.h"
#include "flashcardeditwindow.h"
#include "flashcardaddnewwindow.h"
#include "flashcardmanager.h"
#include "homewidget.h"
#include "flashcardviewwidget.h"

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

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    //void wheelEvent(QWheelEvent* event) override;

private:
    void setupMenu();
    void restorePreviousSettings();
    void setupToolBar();

private:
    HomeWidget *homeWidget;
    FlashCardView *flashCardView;
    FlashCardAddNewWindow *flashCardAddNewWindow;
    FlashCardEditWindow *flashCardEditWindow;
    FlashCardViewWidget *flashCardViewWidget;
    QString flashCardPath;
    QPushButton *nextButton;
    QPushButton *previousButton;
    QString rootFolder;
    FlashcardManager *flashcardManager;
    QToolBar *toolbar;
    QStackedWidget *stackedWidget;
    QString databaseName;
};

#endif // MAINWINDOW_H
