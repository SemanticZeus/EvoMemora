#include "textwidget.h"

TextWidget::TextWidget(QWidget *parent) : QTextEdit(parent) {
    setFontSize(20);
    initialFontSize = 20;
    //setStyleSheet("border: none;");
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void TextWidget::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);
}

void TextWidget::setFontSize(qreal fz)
{
    initialFontSize = fz;
    QFont font = this->font();
    font.setPointSize(initialFontSize);
    setFont(font);
}

void TextWidget::scale(float s)
{
    totalScale*=s;
    int w = initialWidth*totalScale;
    int h = initialHeight*totalScale;
    setFixedSize(w, h);
    QFont font = this->font();
    font.setPointSize(initialFontSize*totalScale);
    setFont(font);
}
