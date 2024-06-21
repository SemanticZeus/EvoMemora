#ifndef FLASHCARDVIEWWIDGET_H
#define FLASHCARDVIEWWIDGET_H

#include "flashcardview.h"
#include "flashcardmanager.h"

class ThumbnailItemWidget : public QWidget {
public:
    ThumbnailItemWidget(const QString& thumbnailPath, QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);

        // Create a QLabel for the thumbnail
        thumbnailLabel = new QLabel(this);
        thumbnail.load(thumbnailPath);
        thumbnailLabel->setPixmap(thumbnail);

        // Add the thumbnail label to the layout
        layout->addWidget(thumbnailLabel);
    }

protected:
    void resizeEvent(QResizeEvent *event) {
        QSize size = event->size();
        QPixmap pix = thumbnail.scaled(size,Qt::KeepAspectRatio);
        thumbnailLabel->setPixmap(pix);
    }

private:
    QPixmap thumbnail;
    QLabel* thumbnailLabel;
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
    void loadOverDueFlashcards();
signals:
    void homeActionTriggered();
    void editActionTriggered();


protected:
    void wheelEvent(QWheelEvent* event) override;
    void setupToolBar();
    void setupReviewButtons(QVBoxLayout *btnsLayout);
    void loadCurrentFlashCard();
    void loadFlashCardThumbnails();

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
