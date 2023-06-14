#include "flashcardviewwidget.h"

FlashCardViewWidget::FlashCardViewWidget(FlashcardManager *flashcardManager, QWidget *parent) : QWidget(parent)
{
    this->flashcardManager = flashcardManager;
    setupToolBar();
    flashCardView = new FlashCardView;
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(flashCardView);
    messageLabel = new QLabel;
    messageLabel->setText("No message");
    messageLabel->setFixedHeight(100);
    QFont font;
    font.setPointSize(19);
    messageLabel->setFont(font);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *mainHorizontalLayout = new QHBoxLayout;
    listWidget = new QListWidget;
    listWidget->setFixedWidth(210);
    connect(listWidget, &QListWidget::itemClicked, [&](QListWidgetItem *item){
        currentIndex = listWidget->row(item);
        loadCurrentFlashCard();
    });
    QVBoxLayout *buttonsLayout = new QVBoxLayout;
    setupReviewButtons(buttonsLayout);

    mainHorizontalLayout->addWidget(listWidget);
    mainHorizontalLayout->addWidget(scrollArea);
    mainHorizontalLayout->addLayout(buttonsLayout);
    mainLayout->addLayout(mainHorizontalLayout);
    mainLayout->addWidget(messageLabel);
    setLayout(mainLayout);
    currentIndex = 0;
    loadOverDueFlashcards();
    loadCurrentFlashCard();
}

void FlashCardViewWidget::loadCurrentFlashCard()
{
    if (flashCardsIndex.count()==0 || currentIndex>=flashCardsIndex.count()) {
        messageLabel->setText("No flashcard to review right now.");
        flashCardView->reset();
        return;
    }
    auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
    flashCardView->loadFlashCard(flashcardManager->getRoot(), f.name);
    listWidget->setCurrentRow(currentIndex);
    messageLabel->setText("You reviewed this flashcard " + f.message());
}

void FlashCardViewWidget::loadOverDueFlashcards()
{
    currentIndex = 0;
    flashCardsIndex.clear();
    listWidget->clear();
    messageLabel->clear();
    flashCardView->reset();
    for (int i=0;i<flashcardManager->getFlashCardList().count();i++) {
        if (flashcardManager->getFlashCardList()[i].isShowTime()) flashCardsIndex.push_back(i);
    }
    loadCurrentFlashCard();
    loadFlashCardThumbnails();
}

void FlashCardViewWidget::loadFlashCardThumbnails()
{
    listWidget->clear();
    for (int i=0;i<flashCardsIndex.count();i++) {
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[i]];
        QString path = QFileInfo(flashcardManager->getRoot(), f.name).absoluteFilePath();
        QString thumbnail_path = QFileInfo(path, "front.jpg").absoluteFilePath();
        QListWidgetItem *thumbnail = new QListWidgetItem(listWidget);
        thumbnail->setSizeHint(QSize(200, 160));
        ThumbnailItemWidget *thumbnailItemWidget = new ThumbnailItemWidget(thumbnail_path);
        thumbnailItemWidget->setFixedSize(190, 150);
        listWidget->setItemWidget(thumbnail, thumbnailItemWidget);
    }
}

