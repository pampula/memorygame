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
    int gridSize = 4;
    QVector<int> cardValues{1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                            10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    QGridLayout *gridLayout;
    QVector<QPushButton*> cards;
    QPushButton *firstCard;
    QPushButton *secondCard;
    QTimer *resetTimer;

    void setupGame();
    void cardClicked(QPushButton *card);
    QVector<int> generateShuffledValues();
    void setCardHidden(QPushButton *card);
    void setCardTurned(QPushButton *card);
    void setCardDisabled(QPushButton *card);

private slots:
    void resetCards();
    void restartGame();
};

#endif // MEMORYGAME_H
