#ifndef LEITNER_H
#define LEITNER_H

#include <QtWidgets>

class Leitner
{
public:
    Leitner(QString root, QString databaseName);
    void addFlashCard(QString cardName);
    void addFlashCard(QVector<QString> cardNameList);
    QList<QString> getFlashCardList(int boxNumber);
    void setAnswer(QString cardName, int boxNumber, bool correct);
    void save();
    void updateBox();

protected:
    void readDatabase();
    void reset();
    void makeNewDatabaseIfNotValid();

private:
    static const int NumberOfBoxes = 9;
    int NumberOfCells;
    QVector<int> endingCells;
    QDate currentDate;
    QDate databaseDate;
    QVector<QList<QString>> cells;
    QVector<QList<QString>> tempCells;
    QList<QString> archive;
    QString root;
    QString databaseName;
};

#endif // LEITNER_H
