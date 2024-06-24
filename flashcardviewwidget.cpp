#include "flashcardviewwidget.h"

class MyListWidget : public QListWidget {
public:
using QListWidget::QListWidget;
    virtual void keyPressEvent(QKeyEvent *event) {
        QListWidget::keyPressEvent(event);
        emit itemClicked(this->currentItem());
    }
};


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
    listWidget = new MyListWidget;
    listWidget->setFixedWidth(210);
    connect(listWidget, &QListWidget::itemClicked, this, [&](QListWidgetItem *item){
        currentIndex = listWidget->row(item);
        loadCurrentFlashCard();
    });
    connect(listWidget, &QListWidget::itemClicked, this, [&](QListWidgetItem *item){
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
    auto &f = flashcardManager->getFlashCardIndex(flashCardsIndex[currentIndex]);
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
    for (int i=0;i<flashcardManager->getFlashcardNames().count();i++) {
        if (flashcardManager->getFlashCardIndex(i).isShowTime()) flashCardsIndex.push_back(i);
    }
    loadCurrentFlashCard();
    loadFlashCardThumbnails();
}

void FlashCardViewWidget::loadFlashCardThumbnails()
{
    listWidget->clear();
    for (int i=0;i<flashCardsIndex.count();i++) {
        auto &f = flashcardManager->getFlashCardIndex(flashCardsIndex[i]);
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
    struct ButtonInfo {
        QString label;
        int delayInSeconds;
    };

    std::vector<ButtonInfo> buttons = {
        {"Review in 1 min", 60},
        {"Review in 10 min", 600},
        {"Review in 1 hour", 3600},
        {"Review in 6 hours", 21600},
        {"Review in 12 hours", 43200},
        {"Review in 1 day", 86400},
        {"Review in 2 days", 172800},
        {"Review in 3 days", 259200},
        {"Review in 5 days", 432000},
        {"Review in 1 week", 604800},
        {"Review in 2 weeks", 1209600},
        {"Review in 1 month", 2592000},
        {"Review in 2 months", 5184000},
        {"Review in 3 months", 7776000},
        {"Review in 6 months", 15552000},
        {"Review in 1 year", 31536000}
    };

    for (auto &buttonInfo : buttons) {
        QPushButton *button = new QPushButton(buttonInfo.label, this);
        button->setFixedSize(150, 40);
        buttonsLayout->addWidget(button);
        connect(button, &QPushButton::released, this, [this, buttonInfo]() {
            if (flashCardsIndex.count() == 0) return;
            auto &f = flashcardManager->getFlashCardIndex(flashCardsIndex[currentIndex]);
            f.updateNextDueDateInSecs(buttonInfo.delayInSeconds);
            QString message = "You will review this flashcard in " + buttonInfo.label;
            messageLabel->setText(message);
        });
    }

    buttonsLayout->addStretch();
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
    connect(updateFlashcardListAction, &QAction::triggered, this, [&](){this->loadOverDueFlashcards();});
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
