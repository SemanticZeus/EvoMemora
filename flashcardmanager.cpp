#include "flashcardmanager.h"

FlashcardManager::FlashcardManager(QString root, QString databaseName)
{
    this->root = root;
    this->databaseName = databaseName;
    makeNewDatabaseIfNotValid();
    readDatabase();
}


void FlashcardManager::makeNewDatabaseIfNotValid()
{
    QString filename = QFileInfo(root, databaseName).absoluteFilePath();
    QFile file(filename);
    if (file.exists()) return;
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // File creation successful
        qDebug() << "New file created: " << filename;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument(); //header
    xmlWriter.writeStartElement("EvoMemora");
    xmlWriter.writeAttribute("date",QDate(1900, 1, 1).toString());
    xmlWriter.writeEndElement(); //EvoMemora
    xmlWriter.writeEndDocument(); //header
    file.close();
}

void FlashcardManager::reset()
{
    flashcardList.clear();
}

void FlashcardManager::addFlashCard(QString cardName)
{
    FlashCardManagerFlashCard flashcard(cardName);
    flashcardList.append(flashcard);
}

QList<FlashCardManagerFlashCard>& FlashcardManager::getFlashCardList()
{
    return flashcardList;
}

int FlashcardManager::countDueDateFlashCards()
{
    int count=0;
    for (auto &f : flashcardList)
        if (f.isShowTime()) count++;
    return count;
}

void FlashcardManager::save()
{
    if (root == "" || databaseName=="") return;
    QString filename = QFileInfo(root, databaseName).absoluteFilePath();
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filename << file.errorString();
        return;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);

    xmlWriter.writeStartDocument(); //header
    xmlWriter.writeStartElement("EvoMemora");
    xmlWriter.writeAttribute("date", QDate::currentDate().toString());

    for (int c=0;c<flashcardList.count();c++) {
            xmlWriter.writeStartElement("flashcard");
            xmlWriter.writeAttribute("name", flashcardList[c].name);
            xmlWriter.writeAttribute("previousDueDate", flashcardList[c].prevDueDate.toString());
            xmlWriter.writeAttribute("nextDueDate", flashcardList[c].nextDueDate.toString());
            xmlWriter.writeEndElement(); // flashcard
    }

    xmlWriter.writeEndElement(); //EvoMemora
    xmlWriter.writeEndDocument(); //header

    file.close();
}

void FlashcardManager::readDatabase()
{
    if (root == "" || databaseName=="") return;
    reset();
    QString filename = QFileInfo(root, databaseName).absoluteFilePath();
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filename << file.errorString();
        return;
    }
    QXmlStreamReader reader(&file);
    if (!reader.readNextStartElement() || reader.name() != QStringLiteral("EvoMemora")) {
        qDebug() << "Error reading the database file. ";
        return;
    }

    QString dateString = reader.attributes().value("date").toString();
    Q_UNUSED(dateString);

    while (!reader.atEnd() && !reader.hasError()) {
        if (reader.readNextStartElement()) {
            if (reader.name() == QStringLiteral("flashcard")) {
                QString cardName = reader.attributes().value("name").toString();
                FlashCardManagerFlashCard flashcard(cardName);
                flashcard.prevDueDate = QDateTime::fromString(reader.attributes().value("previousDueDate").toString());
                flashcard.nextDueDate = QDateTime::fromString(reader.attributes().value("nextDueDate").toString());
                flashcardList.append(flashcard);
                reader.readElementText();
            } else {
                qDebug() << "flashmanager.cpp error reading flashcard" << reader.name();
                reader.skipCurrentElement();
                continue;
            }
        }
    }
    file.close();
}
