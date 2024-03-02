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
     connect(saveAction, &QAction::triggered, this, [&]() {
         flashCardEditWidget->save();
         emit updateView();
     });
     saveToolBar->addAction(saveAction);

     QAction *cancelAction = new QAction("Cancel", this);
     connect(cancelAction, &QAction::triggered, this, [&]() {
         flashCardEditWidget->clear();
         emit cancel();
     });
     saveToolBar->addAction(cancelAction);
}

void FlashCardEditWindow::loadFlashCard(const FlashCard &flashcard)
{
    flashCardEditWidget->loadFlashCard(flashcard);
}
