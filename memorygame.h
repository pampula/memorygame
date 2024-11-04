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

    QGridLayout *gridLayout;
    QVector<QPushButton*> cards;
    QPushButton *firstCard;
    QPushButton *secondCard;
    QTimer *resetTimer;

    void setupGame();
    void cardClicked(QPushButton *card);

    void setCardHidden(QPushButton *card);
    void setCardTurned(QPushButton *card);
    void setCardDisabled(QPushButton *card);

private slots:
    void resetCards();
    void restartGame();
};

#endif // MEMORYGAME_H
