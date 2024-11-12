#ifndef MEMORYGAME_H
#define MEMORYGAME_H

#include <QStackedWidget>
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
    int gridSize = 2;
    QStringList gridSizes = {"4x4", "6x6"};
    QVector<int> cardValues = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                            10, 11, 12, 13, 14, 15, 16, 17, 18};

    QStackedWidget *stackedWidget;
    QWidget *startView;
    QWidget *gameView;

    QGridLayout *gridLayout;
    QVector<QPushButton*> cards;
    QPushButton *firstCard = nullptr;
    QPushButton *secondCard = nullptr;
    QTimer *resetTimer;

    void createStartView();
    void createGameView();

    void setupGame();
    void cardClicked(QPushButton *card);
    QVector<int> generateShuffledValues();
    void setCardHidden(QPushButton *card);
    void setCardTurned(QPushButton *card);
    void setCardDisabled(QPushButton *card);

private slots:
    void startGame();
    void setGridSize(QString size);
    void resetCards();
    void restartGame();
};

#endif // MEMORYGAME_H
