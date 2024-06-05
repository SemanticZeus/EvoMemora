#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QtWidgets>

class HomeWidget : public QWidget
{
    Q_OBJECT
public:
    HomeWidget(QWidget *parent = nullptr);
    QPushButton *viewAllFlashcardsButton;
    QPushButton *addNewFlashcardsButton;
    QPushButton *reviewFlashcardsButton;
    QPushButton *syncFlashcardsButton;
};

#endif // HOMEWIDGET_H
