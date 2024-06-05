#include "homewidget.h"

class SquareButton : public QPushButton {
public:
    SquareButton(const QString& text, QWidget* parent = nullptr)
        : QPushButton(text, parent) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setMinimumSize(100, 100);
    }

    QSize sizeHint() const override {
        int length = qMin(width(), height());
        return QSize(length, length);
    }
    void paintEvent(QPaintEvent* event) override {
        QRectF rect = QRectF(contentsRect()).adjusted(4, 4, -4, -4); // Adjust the margins
        QFont font = this->font();
        int fontSize = calculateFontSize(rect);
        font.setPointSize(fontSize);
        this->setFont(font);
        QPushButton::paintEvent(event);
    }

private:
    int calculateFontSize(const QRectF& rect) const {
        QFont font = QApplication::font();
        int fontSize = 100;

        while (true) {
            font.setPointSize(fontSize);
            QFontMetrics metrics(font);

            QRect textRect = metrics.boundingRect(text());

            if (textRect.width() <= rect.width() && textRect.height() <= rect.height()) {
                // Text fits within the rectangle
                return fontSize;
            }

            // Reduce the font size
            --fontSize;

            // Break the loop if the font size becomes too small
            if (fontSize <= 0) {
                break;
            }
        }

        // Return a default font size if the loop doesn't find a suitable size
        return QApplication::font().pointSize();
    }
};

HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent)
{
    viewAllFlashcardsButton = new SquareButton("View All FlashCards");
    addNewFlashcardsButton = new SquareButton("Add a New FlashCard");
    reviewFlashcardsButton = new SquareButton("Review Todays FlashCards");
    syncFlashcardsButton = new SquareButton("Sync Flashcards");
    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->addWidget(reviewFlashcardsButton, 0, 0);
    mainLayout->addWidget(addNewFlashcardsButton, 0, 1);
    mainLayout->addWidget(viewAllFlashcardsButton, 1, 0);
    mainLayout->addWidget(syncFlashcardsButton, 1,1);
    setLayout(mainLayout);
}