void FlashCardViewWidget::setupReviewButtons(QVBoxLayout *buttonsLayout)
{
    QPushButton *reviewIn1min = new QPushButton("Review in 1 min");
    QPushButton *reviewIn10min = new QPushButton("Review in 10 min");
    QPushButton *reviewIn1hour = new QPushButton("Review in 1 hour");
    QPushButton *reviewIn6hour = new QPushButton("Review in 6 hours");
    QPushButton *reviewIn12hour = new QPushButton("Review in 12 hours");
    QPushButton *reviewIn1day = new QPushButton("Review in 1 day");
    QPushButton *reviewIn2days = new QPushButton("Review in 2 days");
    QPushButton *reviewIn3days = new QPushButton("Review in 3 days");
    QPushButton *reviewIn5days = new QPushButton("Review in 5 days");
    QPushButton *reviewIn1week = new QPushButton("Review in 1 week");
    QPushButton *reviewIn2week = new QPushButton("Review in 2 week");
    QPushButton *reviewIn1month = new QPushButton("Review in 1 month");
    QPushButton *reviewIn2month = new QPushButton("Review in 2 month");
    QPushButton *reviewIn3month = new QPushButton("Review in 3 month");
    QPushButton *reviewIn6month = new QPushButton("Review in 6 month");
    QPushButton *reviewIn1year = new QPushButton("Review in 1 year");
    buttonsLayout->addWidget(reviewIn1min);
    buttonsLayout->addWidget(reviewIn10min);
    buttonsLayout->addWidget(reviewIn1hour);
    buttonsLayout->addWidget(reviewIn6hour);
    buttonsLayout->addWidget(reviewIn12hour);
    buttonsLayout->addWidget(reviewIn1day);
    buttonsLayout->addWidget(reviewIn2days);
    buttonsLayout->addWidget(reviewIn3days);
    buttonsLayout->addWidget(reviewIn5days);
    buttonsLayout->addWidget(reviewIn1week);
    buttonsLayout->addWidget(reviewIn2week);
    buttonsLayout->addWidget(reviewIn1month);
    buttonsLayout->addWidget(reviewIn2month);
    buttonsLayout->addWidget(reviewIn3month);
    buttonsLayout->addWidget(reviewIn6month);
    buttonsLayout->addWidget(reviewIn1year);
    buttonsLayout->addStretch();
    reviewIn1min->setFixedSize(150, 40);
    reviewIn10min->setFixedSize(150, 40);
    reviewIn1hour->setFixedSize(150, 40);
    reviewIn6hour->setFixedSize(150, 40);
    reviewIn12hour->setFixedSize(150, 40);
    reviewIn1day->setFixedSize(150, 40);
    reviewIn2days->setFixedSize(150, 40);
    reviewIn3days->setFixedSize(150, 40);
    reviewIn5days->setFixedSize(150, 40);
    reviewIn1week->setFixedSize(150, 40);
    reviewIn2week->setFixedSize(150, 40);
    reviewIn1month->setFixedSize(150, 40);
    reviewIn2month->setFixedSize(150, 40);
    reviewIn3month->setFixedSize(150, 40);
    reviewIn6month->setFixedSize(150, 40);
    reviewIn1year->setFixedSize(150, 40);
    connect(reviewIn1min, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInSecs(60);
        messageLabel->setText("you will review this flashcard in 1 min");
    });
    connect(reviewIn10min, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInSecs(600);
        messageLabel->setText("you will review this flashcard in 10 min");
    });
    connect(reviewIn1hour, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInSecs(60*60);
        messageLabel->setText("you will review this flashcard in 1 hour");
    });
    connect(reviewIn6hour, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInSecs(6*60*60);
        messageLabel->setText("you will review this flashcard in 6 hours");
    });
    connect(reviewIn12hour, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInSecs(12*60*60);
        messageLabel->setText("you will review this flashcard in 12 hours");
    });
    connect(reviewIn1day, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInDays(1);
        messageLabel->setText("you will review this flashcard in 1 day");
    });
    connect(reviewIn2days, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInDays(2);
        messageLabel->setText("you will review this flashcard in 2 days");
    });
    connect(reviewIn3days, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInDays(3);
        messageLabel->setText("you will review this flashcard in 3 days");
    });
    connect(reviewIn5days, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInDays(5);
        messageLabel->setText("you will review this flashcard in 5 days");
    });
    connect(reviewIn1week, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInDays(7);
        messageLabel->setText("you will review this flashcard in 1 week");
    });
    connect(reviewIn2week, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInDays(14);
        messageLabel->setText("you will review this flashcard in 2 week");
    });
    connect(reviewIn1month, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInDays(30);
        messageLabel->setText("you will review this flashcard in 1 month");
    });
    connect(reviewIn2month, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInDays(60);
        messageLabel->setText("you will review this flashcard in 2 month");
    });
    connect(reviewIn3month, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInDays(3*30);
        messageLabel->setText("you will review this flashcard in 3 month");
    });
    connect(reviewIn6month, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInDays(6*30);
        messageLabel->setText("you will review this flashcard in 6 month");
    });
    connect(reviewIn1year, &QPushButton::released, [&](){
        if (flashCardsIndex.count()==0) return;
        auto &f = flashcardManager->getFlashCardList()[flashCardsIndex[currentIndex]];
        f.updateNextDueDateInDays(365);
        messageLabel->setText("you will review this flashcard in 1 year");
    });
}

void FlashCardViewWidget::nextActionTriggered()
{
    currentIndex++;
    if (currentIndex>=flashCardsIndex.count()) {
        currentIndex = flashCardsIndex.count()-1;
        QMessageBox msgBox;
        msgBox.setWindowTitle("No more Flashcards for now.");
        msgBox.setText("No more flashcards are left for review right now. you can press refresh the flashcards list to refresh the list.");
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    loadCurrentFlashCard();
    messageLabel->clear();
}

void FlashCardViewWidget::previousActionTriggered()
{
    currentIndex--;
    if (currentIndex<0) currentIndex = 0;
    loadCurrentFlashCard();
    messageLabel->clear();
}

void FlashCardViewWidget::setupToolBar()
{
    homeAction = new QAction("Home");
    nextAction = new QAction("Next FlashCard");
    previousAction = new QAction("previous FlashCard");
    editAction = new QAction("Edit This FlashCard");
    QAction *updateFlashcardListAction = new QAction("Refresh the flashcard list");
    toolBar = new QToolBar;
    toolBar->addAction(homeAction);
    toolBar->addSeparator();
    toolBar->addAction(previousAction);
    toolBar->addAction(nextAction);
    toolBar->addSeparator();
    toolBar->addAction(editAction);
    toolBar->addAction(updateFlashcardListAction);

    connect(homeAction, &QAction::triggered, this, &FlashCardViewWidget::homeActionTriggered);
    connect(editAction, &QAction::triggered, this, &FlashCardViewWidget::editActionTriggered);
    connect(previousAction, &QAction::triggered, this, &FlashCardViewWidget::previousActionTriggered);
    connect(nextAction, &QAction::triggered, this, &FlashCardViewWidget::nextActionTriggered);
    connect(updateFlashcardListAction, &QAction::triggered, [&](){this->loadOverDueFlashcards();});
}

QToolBar *FlashCardViewWidget::getToolBar()
{
    return toolBar;
}

void FlashCardViewWidget::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        int delta = event->angleDelta().y()/10;
        int w = flashCardView->width();
        qreal scaleFactor = static_cast<qreal>(w+delta)/w;
        flashCardView->scale(scaleFactor);
        update();
    }
    QWidget::wheelEvent(event);
}
