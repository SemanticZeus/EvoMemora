#include "flashcardedit.h"
#include <QSizePolicy>

FlashCardEdit::FlashCardEdit(QWidget *parent): QTabWidget(parent), snapShotWidget(nullptr),
    frontWidget(nullptr), backWidget(nullptr)
{
    newFlashCard();
}

void FlashCardEdit::newFlashCard()
{
    flashcard.clear();
    textList.clear();
    imageList.clear();
    if (frontWidget) delete frontWidget;
    if (backWidget) delete backWidget;
    this->clear();
    frontWidget = new QSplitter(Qt::Vertical);
    backWidget = new QSplitter(Qt::Vertical);
    this->addTab(frontWidget, "Front Side");
    this->addTab(backWidget, "Back Side");

    currentRow = -1;
    widgetSize = QSize(900, 700);
    this->setFixedSize(widgetSize);
    totalScaleFactor = 1;
    flashcard.setSize(widgetSize);
}

bool FlashCardEdit::save()
{
    updateFlashCard();
    flashcard.writeFlashCard();
    return true;
}

void FlashCardEdit::updateFlashCard()
{
    QString root = flashcard.getRoot();
    QString name = flashcard.getName();
    flashcard.clear();
    flashcard.setRootFolder(root);
    flashcard.setName(name);
    flashcard.setSize(widgetSize);
    for (int face = 0;face<numFaces;face++) {
        QSplitter *w = QList<QSplitter*>{frontWidget, backWidget}[face];
        for (int i=0;i<w->count();i++) {
            QSplitter *row = (QSplitter *) w->widget(i);
            for (int k=0;k<row->count();k++) {
                QWidget *item = row->widget(k);
                int w = item->width()/totalScaleFactor;
                int h = item->height()/totalScaleFactor;
                if (qobject_cast<ImageWidget*>(item)) {
                    ImageWidget *iw = qobject_cast<ImageWidget*>(item);
                    flashcard.addImage(face, i, iw->getPixmap(), w, h);
                } else if (qobject_cast<TextWidget*>(item)) {
                    TextWidget *tw = qobject_cast<TextWidget*>(item);
                    flashcard.addText(face, i, tw->toPlainText(), tw->font(), w, h);
                }
            }
        }
    }
    QPixmap frontThumbnail = generateThumbnail(frontWidget, QSize(400, 300));
    QPixmap backThumbnail = generateThumbnail(backWidget, QSize(400, 300));
    flashcard.setThumbnails(frontThumbnail, backThumbnail);
}

bool FlashCardEdit::loadFlashCard(FlashCard &flashcard)
{
    newFlashCard();
    this->flashcard = flashcard;
    loadFaceWidget(Front);
    loadFaceWidget(Back);
    addTab(frontWidget, "Question");
    addTab(backWidget, "Answer");
    adjustSize();
    setFixedHeight(flashcard.getSize().height()*1.02);
    return true;
}

bool FlashCardEdit::loadFlashCard(QString root, QString name)
{
    newFlashCard();
    flashcard.setRootFolder(root);
    if (!flashcard.loadFlashCard(name)) return false;
    loadFaceWidget(Front);
    loadFaceWidget(Back);
    addTab(frontWidget, "Question");
    addTab(backWidget, "Answer");
    adjustSize();
    setFixedHeight(flashcard.getSize().height()*1.02);

    return true;
}

void FlashCardEdit::loadFaceWidget(int face)
{
    QSplitter *w;
    if (face == Front) w = frontWidget;
    else if (face == Back) w = backWidget;
    else return;
    w->setFixedSize(flashcard.getSize());
    auto rows = flashcard.getRows(face);
    QList<int> verticalSizes;
    for (int r=0;r<flashcard.rowCount(face);r++) {
        auto row = rows[r];
        //QSplitter *row_splitter = new QSplitter(Qt::Horizontal);
        RowWidget *row_widget = new RowWidget;
        connect(row_widget, &RowWidget::widgetClicked,
                this, &FlashCardEdit::handleWidgetClick);
        //w->addWidget(row_splitter);
        w->addWidget(row_widget);
        QList<int> horizontalSizes;
        int s = 0;
        for (int i=0;i<row.count();i++) {
            auto item = row[i];
            if (item.type == Item::Text) {
                TextWidget *text = new TextWidget;
                //text->setSize(item.width, item.height);
                horizontalSizes << item.width;
                text->setFont(item.font);
                text->setText(item.text);
                textList.append(text);
                row_widget->addNextWidget(text);
                if (item.height>s) s = item.height;
                connect(text, &TextWidget::clicked,
                        row_widget, &RowWidget::childWidgetClicked);
            } else if (item.type == Item::Image) {
                ImageWidget *image = new ImageWidget(item.image, item.width, item.height);
                image->setMinimumSize(50, 50);
                imageList.append(image);
                horizontalSizes << item.width;
                row_widget->addNextWidget(image);
                if (item.height>s) s = item.height;
            }
        }
        row_widget->setSizes(horizontalSizes);
        verticalSizes << s;
    }
    w->setSizes(verticalSizes);
}

