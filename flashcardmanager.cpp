#include "flashcardmanager.h"
#include <QErrorMessage>
#include <QFile>
#include <QProgressDialog>
#include <QDebug>
#include <QHBoxLayout>
#include <QDirIterator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "credentials.h"

FlashcardManager::FlashcardManager(QString root, QString databaseName)
{
    this->root = root;
    this->databaseName = databaseName;
    makeNewDatabaseIfNotValid();
    readDatabase();
}


void FlashcardManager::makeNewDatabaseIfNotValid()
{
    if (!QDir(root).exists()) {
        root = "";
        databaseName = "";
        QMessageBox::warning(nullptr, "Can not Open the Folder.", "Error, Cannot Open the Folder", QMessageBox::Abort);
        return;
    }
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
    reset();
    flashcardList = readDatabase(this->root, this->databaseName);
}

QList<FlashCardManagerFlashCard> FlashcardManager::readDatabase(const QString& root, const QString& databaseName)
{
    QList<FlashCardManagerFlashCard> flashcardList;
    if (root == "" || databaseName=="")
        return flashcardList;

    QString filename = QFileInfo(root, databaseName).absoluteFilePath();
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filename << file.errorString();
        flashcardList.clear();
        return flashcardList;
    }
    QXmlStreamReader reader(&file);
    if (!reader.readNextStartElement() || reader.name() != QStringLiteral("EvoMemora")) {
        qDebug() << "Error reading the database file. ";
        flashcardList.clear();
        return flashcardList;
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
    return flashcardList;
}

void FlashcardManager::sync()
{
    if (root == "" || databaseName == "") {
        QMessageBox msg{};
        msg.setText("Error, please open a flashcard folder first");
        msg.setIcon(QMessageBox::Warning);
        msg.setInformativeText("Error");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        return;
    }
    QString tmpFolder = QDir(root).filePath("tmp");
    download(tmpFolder, "", "EvoMemora.rox");
    auto tmp_flashList = readDatabase(tmpFolder, "EvoMemora.rox");
}

QHttpMultiPart* FlashcardManager::make_header(QString username, QString password, QString command)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart username_part;
    username_part.setHeader(QNetworkRequest::ContentDispositionHeader,
                            QVariant("form-data; name=\"username\""));
    username_part.setBody(username.toUtf8());
    QHttpPart password_part;
    password_part.setHeader(QNetworkRequest::ContentDispositionHeader,
                            QVariant("form-data; name=\"password\""));
    password_part.setBody(password.toUtf8());

    QHttpPart command_part;

    command_part.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    command_part.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant("form-data; name=\"command\""));
    command_part.setBody(command.toUtf8());


    multiPart->append(username_part);
    multiPart->append(password_part);
    multiPart->append(command_part);

    return multiPart;
}


void FlashcardManager::upload(const QString &root, const QString& filename)
{
    if (!QFileInfo(QDir(root).filePath(filename)).isFile()) {
        qDebug() << "Error: file does not exist";
        return;
    }
    QHttpMultiPart *multiPart = make_header(username, password, "upload");

    QHttpPart fileContent;
    fileContent.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    fileContent.setHeader(QNetworkRequest::ContentDispositionHeader,
                          QVariant(QString("form-data; name=\"file\"; filename=\"%1\"").arg(filename)));
    QFile *file = new QFile(QDir(root).filePath(filename));
    file->open(QIODevice::ReadOnly);
    fileContent.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(fileContent);

    QUrl url("https://www.jamesaminian.com/evomemora/");
    request.setUrl(url);
    QNetworkReply *reply = manager.post(request, multiPart);
    multiPart->setParent(reply);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Upload successful";
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();
}

