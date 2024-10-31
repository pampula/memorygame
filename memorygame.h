#ifndef MEMORYGAME_H
#define MEMORYGAME_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QVector>
#include <QTimer>

class MemoryGame : public QMainWindow {
    Q_OBJECT

public:
    MemoryGame(QWidget *parent = nullptr);
    ~MemoryGame();

private:
    QGridLayout *gridLayout;
    QVector<QPushButton*> cards;
    QPushButton *firstCard;
    QPushButton *secondCard;
    QTimer *resetTimer;

    void setupGame();
    void cardClicked(QPushButton *card);

    void setHiddenCardColor(QPushButton *card);
    void setTurnedCardColor(QPushButton *card);
    void disableCard(QPushButton *card);

private slots:
    void resetCards();
};

#endif // MEMORYGAME_H
