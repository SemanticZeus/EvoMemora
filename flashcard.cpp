#include "flashcard.h"

FlashCard::FlashCard()
{
    size = QSize(900, 700);
}

/*
QString FlashCard::generateName()
{
    if (root=="") return "";
    QDir folder(root);
    QStringList entryList = folder.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    int number = 1;
    while (true) {
        QString name = "flashcard" + QString::number(number);
        bool exists = false;
        for (int i = 0; i < entryList.size(); ++i) {
            if (entryList.at(i) == name) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            return name;
        }
        ++number;
    }
}
*/

QString FlashCard::generateName()
{
    if (root=="") return "";
    return QString("flashcard_") + QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss") + "_" + QSysInfo::machineHostName().replace(".", "_");
}

void FlashCard::removeRow(int face, int r)
{
    rows[face].removeAt(r);
}

void FlashCard::setSize(QSize s)
{
    size = s;
}

QSize FlashCard::getSize()
{
    return size;
}

void FlashCard::setName(QString n)
{
    name = n;
}

QString FlashCard::getName()
{
    return name;
}

void FlashCard::setRootFolder(QString r)
{
    root = r;
}

bool FlashCard::loadFlashCard(QString n)
{
    name = n;
    for (auto &r : rows) r.clear();
    absolutePath = getAbsolutePath();
    QString filename = QFileInfo(getAbsolutePath(), "flashcard.xml").absoluteFilePath();
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filename << file.errorString();
        return false;
    }

    QXmlStreamReader reader(&file);

    while (!reader.atEnd() && !reader.hasError()) {
        if (reader.readNextStartElement()) {
            if (reader.name() != QStringLiteral("flashcard")) {
                qDebug() << "error reading flashcard" << reader.name();
                return false;
            }
            int w = reader.attributes().value("width").toInt();
            int h = reader.attributes().value("height").toInt();
            size = QSize(w,h);

            //read front face
            if (!reader.readNextStartElement() || reader.name().toString() != "front")
                return false;
            if (!parseFace(&reader, Front)) return false;
            //read back face
            if (!reader.readNextStartElement() || reader.name().toString() != "back")
                return false;
            if (!parseFace(&reader, Back)) return false;
        }
    }
    file.close();
    if (QFileInfo(root, "front.jpg").exists()) {
        thumbnailFront = QPixmap(QFileInfo(root, "front.jpg").absoluteFilePath());
    }
    if (QFileInfo(root, "back.jpg").exists()) {
        thumbnailBack = QPixmap(QFileInfo(root, "back.jpg").absoluteFilePath());
    }
    return true;
}

bool FlashCard::parseFace(QXmlStreamReader *reader, Face face)
{
    int currentRow = -1;
    while (reader->readNextStartElement()) {
        if (reader->name().toString() != "row") reader->skipCurrentElement();
        currentRow++;
        while (reader->readNextStartElement()) {
            auto name = reader->name().toString();
            if (name == "text") {
                int width = reader->attributes().value("width").toInt();
                int height = reader->attributes().value("height").toInt();
                int fontSize = reader->attributes().value("font-size").toInt();
                QFont font;
                font.setPointSize(fontSize);
                QString text = reader->readElementText();
                addText(face, currentRow, text, font, width, height);
            } else if (name == "picture") {
                QString name = reader->attributes().value("src").toString();
                int width = reader->attributes().value("width").toInt();
                int height = reader->attributes().value("height").toInt();
                reader->readElementText();
                QString path = QFileInfo(getAbsolutePath(), name).absoluteFilePath();
                addImage(face, currentRow, name, QPixmap(path), width, height);
            } else {
                qDebug() << "skip"  << reader->name().toString();
                reader->skipCurrentElement();
            }
        }
    }
    return true;
}

void FlashCard::setThumbnails(QPixmap front, QPixmap back)
{
    thumbnailFront = front;
    thumbnailBack = back;
}

void FlashCard::deleteImageFiles()
{
    QDir folder(getAbsolutePath());
    folder.setNameFilters(QStringList() << "*.jpg");
    foreach (QString file, folder.entryList()) {
        QFile::remove(QFileInfo(getAbsolutePath(), file).absoluteFilePath());
    }
}

