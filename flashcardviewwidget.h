#ifndef FLASHCARDVIEWWIDGET_H
#define FLASHCARDVIEWWIDGET_H

#include "flashcardview.h"
#include "flashcardmanager.h"

class ThumbnailItemWidget : public QWidget {
public:
    ThumbnailItemWidget(const QPixmap& thumbnail, const QString& text, QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);
        QLabel* thumbnailLabel = new QLabel(this);
        QLabel* textLabel = new QLabel(text, this);

        thumbnailLabel->setPixmap(thumbnail.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        layout->addWidget(thumbnailLabel);
        layout->addWidget(textLabel);
        layout->setContentsMargins(5, 5, 5, 5);
        layout->setSpacing(5);

        setLayout(layout);
    }
};

class FlashCardViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FlashCardViewWidget(FlashcardManager *flashcardManager, QWidget *parent = nullptr);
    void addMenusAndToolBar(QMainWindow *mainwindow);
    void setFlashcardManager(FlashcardManager *flashcardManager);
    QToolBar *getToolBar();
    FlashCardView* getFlashCardView() { return flashCardView; }

signals:
    void homeActionTriggered();
    void editActionTriggered();


protected:
    void wheelEvent(QWheelEvent* event) override;
    void setupToolBar();
    void setupReviewButtons(QVBoxLayout *btnsLayout);
    void loadCurrentFlashCard();
    void loadOverDueFlashcards();

protected slots:
    void nextActionTriggered();
    void previousActionTriggered();

private:
    int currentIndex;
    FlashCardView *flashCardView;
    FlashcardManager *flashcardManager;
    QToolBar *toolBar;
    QMenu *menu;
    QAction *homeAction;
    QAction *nextAction;
    QAction *previousAction;
    QAction *editAction;
    QMenu *fileMenu;
    QColor backGroundColor;
    QVector<int> flashCardsIndex;
    QTimer refreshFlashCardList;
    QListWidget *listWidget;
    QLabel *messageLabel;
};

#endif // FLASHCARDVIEWWIDGET_H
