#ifndef FLASHCARDEDITWIDGET_H
#define FLASHCARDEDITWIDGET_H

#include <QWidget>
#include "flashcardedit.h"

class FlashCardEditWidget : public QWidget
{
    Q_OBJECT
public:
    FlashCardEditWidget(QWidget* parent = nullptr);
    void loadFlashCard(FlashCard f);
    void clear();
    void setRoot(QString r) { flashCardEdit->setRoot(r); }
    QString getRoot() { return flashCardEdit->getRoot(); }
    void save() { flashCardEdit->save(); }
    QString getName() { return flashCardEdit->getName(); }
    QToolBar *makeToolBar();

signals:
    void updateView();
    void newFlashcardsAdded();

protected:
    void setupToolBar();
    void wheelEvent(QWheelEvent* event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    FlashCardEdit *flashCardEdit;
    QVBoxLayout *mainLayout;
    QToolBar *toolBar;
    QFontDialog *fontDialog;
     QToolBar *fontToolBar;
     QComboBox *sizeComboBox;
     QComboBox *fontComboBox;
     QLabel *fontLabel;
     QLabel *sizeLabel;
     QString root;
     QVector<QString> names;
};

#endif // FLASHCARDEDITWIDGET_H