QSize FlashCardEdit::getFlashCardSize() {
    return widgetSize;
}

void FlashCardEdit::resizeTo(int width, int height)
{
    this->setFixedSize(width, height);
    totalScaleFactor = 1;
    flashcard.setSize(QSize(width, height));
    adjustSize();
    update();
}

void FlashCardEdit::addNewRow()
{
    QSplitter *splitter = (QSplitter *) this->currentWidget();
    RowWidget *row = new RowWidget;
    splitter->addWidget(row);
    connect(row, &RowWidget::widgetClicked,
            this, &FlashCardEdit::handleWidgetClick);
    handleWidgetClick(row);
    row->setContentsMargins(5, 5, 5, 5);
}

void FlashCardEdit::removeRow(int index)
{
    QSplitter *splitter = (QSplitter *)this->currentWidget();
    QWidget *item = splitter->widget(index);
    if (!item) return;
    delete item;
}

void FlashCardEdit::takeSnapShot()
{
    if (!snapShotWidget) {
        snapShotWidget = new SnapShotWidget;
        connect(snapShotWidget, &SnapShotWidget::accepted, [=]() {
            QSplitter *splitter = (QSplitter *) this->currentWidget();
            RowWidget *row = nullptr;
            for (int i=0;i<splitter->count();i++) {
                RowWidget *r = (RowWidget *)splitter->widget(i);
                if (r->active()) {
                    row = r;
                    break;
                }
            }
            if (!row) return;
            QPixmap pixmap = snapShotWidget->getPixmap();
            int width = row->width();
            int height = row->height();
            ImageWidget *image = new ImageWidget(pixmap, 5000, height);
            image->setMinimumSize(50, 50);
            row->addNextWidget(image);
        });
    }
    snapShotWidget->show();
    snapShotWidget->raise();
}

void FlashCardEdit::addText()
{
    QSplitter *splitter = (QSplitter *) this->currentWidget();
    RowWidget *row = nullptr;
    for (int i=0;i<splitter->count();i++) {
        RowWidget *r = (RowWidget *)splitter->widget(i);
        if (r->active()) {
            row = r;
            break;
        }
    }
    if (!row) return;
    TextWidget *textEdit = new TextWidget;
    QTextOption textOption = textEdit->document()->defaultTextOption();
    textOption.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    textEdit->document()->setDefaultTextOption(textOption);


    //textEdit->setStyleSheet("QTextEdit { margin: 10px; }");
    row->addNextWidget(textEdit);
    QFont font = textEdit->font();
    font.setPointSize(20);
    textEdit->setFont(font);

    connect(textEdit, &TextWidget::clicked,
            row, &RowWidget::childWidgetClicked);
}

void FlashCardEdit::handleWidgetClick(QWidget* sender)
{
    RowWidget *row = (RowWidget *)sender;
    currentRow = -1;
    QSplitter *splitter = (QSplitter *) this->currentWidget();
    for (int i = 0; i < splitter->count();i++) {
        RowWidget *c = (RowWidget *)(splitter->widget(i));
        c->setActive(false);
        if (c == row) {
            c->setActive(true);
            currentRow = i;
        }
    }
}

QPixmap FlashCardEdit::generateThumbnail(QWidget *widget, const QSize &size)
{
    QPixmap pixmap(widget->size());
    widget->render(&pixmap);
    return pixmap.scaled(size, Qt::KeepAspectRatio);
}

void FlashCardEdit::scale(qreal factor) {
    if (factor*totalScaleFactor<.7) return;
    if (factor*totalScaleFactor>3) return;
    totalScaleFactor*=factor;
    int w = flashcard.getSize().width()*totalScaleFactor;
    int h = flashcard.getSize().height()*totalScaleFactor;
    frontWidget->setFixedSize(w, h);
    backWidget->setFixedSize(w, h);
    //setFixedSize(initialWidth+20, initialHeight+20);
    //for (auto w : imageList) w->scale(totalScaleFactor);
    //for (auto w : textList) w->scale(totalScaleFactor);
    setFixedSize(w, h*1.02);
    adjustSize();
    update();
}
