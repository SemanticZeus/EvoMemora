#include "flashcardaddnewwindow.h"
#include "flashcardeditwindow.h"

FlashCardAddNewWindow::FlashCardAddNewWindow(FlashcardManager *flashcardManager, QWidget *parent) : QWidget(parent)
{
    this->flashcardManager = flashcardManager;
    flashCardEditWidget = new FlashCardEditWidget;
    setupToolBar();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(mainToolBar);
    mainLayout->addWidget(flashCardEditWidget);
    this->setLayout(mainLayout);
}

void FlashCardAddNewWindow::setRoot(QString root)
{
    this->root = root;
    flashCardEditWidget->setRoot(root);
}

QString FlashCardAddNewWindow::getRoot()
{
    return root;
}

void FlashCardAddNewWindow::setupToolBar()
{
    mainToolBar = new QToolBar;
    //QWidget* spacer = new QWidget();
    //spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //saveToolBar->addWidget(spacer);
    mainToolBar->setObjectName("FlashCardAddNewToolBar");

     addAction = new QAction("Add FlashCard", this);
     connect(addAction, &QAction::triggered, [&]() {
         flashCardEditWidget->save();
         //names.append(flashCardEditWidget->getName());
         flashcardManager->addFlashCard(flashCardEditWidget->getName());
         flashCardEditWidget->clear();
         flashCardEditWidget->setRoot(root);
     });
     homeAction = new QAction("Home", this);
     connect(homeAction, &QAction::triggered, [&]() {
         flashCardEditWidget->clear();
         emit homeActionTriggered();
     });
     mainToolBar->addAction(homeAction);
     mainToolBar->addAction(addAction);
}
