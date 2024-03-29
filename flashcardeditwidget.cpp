#include "flashcardeditwidget.h"

FlashCardEditWidget::FlashCardEditWidget(QWidget *parent) : QWidget(parent)
{
    setupToolBar();
    mainLayout = new QVBoxLayout(this);
    flashCardEdit = new FlashCardEdit;
    QScrollArea *scrollArea = new QScrollArea;
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(fontToolBar);
    scrollArea->setWidget(flashCardEdit);
    mainLayout->addWidget(scrollArea);
}

void FlashCardEditWidget::clear()
{
    flashCardEdit->newFlashCard();
}

void FlashCardEditWidget::loadFlashCard(FlashCard f)
{
    flashCardEdit->loadFlashCard(f);
}

void FlashCardEditWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit updateView();
}

QToolBar *FlashCardEditWidget::makeToolBar()
{
    QToolBar *mainToolBar = new QToolBar;
    //QWidget* spacer = new QWidget();
    //spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //saveToolBar->addWidget(spacer);
    mainToolBar->setObjectName("FlashCardAddNewToolBar");

     QAction *addAction = new QAction("Add FlashCard", this);
     connect(addAction, &QAction::triggered, this, [&]() {
         this->save();
         names.append(this->getName());
         this->clear();
         this->setRoot(root);
     });
     mainToolBar->addAction(addAction);

     QAction *closeAction = new QAction("Close", this);
     connect(closeAction, &QAction::triggered, this, [&]() {
         this->clear();
         this->close();
         emit newFlashcardsAdded();
     });
     mainToolBar->addAction(closeAction);

     return mainToolBar;
}

void FlashCardEditWidget::setupToolBar()
{
    toolBar = new QToolBar;
    toolBar->setObjectName("FlashCardEditWidgetToolBar");

     QAction *addRowAction = new QAction("Add New Row", this);
     connect(addRowAction, &QAction::triggered, this, [&]() {
         flashCardEdit->addNewRow();
     });
     toolBar->addAction(addRowAction);

     QAction *removeRowAction = new QAction("Remove Current Row", this);
     connect(removeRowAction, &QAction::triggered, this, [&](){
         flashCardEdit->removeCurrentRow();
     });

     toolBar->addAction(removeRowAction);

     QAction *addTextAction = new QAction("Add Text", this);
     connect(addTextAction, &QAction::triggered, this, [&]() {
        flashCardEdit->addText();
     });
     toolBar->addAction(addTextAction);

     QAction *addImageAction = new QAction("Add Image", this);
     connect(addImageAction, &QAction::triggered, this, [&]() {
         flashCardEdit->takeSnapShot();
     });
     toolBar->addAction(addImageAction);


     fontDialog = new QFontDialog(this);
     fontToolBar = new QToolBar("font toolbar", this);
     fontToolBar->setObjectName("fontToolBar");

     fontLabel = new QLabel("Font: Arial", this);
     sizeLabel = new QLabel("Size: 12", this);

     fontComboBox = new QComboBox(this);
     fontComboBox->addItems({"Arial", "Times New Roman", "Verdana"});

     sizeComboBox = new QComboBox(this);
     sizeComboBox->addItems({"12", "14", "16", "18", "20"});

     fontToolBar->addWidget(fontLabel);
     fontToolBar->addWidget(fontComboBox);
     fontToolBar->addSeparator();
     fontToolBar->addWidget(sizeLabel);
     fontToolBar->addWidget(sizeComboBox);

     //connect(fontComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::updateFont);
     //connect(sizeComboBox, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::updateFontSize);
}

void FlashCardEditWidget::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        int delta = event->angleDelta().y()/10;
        int w = flashCardEdit->width();
        qreal scaleFactor = static_cast<qreal>(w+delta)/w;
        flashCardEdit->scale(scaleFactor);
        //flashCardEdit->changeSize(QSize(delta, delta));
        update();
    }

    QWidget::wheelEvent(event);
}
