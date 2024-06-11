#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QtWidgets>

struct Item {
    enum Type {Image, Text};
    Type type;
    int width;
    int height;
    QString text;
    QString imageName;
    QPixmap image;
    QFont font;
};

enum Face {Front, Back, numFaces};

class FlashCard
{
public:
    FlashCard();
    bool loadFlashCard(QString url);
    bool writeFlashCard();
    int rowCount(int face);
    void removeRow(int face, int r);
    void setSize(QSize s);
    QSize getSize();
    void setName(QString n);
    QString getName();
    void setRootFolder(QString r);
    void swapFaces();
    void addImage(int face, int row, QPixmap pixmap, int w, int h);
    void addImage(int face, int row, QString name, int w, int h);
    void addText(int face, int row, QString text, QFont font, int w, int h);
    void setThumbnails(QPixmap front, QPixmap back);
    QPixmap getThumbnail(int face);
    void clear();
    auto getRows(int face) { return rows[face]; }
    QString getRoot() { return root; }
    QString getModificationDate();

protected:
    void addImage(int face, int row, QString name, QPixmap pixmap, int w, int h);
    QString generateName();
    bool parseFace(QXmlStreamReader *reader, Face face);
    bool writeFace(int face, QXmlStreamWriter& writer);
    QString generateImageName();
    QString getAbsolutePath();
    void deleteImageFiles();

private:
    QList<QList<Item>> rows[numFaces];
    QPixmap thumbnailFront;
    QPixmap thumbnailBack;
    QString id;
    QString name;
    QString root;
    QString absolutePath;
    QSize size;
};

#endif // FLASHCARD_H
