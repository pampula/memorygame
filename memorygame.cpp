#include "memorygame.h"
#include <QComboBox>
#include <QLabel>
#include <QRandomGenerator>
#include <QTimer>
#include <algorithm>

MemoryGame::MemoryGame(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Memory Game");
    resize(1000, 800);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    createStartView();
    createGameView();

    stackedWidget->setCurrentWidget(startView);
}

MemoryGame::~MemoryGame(){}

void MemoryGame::createStartView() {
  startView = new QWidget;
  QVBoxLayout *mainLayout = new QVBoxLayout(startView);

  QLabel *titleLabel = new QLabel("Memory Game");
  titleLabel->setStyleSheet("font-size: 30px; font-weight: bold; color: #4a4a4a;");
  titleLabel->setAlignment(Qt::AlignCenter);

  QHBoxLayout *selectionLayout = new QHBoxLayout();
  QLabel *cardAmountLabel = new QLabel("Number of Cards:");
  QComboBox *cardAmountSelector = new QComboBox();
  cardAmountSelector->addItems(gridSizes);
  cardAmountSelector->setMaximumWidth(200);
  cardAmountLabel->setStyleSheet("font-size: 16px; color: #4a4a4a;");
  selectionLayout->addWidget(cardAmountLabel);
  selectionLayout->addWidget(cardAmountSelector);
  selectionLayout->setAlignment(Qt::AlignCenter);

  QPushButton *startButton = new QPushButton("Start Game");
  startButton->setMaximumWidth(150);
  startButton->setStyleSheet(
      "QPushButton {"
      "    font-size: 16px; color: white; background-color: #4285f4; border-radius: 8px; padding: 8px 16px;"
      "}"
      "QPushButton:hover { background-color: #3072e0; }"
      "QPushButton:pressed { background-color: #2c5dab; }"
  );

  mainLayout->addWidget(titleLabel);
  mainLayout->addLayout(selectionLayout);
  mainLayout->addWidget(startButton, 0, Qt::AlignCenter);

  startView->setLayout(mainLayout);

  connect(startButton, &QPushButton::clicked, this, &MemoryGame::startGame);

  stackedWidget->addWidget(startView);
}

void MemoryGame::createGameView() {
    gameView = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(gameView);

    QHBoxLayout *controlsLayout = new QHBoxLayout();
    QPushButton *restartButton = new QPushButton("Restart Game", this);
    restartButton->setMaximumWidth(150);
    restartButton->setStyleSheet(
          "QPushButton {"
          "    font-size: 16px; color: white; background-color: #4285f4; border-radius: 8px; padding: 8px 16px;"
          "}"
          "QPushButton:hover { background-color: #3072e0; }"
          "QPushButton:pressed { background-color: #2c5dab; }"
      );

    controlsLayout->addWidget(restartButton);
    layout->addLayout(controlsLayout);

    gridLayout = new QGridLayout();
    layout->addLayout(gridLayout);

    setupGame();

    resetTimer = new QTimer(this);
    resetTimer->setSingleShot(true);
    connect(resetTimer, &QTimer::timeout, this, &MemoryGame::resetCards);
    connect(restartButton, &QPushButton::clicked, this, &MemoryGame::restartGame);

    gameView->setLayout(layout);

    stackedWidget->addWidget(gameView);
}

void MemoryGame::startGame() {
    stackedWidget->setCurrentWidget(gameView);
}

void MemoryGame::setupGame() {

    QVector<int> cardValues = generateShuffledValues();
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

    QVector<int> cardValues = generateShuffledValues();
    for (int i = 0; i < cards.size(); ++i) {
        cards[i]->setProperty("value", cardValues[i]);
    }
}

QVector<int> MemoryGame::generateShuffledValues() {
    int totalPairs = (gridSize * gridSize) / 2;
    QVector<int> cardValues;

    for (int i = 1; i <= totalPairs; ++i) {
        cardValues.append(i);
        cardValues.append(i);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cardValues.begin(), cardValues.end(), std::default_random_engine(seed));
    return cardValues;
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
          "QPushButton {"
            "background-color: #4285f4;"
            "border-radius: 15px;"
            "border: 2px solid #4285f4;"
            "font-size: 20px;"
          "}"
          "QPushButton:hover { background-color: #3072e0; }"
          "QPushButton:pressed { background-color: #2c5dab; }"
    );
    card->setText("");
}

void MemoryGame::setCardTurned(QPushButton *card) {
    card->setStyleSheet(
          "background-color: #9dbdf2;"
          "border-radius: 15px;"
          "border: 2px solid #9dbdf2;"
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
