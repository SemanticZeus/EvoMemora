#include "flashcardview.h"
#include "textwidget.h"
#include "imagewidget.h"

FlashCardView::FlashCardView(int w, int h, QWidget *parent)
    : QTabWidget(parent), frontWidget(nullptr), backWidget(nullptr)
{
    flashcard.setSize(QSize(w,h));
}

FlashCard FlashCardView::getFlashCard()
{
    return flashcard;
}

void FlashCardView::reset()
{
    totalScale=1;
    flashcard.clear();
    textList.clear();
    imageList.clear();
    if (frontWidget) delete frontWidget;
    if (backWidget) delete backWidget;
    frontWidget = new QWidget(this);
    backWidget = new QWidget(this);
}

bool FlashCardView::loadFlashCard(QString root, QString name)
{
    reset();
    flashcard.setRootFolder(root);
    if (!flashcard.loadFlashCard(name)) return false;
    loadFaceWidget(Front);
    loadFaceWidget(Back);
    addTab(frontWidget, "Question");
    addTab(backWidget, "Answer");
    adjustSize();
    setFixedSize(flashcard.getSize().width(), flashcard.getSize().height()*1.05);
    return true;
}

void FlashCardView::reloadFlashCard()
{
    totalScale = 1;
    loadFlashCard(flashcard.getRoot(), flashcard.getName());
}

void FlashCardView::loadFaceWidget(int face)
{
    QWidget *w;
    if (face == Front) w = frontWidget;
    else if (face == Back) w = backWidget;
    else return;
    w->setFixedSize(flashcard.getSize());
    QVBoxLayout *mainLayout = new QVBoxLayout;
    w->setLayout(mainLayout);
    auto rows = flashcard.getRows(face);
    for (int r=0;r<flashcard.rowCount(face);r++) {
        auto row = rows[r];
        QHBoxLayout *rowLayout = new QHBoxLayout;
        mainLayout->addLayout(rowLayout);
        rowLayout->setAlignment(Qt::AlignLeft);
        for (int i=0;i<row.count();i++) {
            auto item = row[i];
            if (item.type == Item::Text) {
                TextWidget *text = new TextWidget;
                text->setSize(item.width, item.height);
                text->setFont(item.font);
                text->setText(item.text);
                textList.append(text);
                text->setReadOnly(true);
                rowLayout->addWidget(text);
            } else if (item.type == Item::Image) {
                ImageWidget *image = new ImageWidget(item.image, item.width, item.height);
                imageList.append(image);
                rowLayout->addWidget(image);
            }
        }
    }
}

void FlashCardView::scale(qreal scaleFactor)
{
    if (scaleFactor*totalScale<.7) return;
    if (scaleFactor*totalScale>3) return;
    totalScale*=scaleFactor;
    int w = flashcard.getSize().width()*totalScale;
    int h = flashcard.getSize().height()*totalScale;
    frontWidget->setFixedSize(w, h*1.02);
    backWidget->setFixedSize(w, h*1.02);
    //setFixedSize(initialWidth+20, initialHeight+20);
    for (auto w : imageList) w->scale(scaleFactor);
    for (auto w : textList) w->scale(scaleFactor);
    setFixedSize(w, h*1.05);
}
