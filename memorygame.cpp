#include "memorygame.h"
#include <QRandomGenerator>
#include <QTimer>
#include <algorithm>

MemoryGame::MemoryGame(QWidget *parent)
    : QMainWindow(parent), firstCard(nullptr), secondCard(nullptr) {
    setWindowTitle("Memory Game");
    resize(800, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *controlsLayout = new QHBoxLayout();
    QPushButton *restartButton = new QPushButton("Restart Game", this);
    controlsLayout->addWidget(restartButton);
    mainLayout->addLayout(controlsLayout);

    gridLayout = new QGridLayout();
    mainLayout->addLayout(gridLayout);

    setupGame();

    resetTimer = new QTimer(this);
    resetTimer->setSingleShot(true);
    connect(resetTimer, &QTimer::timeout, this, &MemoryGame::resetCards);
    connect(restartButton, &QPushButton::clicked, this, &MemoryGame::restartGame);
}

MemoryGame::~MemoryGame() {}

void MemoryGame::setupGame() {
    QVector<int> cardValues{1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cardValues.begin(), cardValues.end(), std::default_random_engine(seed));

    for (int i = 0; i < gridSize * gridSize; ++i) {
        QPushButton *card = new QPushButton(this);
        card->setFixedSize(100, 100);
        setCardHidden(card);
        card->setProperty("value", cardValues[i]);

        connect(card, &QPushButton::clicked, [this, card]() { cardClicked(card); });

        gridLayout->addWidget(card, i / gridSize, i % gridSize);
        cards.append(card);
    }
}

void MemoryGame::restartGame() {
    firstCard = nullptr;
    secondCard = nullptr;

    for (QPushButton *card : cards) {
        card->setEnabled(true);
        setCardHidden(card);
    }

    QVector<int> cardValues{1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cardValues.begin(), cardValues.end(), std::default_random_engine(seed));

    for (int i = 0; i < cards.size(); ++i) {
        cards[i]->setProperty("value", cardValues[i]);
    }
}


void MemoryGame::cardClicked(QPushButton *card) {
    // Two cards are already turned
    if (firstCard && secondCard) return;

    card->setText(QString::number(card->property("value").toInt()));

    if (!firstCard) {
        firstCard = card;
        setCardTurned(firstCard);
    }
    else if (!secondCard) {
        // Ensure the second card is not the same as the first
        if (card == firstCard) return;

        secondCard = card;
        setCardTurned(secondCard);

        if (firstCard->property("value") == secondCard->property("value")) {
            setCardDisabled(firstCard);
            setCardDisabled(secondCard);
            firstCard = nullptr;
            secondCard = nullptr;
        }
        else {
            resetTimer->start(500);
        }
    }
}

void MemoryGame::resetCards() {
    if (firstCard) {
        setCardHidden(firstCard);
    }
    if (secondCard) {
        setCardHidden(secondCard);
    }
    firstCard = nullptr;
    secondCard = nullptr;
}

void MemoryGame::setCardHidden(QPushButton *card) {
    card->setStyleSheet(
          "background-color: #578e58;"
          "border-radius: 15px;"
          "border: 2px solid #388E3C;"
          "font-size: 20px;"
    );
    card->setText("");
}

void MemoryGame::setCardTurned(QPushButton *card) {
    card->setStyleSheet(
          "background-color: #bed1be;"
          "border-radius: 15px;"
          "border: 2px solid #bed1be;"
          "color: black;"
          "font-size: 20px;"
    );
}

void MemoryGame::setCardDisabled(QPushButton *card) {
    card->setEnabled(false);
    card->setStyleSheet(
          "background-color: #cccccc;"
          "border-radius: 15px;"
          "border: 2px solid #cccccc;"
          "color: #898989;"
          "font-size: 20px;"
    );
}
