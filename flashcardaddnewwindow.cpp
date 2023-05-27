#include "flashcardaddnewwindow.h"


#include "flashcardeditwindow.h"

FlashCardAddNewWindow::FlashCardAddNewWindow(QWidget *parent) : QWidget(parent)
{
    flashCardEditWidget = new FlashCardEditWidget;
    setupToolBar();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mainToolBar);
    mainLayout->addWidget(flashCardEditWidget);
    this->setLayout(mainLayout);
}

void FlashCardAddNewWindow::setRoot(QString root)
{
    this->root = root;
    flashCardEditWidget->setRoot(root);
}

void FlashCardAddNewWindow::closeEvent(QCloseEvent *event)
{
    emit newFlashcardsAdded();
}

QString FlashCardAddNewWindow::getRoot()
{
    return root;
}

QVector<QString> &FlashCardAddNewWindow::getNames()
{
    return names;
}

void FlashCardAddNewWindow::setupToolBar()
{
    mainToolBar = new QToolBar;
    //QWidget* spacer = new QWidget();
    //spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //saveToolBar->addWidget(spacer);
    mainToolBar->setObjectName("FlashCardAddNewToolBar");

     QAction *addAction = new QAction("Add FlashCard", this);
     connect(addAction, &QAction::triggered, [&]() {
         flashCardEditWidget->save();
         names.append(flashCardEditWidget->getName());
         flashCardEditWidget->clear();
         flashCardEditWidget->setRoot(root);
     });
     mainToolBar->addAction(addAction);

     QAction *closeAction = new QAction("Close", this);
     connect(closeAction, &QAction::triggered, [&]() {
         flashCardEditWidget->clear();
         this->close();
         emit newFlashcardsAdded();
     });
     mainToolBar->addAction(closeAction);
}
