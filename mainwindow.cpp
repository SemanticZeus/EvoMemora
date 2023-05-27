#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupMenu();
    setupToolBar();
    flashCardView = new FlashCardView;
    flashCardEditWindow = new FlashCardEditWindow;
    flashCardAddNewWindow = new FlashCardAddNewWindow;
    restorePreviousSettings();
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(flashCardView);
    setCentralWidget(scrollArea);
    connect(flashCardEditWindow, &FlashCardEditWindow::updateView,
            flashCardView, &FlashCardView::reloadFlashCard);
    connect(flashCardAddNewWindow, &FlashCardAddNewWindow::newFlashcardsAdded,
            this, &MainWindow::updateFlashCardsList);

    leitner = new Leitner(flashCardPath, "leitner.rox");
}

MainWindow::~MainWindow()
{
    delete leitner;
}

void MainWindow::updateFlashCardsList()
{
    auto names = flashCardAddNewWindow->getNames();
    leitner->addFlashCard(names);
    flashCardAddNewWindow->clearNames();
}

void MainWindow::setupToolBar()
{
    QToolBar *toolbar = new QToolBar("Main ToolBar", this);
    addToolBar(toolbar);
    nextButton = new QPushButton;
    previousButton = new QPushButton;

    nextButton->setText("next flashcard");
    nextButton->setIcon(QIcon(":/resources/icons8-forward-100.png"));

    previousButton->setIcon(QIcon(":/resources/icons8-back-100.png"));
    previousButton->setText("previous flashcard");

    toolbar->addWidget(previousButton);
    toolbar->addWidget(nextButton);
    connect(nextButton, &QPushButton::released, [&](){

    });
}

void MainWindow::setupMenu()
{
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    QMenu* fileMenu = menuBar->addMenu("File");
    QAction* openAction = new QAction("Open FlashCards Folder", this);
    QAction *exitAction = new QAction("Exit", this);
    exitAction->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    connect(openAction, &QAction::triggered, this, &MainWindow::openAction);
    QAction* newFlashCardAction = new QAction("New FlashCard", this);
    newFlashCardAction->setShortcut(Qt::CTRL + Qt::Key_N);
    connect(newFlashCardAction, &QAction::triggered, [&]() {
        flashCardAddNewWindow->setRoot(flashCardPath);
        flashCardAddNewWindow->show();
        flashCardAddNewWindow->raise();
    });
    QAction* editFlashCardAction = new QAction("Edit Current FlashCard", this);
    newFlashCardAction->setShortcut(Qt::CTRL + Qt::Key_E);
    connect(editFlashCardAction, &QAction::triggered, [&]() {
       flashCardEditWindow->show();
       flashCardEditWindow->raise();
       flashCardEditWindow->loadFlashCard(flashCardView->getFlashCard());
    });
    fileMenu->addAction(openAction);
    fileMenu->addAction(newFlashCardAction);
    fileMenu->addAction(editFlashCardAction);
    fileMenu->addAction(exitAction);
}

void MainWindow::restorePreviousSettings()
{
    QSettings settings("Roxane", "Leitner");
    const QByteArray previousSize =
            settings.value("MainWindow/Size", QByteArray()).toByteArray();
    if (!previousSize.isEmpty()) restoreGeometry(previousSize);
    else resize(900,700);
    flashCardPath = settings.value("flashCardPath").toString();
    const QByteArray previousEditSize =
            settings.value("FlashCardEditWindow/Size", QByteArray()).toByteArray();
    if (!previousEditSize.isEmpty()) flashCardEditWindow->restoreGeometry(previousEditSize);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    QSettings settings("Roxane", "Leitner");
    settings.setValue("FlashCardEditWindow/Size", flashCardEditWindow->saveGeometry());
    settings.setValue("MainWindow/Size", saveGeometry());
    settings.setValue("flashCardPath", flashCardPath);
    settings.setValue("FlashCardPath", flashCardPath);
    leitner->save();
}

void MainWindow::openAction()
{
    QString folderPath = QFileDialog::getExistingDirectory(
        nullptr,
        "Select a folder",
        flashCardPath,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    // Check if a folder was selected
    if (folderPath.isEmpty()) return;
    flashCardPath = folderPath;
    emit flashCardPathUpdated();
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        int delta = event->angleDelta().y()/10;
        int w = flashCardView->width();
        qreal scaleFactor = static_cast<qreal>(w+delta)/w;
        flashCardView->scale(scaleFactor);
        update();
    }
    QMainWindow::wheelEvent(event);
}
