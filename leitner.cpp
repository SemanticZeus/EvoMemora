#include "leitner.h"
#include <cmath>

Leitner::Leitner(QString root, QString databaseName)
{
    this->root = root;
    this->databaseName = databaseName;
    NumberOfCells = std::pow(2, NumberOfBoxes)-1;
    for (int i=0;i<NumberOfBoxes;i++) endingCells+=std::pow(2, i+1)-2;
    reset();
    makeNewDatabaseIfNotValid();
    readDatabase();
}

void Leitner::makeNewDatabaseIfNotValid()
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
    xmlWriter.writeStartElement("leitner");
    xmlWriter.writeAttribute("date",QDate(1900, 1, 1).toString());
    xmlWriter.writeEndElement(); //leitner
    xmlWriter.writeEndDocument(); //header
    file.close();
}

void Leitner::reset()
{
    cells.clear();
    cells.resize(NumberOfCells);
    tempCells.clear();
    tempCells.resize(NumberOfBoxes);
    archive.clear();
}

void Leitner::addFlashCard(QString cardName)
{
    cells[0].append(cardName);
}

void Leitner::addFlashCard(QVector<QString> cardNameList)
{
    foreach(QString s, cardNameList)
        cells[0]+=s;
}

QList<QString> Leitner::getFlashCardList(int boxNumber)
{
    int n = std::pow(2, boxNumber+1)-2;
    if (n>NumberOfCells) return QList<QString>();
    return cells[n];
}

void Leitner::updateBox()
{
    if (QDate::currentDate() <= databaseDate) return;
    for (int i=NumberOfCells-1,k=NumberOfBoxes;i>=0;i--) {
        if (endingCells.contains(i)) {
            k--;
            if (i==NumberOfCells-1) {
                archive+=tempCells[k];
            } else {
                cells[i+1]+=tempCells[k];
                tempCells[k].clear();
            }
        } else {
            cells[i+1]+=cells[i];
            cells[i].clear();        }
    }
    databaseDate = QDate::currentDate();
}

void Leitner::setAnswer(QString cardName, int boxNumber, bool correct)
{
    if (boxNumber >= NumberOfBoxes || boxNumber < 0) return;
    int n = std::pow(2,boxNumber+1)-2;
    if (correct) {
        cells[n].removeAll(cardName);
        tempCells[boxNumber].append(cardName);
    } else {
        cells[boxNumber].removeAll(cardName);
        cells[0].append(cardName);
    }
}

void Leitner::save()
{
    if (root == "" || databaseName=="") return;
    updateBox();
    QString filename = QFileInfo(root, databaseName).absoluteFilePath();
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filename << file.errorString();
        return;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);

    xmlWriter.writeStartDocument(); //header
    xmlWriter.writeStartElement("leitner");
    xmlWriter.writeAttribute("date", databaseDate.toString());

    for (int c=0;c<NumberOfCells;c++) {
        for (int i=0;i<cells[c].count();i++) {
            xmlWriter.writeStartElement("flashcard");
            xmlWriter.writeAttribute("name", cells[c][i]);
            xmlWriter.writeAttribute("cell", QString::number(c));
            xmlWriter.writeEndElement(); // flashcard
        }
    }

    for (int c=0;c<NumberOfBoxes;c++) {
        for (int i=0;i<tempCells[c].count();i++) {
            xmlWriter.writeStartElement("temp");
            xmlWriter.writeAttribute("name", tempCells[c][i]);
            xmlWriter.writeAttribute("box", QString::number(c));
            xmlWriter.writeEndElement(); // flashcard
        }
    }

    for (int i=0;i<archive.count();i++) {
        xmlWriter.writeStartElement("archive");
        xmlWriter.writeAttribute("name", archive[i]);
        xmlWriter.writeEndElement(); // flashcard
    }

    xmlWriter.writeEndElement(); //leitner
    xmlWriter.writeEndDocument(); //header

    file.close();
}

void Leitner::readDatabase()
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

    currentDate = QDate::currentDate();
    if (!reader.readNextStartElement() || reader.name() != "leitner") {
        qDebug() << "Error reading the database file. ";
        return;
    }
    QString dateString = reader.attributes().value("date").toString();
    databaseDate = QDate::fromString(dateString, Qt::ISODate);
    if (!databaseDate.isValid())
        databaseDate = QDate(1900, 1, 1);

    while (!reader.atEnd() && !reader.hasError()) {
        if (reader.readNextStartElement()) {
            if (reader.name() == "flashcard") {
                int cellNumber = reader.attributes().value("cell").toInt();
                QString cardName = reader.attributes().value("name").toString();
                cells[cellNumber].push_back(cardName);
                reader.readElementText();
            } else if (reader.name() == "archive") {
                QString cardName = reader.attributes().value("name").toString();
                archive.push_back(cardName);
                reader.readElementText();
            } else if (reader.name() == "temp" ) {
                int tempBoxNumber = reader.attributes().value("box").toInt();
                QString cardName = reader.attributes().value("name").toString();
                tempCells[tempBoxNumber].push_back(cardName);
                reader.readElementText();
            } else {
                qDebug() << "Leitner.cpp error reading flashcard" << reader.name();
                reader.skipCurrentElement();
                continue;
            }
        }
    }
    file.close();
}
