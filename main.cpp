#include <QApplication>
#include "mainwindow.h"
#include <QSysInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPixmap pixmap(":/resources/icons8-smartphone-ram-64.png");
    pixmap.scaled(300,300);
    QIcon icon(pixmap);
    app.setWindowIcon(icon);
    MainWindow w;
    w.show();
    qDebug() << QString("flashcard_") + QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss") + "_" + QSysInfo::machineHostName().replace(".", "_");
    return app.exec();
}
