#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    databaseName = "EvoMemora.rox";
    setupMenu();
    restorePreviousSettings();
    flashcardManager = new FlashcardManager(flashCardPath, databaseName);

    flashCardViewWidget = new FlashCardViewWidget(flashcardManager);
    addToolBar(flashCardViewWidget->getToolBar());
    flashCardViewWidget->getToolBar()->hide();
    flashCardEditWindow = new FlashCardEditWindow;
    flashCardAddNewWindow = new FlashCardAddNewWindow(flashcardManager);
    addToolBar(flashCardAddNewWindow->getToolBar());
    flashCardAddNewWindow->getToolBar()->hide();


    homeWidget = new HomeWidget;
    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(homeWidget);
    stackedWidget->addWidget(flashCardViewWidget);
    stackedWidget->addWidget(flashCardAddNewWindow);
    stackedWidget->addWidget(flashCardEditWindow);
    setCentralWidget(stackedWidget);


    connect(homeWidget->reviewFlashcardsButton, &QPushButton::released, this, [&]() {
        flashCardViewWidget->getToolBar()->show();
        stackedWidget->setCurrentWidget(flashCardViewWidget);
    });
    connect(flashCardViewWidget, &FlashCardViewWidget::homeActionTriggered, this, [&](){
       flashCardViewWidget->getToolBar()->hide();
       stackedWidget->setCurrentWidget(homeWidget);
    });
    connect(homeWidget->addNewFlashcardsButton, &QPushButton::released, this, [&]() {
        flashCardAddNewWindow->getToolBar()->show();
        flashCardAddNewWindow->setRoot(flashCardPath);
        stackedWidget->setCurrentWidget(flashCardAddNewWindow);
    });
    connect(flashCardAddNewWindow, &FlashCardAddNewWindow::homeActionTriggered, this, [&](){
       flashCardAddNewWindow->getToolBar()->hide();
       stackedWidget->setCurrentWidget(homeWidget);
    });
    connect(flashCardEditWindow, &FlashCardEditWindow::updateView,
            flashCardViewWidget->getFlashCardView(), &FlashCardView::reloadFlashCard);

    connect(flashCardViewWidget, &FlashCardViewWidget::editActionTriggered, this, [&](){
        auto root = flashCardViewWidget->getFlashCardView()->getRoot();
        auto flashcard = flashCardViewWidget->getFlashCardView()->getName();
        QString path = QFileInfo(root, flashcard).absoluteFilePath();
        flashCardViewWidget->getToolBar()->hide();
        stackedWidget->setCurrentWidget(flashCardEditWindow);
        flashCardEditWindow->loadFlashCard(flashCardViewWidget->getFlashCardView()->getFlashCard());
    });

    connect(flashCardEditWindow, &FlashCardEditWindow::updateView, this, [&](){
        flashCardViewWidget->getToolBar()->show();
        stackedWidget->setCurrentWidget(flashCardViewWidget);

    });
    connect(flashCardEditWindow, &FlashCardEditWindow::cancel, this, [&](){
        flashCardViewWidget->getToolBar()->show();
        stackedWidget->setCurrentWidget(flashCardViewWidget);
    });
}

MainWindow::~MainWindow()
{

    delete flashcardManager;
}

void MainWindow::setupMenu()
{
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    QMenu* fileMenu = menuBar->addMenu("File");
    QAction* openAction = new QAction("Open FlashCards Folder", this);
    QAction *exitAction = new QAction("Exit", this);
    exitAction->setShortcut(Qt::CTRL | Qt::Key_Q);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    connect(openAction, &QAction::triggered, this, &MainWindow::openAction);

    QAction* editFlashCardAction = new QAction("Edit Current FlashCard", this);
    connect(editFlashCardAction, &QAction::triggered, this, [&]() {
       flashCardEditWindow->show();
       flashCardEditWindow->raise();
       flashCardEditWindow->loadFlashCard(flashCardView->getFlashCard());
    });
    fileMenu->addAction(openAction);
    fileMenu->addAction(exitAction);
}

void MainWindow::restorePreviousSettings()
{
    QSettings settings("Roxane", databaseName);
    const QByteArray previousSize =
            settings.value("MainWindow/Size", QByteArray()).toByteArray();
    if (!previousSize.isEmpty()) restoreGeometry(previousSize);
    else resize(900,700);
    flashCardPath = settings.value("flashCardPath").toString();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    QSettings settings("Roxane", databaseName);
    settings.setValue("MainWindow/Size", saveGeometry());
    settings.setValue("flashCardPath", flashCardPath);
    settings.setValue("FlashCardPath", flashCardPath);
    flashcardManager->save();
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
    flashcardManager->save();
    delete flashcardManager;
    flashcardManager = new FlashcardManager(flashCardPath, databaseName);
}
