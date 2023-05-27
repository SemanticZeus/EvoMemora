#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include <QtWidgets>

class TextWidget : public QTextEdit
{
    Q_OBJECT
public:
    TextWidget(QWidget *parent = nullptr);
    void scale(float s);
    void setFontSize(qreal fz);
    void setSize(int w, int h) {
        setFixedSize(w,h);
        initialWidth = w;
        initialHeight = h;
    }

signals:
    void clicked(TextWidget *textEdit);

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        emit clicked(this);
        QTextEdit::mousePressEvent(event);
    }
    void resizeEvent(QResizeEvent *event) override;

private:
    qreal initialWidth;
    qreal initialHeight;
    qreal totalScale = 1;
    qreal initialFontSize;
};

#endif // TEXTWIDGET_H
