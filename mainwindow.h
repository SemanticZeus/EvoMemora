#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "flashcardview.h"
#include "flashcardeditwindow.h"
#include "flashcardaddnewwindow.h"
#include "flashcardmanager.h"
#include "homewidget.h"
#include "flashcardviewwidget.h"

class BlurOverlay : public QWidget {
public:
    BlurOverlay(QWidget* parent = nullptr) : QWidget(parent) {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setWindowFlags(Qt::WindowStaysOnTopHint);
        setStyleSheet("background-color: rgba(255, 255, 255, 127);"); // Semi-transparent white overlay

        // Apply a blur effect to the parent (optional, only visual)
        if (parent) {
            blurEffect = new QGraphicsBlurEffect(this);
            blurEffect->setBlurRadius(10);
            parent->setGraphicsEffect(blurEffect);
        }
    }

    ~BlurOverlay(){
        if (parentWidget()) parentWidget()->setGraphicsEffect(nullptr);
    }

protected:
    void resizeEvent(QResizeEvent* event) override {
        setGeometry(parentWidget()->geometry());
    }

private:
    QGraphicsBlurEffect* blurEffect;
};

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
    FlashCardEditWidget *flashCardEditWidget;
    FlashCardViewWidget *flashCardViewWidget;
    QString flashCardPath;
    QPushButton *nextButton;
    QPushButton *previousButton;
    QString rootFolder;
    FlashcardManager *flashcardManager;
    QToolBar *toolbar;
    QStackedWidget *stackedWidget;
    QString databaseName;
    BlurOverlay* overlay;
};

#endif // MAINWINDOW_H
