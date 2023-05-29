#ifndef FLASHCARDMANAGER_H
#define FLASHCARDMANAGER_H

#include <QtWidgets>
#include <cmath>

struct FlashCardManagerFlashCard {
    FlashCardManagerFlashCard(QString name)
    {
        this->name = name;
        nextDueDate = QDateTime::currentDateTime();
        prevDueDate = nextDueDate;
    }

    void updateNextDueDateInDays(int days)
    {
        prevDueDate = QDateTime::currentDateTime();
        prevDueDate.setTime(QTime(0, 0, 0));
        nextDueDate = prevDueDate.addDays(days);
    }

    void updateNextDueDateInSecs(int secs)
    {
        prevDueDate = QDateTime::currentDateTime();
        nextDueDate = prevDueDate.addSecs(secs);
    }

    bool isShowTime()
    {
        if (QDateTime::currentDateTime()>nextDueDate) return true;
        return false;
    }

    int preferedNextReviewDate() {
        int d = prevDueDate.daysTo(QDateTime::currentDateTime());
        if (d<1) d = 1;
        return findNearestPowerOf2(d);
    }

    int findNearestPowerOf2(int number) {
        double logValue = log2(number);
        int roundedValue = static_cast<int>(round(logValue));
        int nearestPowerOf2 = pow(2, roundedValue);
        return nearestPowerOf2;
    }

    QString name;
    QDateTime nextDueDate;
    QDateTime prevDueDate;
};

class FlashcardManager
{
public:
    FlashcardManager(QString root, QString databaseName);
    void addFlashCard(QString name);
    QList<FlashCardManagerFlashCard>& getFlashCardList();
    int countDueDateFlashCards();
    void save();
    void reset();
    QString getRoot() { return root; }

protected:
    void readDatabase();
    void makeNewDatabaseIfNotValid();
    void readDatabse();

private:
    QList<FlashCardManagerFlashCard> flashcardList;
    QString root;
    QString databaseName;
};

#endif // FLASHCARDMANAGER_H
