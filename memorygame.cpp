#include "memorygame.h"
#include <QRandomGenerator>
#include <QTimer>

MemoryGame::MemoryGame(QWidget *parent)
    : QMainWindow(parent), firstCard(nullptr), secondCard(nullptr) {
    setWindowTitle("Memory Game");
    resize(800, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    gridLayout = new QGridLayout(centralWidget);
    setupGame();

    resetTimer = new QTimer(this);
    resetTimer->setSingleShot(true);
    connect(resetTimer, &QTimer::timeout, this, &MemoryGame::resetCards);
}

MemoryGame::~MemoryGame() {}

void MemoryGame::setupGame() {
    int gridSize = 4;  // 4x4 grid
    QVector<int> cardValues{1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(cardValues.begin(), cardValues.end(), std::default_random_engine(seed));


    for (int i = 0; i < gridSize * gridSize; ++i) {
        QPushButton *card = new QPushButton(this);
        card->setFixedSize(100, 100);
        card->setProperty("value", cardValues[i]);
        card->setText("?");

        connect(card, &QPushButton::clicked, [this, card]() { cardClicked(card); });

        gridLayout->addWidget(card, i / gridSize, i % gridSize);
        cards.append(card);
    }
}

void MemoryGame::cardClicked(QPushButton *card) {
    //two cards are already turned
    if (firstCard && secondCard) return;

    card->setText(QString::number(card->property("value").toInt()));

    if (!firstCard) {
        firstCard = card;
    }
    else if (!secondCard) {
        // Ensure the second card is not the same as the first
        if (card == firstCard) return;

        secondCard = card;

        if (firstCard->property("value") == secondCard->property("value")) {
            firstCard->setEnabled(false);
            secondCard->setEnabled(false);
            firstCard = nullptr;
            secondCard = nullptr;
        }
        else {
          resetTimer->start(500);
        }
    }
}

void MemoryGame::resetCards() {
    if (firstCard) firstCard->setText("?");
    if (secondCard) secondCard->setText("?");
    firstCard = nullptr;
    secondCard = nullptr;
}