bool FlashCard::writeFlashCard()
{
    QDir dir;
    QString url = getAbsolutePath();
    if (root == "") {
        qDebug() << "Error root cannot be empty";
        return false;
    }
    if (!dir.exists(url) && !dir.mkpath(url))
        return false;
    deleteImageFiles();
    QString filename = QFileInfo(url, "flashcard.xml").absoluteFilePath();
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);

    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("flashcard");
    xmlWriter.writeAttribute("width", QString::number(size.width()));
    xmlWriter.writeAttribute("height", QString::number(size.height()));
    xmlWriter.writeAttribute("ID", id);

    xmlWriter.writeStartElement("front");
    writeFace(Front, xmlWriter);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("back");
    writeFace(Back, xmlWriter);
    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();//flashcard
    xmlWriter.writeEndDocument();

    file.close();

    thumbnailFront.save(QFileInfo(getAbsolutePath(), "front.jpg").absoluteFilePath());
    thumbnailBack.save(QFileInfo(getAbsolutePath(), "back.jpg").absoluteFilePath());
    return true;
}

int FlashCard::rowCount(int face)
{
    return rows[face].count();
}

void FlashCard::addImage(int face, int row, QString imageName, int w, int h)
{
    QPixmap pixmap(QFileInfo(getAbsolutePath(), imageName).absoluteFilePath());
    addImage(face, row, "", pixmap, w, h);
}

void FlashCard::addImage(int face, int row, QPixmap pixmap, int w, int h)
{
    addImage(face, row, "", pixmap, w, h);
}

void FlashCard::addImage(int face, int row, QString name,  QPixmap pixmap, int w, int h)
{
    int margin = 3;
    w-=margin;
    h-=margin;
    qreal iw = pixmap.width();
    qreal ih = pixmap.height();
    if (w/iw>h/ih) w=h/ih*iw; else h=w/iw*ih;
    auto &f = rows[face];
    while (f.count() <= row) f.append(QList<Item>{});
    auto &r = f[row];
    Item image;
    image.type = Item::Image;
    image.imageName = name;
    image.width = w;
    image.height = h;
    image.image = pixmap;
    r.append(image);
}

void FlashCard::addText(int face, int row, QString text, QFont font, int w, int h)
{
    auto &f = rows[face];
    while (f.count() <= row) f.append(QList<Item>{});
    auto &r = f[row];
    Item item;
    item.type = Item::Text;
    item.text = text;
    item.width = w;
    item.height = h;
    item.font = font;
    r.append(item);
}

bool FlashCard::writeFace(int face, QXmlStreamWriter& writer)
{
    auto &f = rows[face];
    for (int i = 0; i < f.count(); ++i) {
        writer.writeStartElement("row");
        for (int k=0;k<f[i].count();k++) {
            Item &item = f[i][k];
            if (item.type == Item::Text) {
                writer.writeStartElement("text");
                writer.writeAttribute("width", QString::number(item.width));
                writer.writeAttribute("height", QString::number(item.height));
                writer.writeAttribute("font-size",
                                      QString::number(item.font.pointSize()));
                writer.writeCharacters(item.text);
                writer.writeEndElement();
            } else if (item.type == Item::Image) {
                QString filename = item.imageName;
                if (filename == "") {
                    filename = generateImageName();
                    item.imageName = filename;
                }
                if (filename == "") return false;
                QString path = getAbsolutePath();
                if (path == "") return false;
                item.image.save(QFileInfo(path, filename).absoluteFilePath());
                writer.writeStartElement("picture");
                writer.writeAttribute("width", QString::number(item.width));
                writer.writeAttribute("height", QString::number(item.height));
                writer.writeAttribute("src", filename);
                writer.writeEndElement();
            }
        }
        writer.writeEndElement();
    }
    return true;
}

QString FlashCard::generateImageName() {
    QString folderPath = getAbsolutePath();
    if (folderPath == "") return "";
    QDir folder(folderPath);
    QStringList nameFilters;
    nameFilters << QString("image*.jpg");
    QStringList fileNames = folder.entryList(nameFilters, QDir::Files | QDir::NoDotAndDotDot);

    int lowestNumber = 1;
    for (const QString& fileName : fileNames) {
        QFileInfo fileInfo(fileName);
        QString baseName = fileInfo.baseName(); // Get the file name without extension

        bool ok;
        int number = baseName.mid(QString("image").length()).toInt(&ok); // Extract the number part of the file name
        if (ok && number >= lowestNumber) {
            lowestNumber = number + 1;
        }
    }
    return QString("image%1.jpg").arg(lowestNumber);
}

QString FlashCard::getAbsolutePath()
{
    if (name == "") name = generateName();
    QFileInfo fileInfo(root, name);
    return fileInfo.absoluteFilePath();
}

void FlashCard::clear()
{
    for(auto &f : rows) f.clear();
    thumbnailFront = QPixmap();
    thumbnailBack = QPixmap();
    id = "";
    name = "";
    root = "";
    absolutePath = "";
    size = QSize();
}