void FlashcardManager::download(const QString &root,
                    const QString &foldername,
                    const QString &filename)
{
    auto multiPart = make_header(username, password, "download");

    QHttpPart file;
    file.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    file.setHeader(QNetworkRequest::ContentDispositionHeader,
                   QVariant("form-data; name=\"path\""));
    QString full_path = QDir(foldername).filePath(filename);
    qDebug() << "full_path = " << full_path;
    file.setBody(full_path.toUtf8());
    multiPart->append(file);

    request.setUrl(url);
    QNetworkReply *reply = manager.post(request, multiPart);
    multiPart->setParent(reply);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QDir dir;
    if (!dir.exists(QDir(root).filePath(foldername))) {
        dir.mkdir(QDir(root).filePath(foldername));
    }

    QFile localFile(QDir(root).filePath(full_path));
    QString full_local_path = QDir(root).filePath(full_path);
    qDebug() << full_local_path;
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode == 200 && localFile.open(QIODevice::WriteOnly)) {
        localFile.write(reply->readAll());
        localFile.close();
        qDebug() << "Download successful, file written to downloaded_file";
    } else if (statusCode!=200)
        qDebug() << "Could not download the file";
    else {
        qDebug() << "Could not open file for writing.";
    }
    reply->deleteLater();
}

void FlashcardManager::upload_folder(const QString& root, const QString& foldername)
{
    QHttpMultiPart *multiPart = make_header(username, password, "upload folder");

    QHttpPart folder_name_http_part;
    folder_name_http_part.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    folder_name_http_part.setHeader(QNetworkRequest::ContentDispositionHeader,
                                    QVariant("form-data; name=\"foldername\""));
    folder_name_http_part.setBody(foldername.toUtf8());
    multiPart->append(folder_name_http_part);

    QString full_folder_path = QDir(root).filePath(foldername);

    QDirIterator it(full_folder_path, QDir::Files, QDirIterator::Subdirectories);

    int i=0;
    while (it.hasNext()) {
        QString file_path = it.next();
        QHttpPart fileContent;
        fileContent.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
        fileContent.setHeader(QNetworkRequest::ContentDispositionHeader,
                              QVariant(QString("form-data; name=\"file%1\"; filename=\"%2\"")
                                           .arg(i++).arg(file_path)));
        QFile *file = new QFile(file_path);
        file->open(QIODevice::ReadOnly);
        fileContent.setBodyDevice(file);
        file->setParent(multiPart);
        multiPart->append(fileContent);
    }

    request.setUrl(url);
    QNetworkReply *reply = manager.post(request, multiPart);
    multiPart->setParent(reply); // delete the multiPart with the reply

    QProgressDialog progressDialog("Uploading file...", "Cancel", 0, 100);
    progressDialog.setWindowModality(Qt::ApplicationModal);

    QObject::connect(reply, &QNetworkReply::uploadProgress, [&](qint64 bytesSent, qint64 totalBytes) {
        if (totalBytes > 0) {
            progressDialog.setValue(static_cast<int>(100 * bytesSent / totalBytes));
        }
    });

    QObject::connect(&progressDialog, &QProgressDialog::canceled, [&]() {
        reply->abort();
    });

    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        progressDialog.close();
    });

    //progressDialog.show();
    progressDialog.exec(); // Block until upload finished or canceled

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Upload successful";
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();
}


void FlashcardManager::download_folder(const QString& root, const QString& foldername)
{
    QStringList files = folder_list(foldername);
    for (auto &file : files) {
        download(root, foldername, file);
    }
}

QStringList FlashcardManager::folder_list(const QString& foldername)
{
    QHttpMultiPart *multiPart = make_header(username, password, "folder list");

    QHttpPart folder_path;
    folder_path.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    folder_path.setHeader(QNetworkRequest::ContentDispositionHeader,
                          QVariant("form-data; name=\"path\""));
    folder_path.setBody(foldername.toUtf8());
    multiPart->append(folder_path);

    request.setUrl(url);
    QNetworkReply *reply = manager.post(request, multiPart);
    multiPart->setParent(reply);
    manager.setTransferTimeout(5000);
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QStringList files_list;
        QByteArray response = reply->readAll();
        reply->deleteLater();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if (doc.isNull() || !doc.isObject())
            return QStringList();
        QJsonObject obj = doc.object();
        QJsonArray filesArray = obj["files"].toArray();
        for (int i=0;i<filesArray.size();i++) {
            files_list.append(filesArray[i].toString());
        }
        return files_list;
    } else {
        qDebug() << "Network error:" << reply->errorString();
        reply->deleteLater();
        return QStringList();
    }
}

