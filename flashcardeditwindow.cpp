#include "flashcardeditwindow.h"

FlashCardEditWindow::FlashCardEditWindow(QWidget *parent) : QWidget(parent)
{
    flashCardEditWidget = new FlashCardEditWidget;
    setupToolBar();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(saveToolBar);
    mainLayout->addWidget(flashCardEditWidget);
    this->setLayout(mainLayout);
}

void FlashCardEditWindow::setupToolBar()
{
    saveToolBar = new QToolBar;
    //QWidget* spacer = new QWidget();
    //spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //saveToolBar->addWidget(spacer);
    saveToolBar->setObjectName("FlashCardEditWidgetSaveToolBar");

     QAction *saveAction = new QAction("Save FlashCard", this);
     connect(saveAction, &QAction::triggered, [&]() {
         flashCardEditWidget->save();
         emit updateView();
         this->close();
     });
     saveToolBar->addAction(saveAction);

     QAction *cancelAction = new QAction("Cancel", this);
     connect(cancelAction, &QAction::triggered, [&]() {
         flashCardEditWidget->clear();
         this->close();
     });
     saveToolBar->addAction(cancelAction);
}

void FlashCardEditWindow::loadFlashCard(const FlashCard &flashcard)
{
    flashCardEditWidget->loadFlashCard(flashcard);
}
