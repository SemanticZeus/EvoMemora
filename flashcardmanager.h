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

    QString message()
    {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        int seconds = prevDueDate.secsTo(currentDateTime);
        if (seconds < 60)
            return QString::number(seconds) + " second" + (seconds > 1 ? "s" : "") + " ago";

        int minutes = seconds / 60;
        if (minutes < 60)
        {
            int remainingSeconds = seconds % 60;
            return QString::number(minutes) + " minute" + (minutes > 1 ? "s" : "") + " and " +
                   QString::number(remainingSeconds) + " second" + (remainingSeconds > 1 ? "s" : "") + " ago";
        }

        int hours = minutes / 60;
        if (hours < 24)
        {
            int remainingMinutes = minutes % 60;
            return QString::number(hours) + " hour" + (hours > 1 ? "s" : "") + " and " +
                   QString::number(remainingMinutes) + " minute" + (remainingMinutes > 1 ? "s" : "") + " ago";
        }

        int days = hours / 24;
        int remainingHours = hours % 24;

        if (days < 30)
            {
                return QString::number(days) + " day" + (days > 1 ? "s" : "") + " and " +
                       QString::number(remainingHours) + " hour" + (remainingHours > 1 ? "s" : "") + " ago";
            }

            int months = days / 30;
            int remainingDays = days % 30;

            if (months < 12)
            {
                return QString::number(months) + " month" + (months > 1 ? "s" : "") + ", " +
                       QString::number(remainingDays) + " day" + (remainingDays > 1 ? "s" : "") + " and " +
                       QString::number(remainingHours) + " hour" + (remainingHours > 1 ? "s" : "") + " ago";
            }

            int years = months / 12;
            int remainingMonths = months % 12;

            return QString::number(years) + " year" + (years > 1 ? "s" : "") + ", " +
                   QString::number(remainingMonths) + " month" + (remainingMonths > 1 ? "s" : "") + ", " +
                   QString::number(remainingDays) + " day" + (remainingDays > 1 ? "s" : "") + ", " +
                   QString::number(remainingHours) + " hour" + (remainingHours > 1 ? "s" : "") + " ago";
    }

    QString name;
    QDateTime nextDueDate;
    QDateTime prevDueDate;
};
//
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
