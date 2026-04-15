#include "pages/PlayerModePage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

PlayerModePage::PlayerModePage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #2f2f2f; color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);

    QLabel *titleLabel = new QLabel("Player Mode", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 700;");

    QLabel *infoLabel = new QLabel(
        "Go through lessons on rules and strategy, test your knowledge with quizzes, and play the penalty kick game.",
        this
        );
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("font-size: 16px; color: #d0d0d0;");

    QPushButton *lessonsButton = new QPushButton("Open Player Lessons", this);
    QPushButton *backButton = new QPushButton("Back to Mode Selection", this);
    QPushButton *playGameButton = new QPushButton("Play the Game", this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(infoLabel);
    layout->addSpacing(20);
    layout->addWidget(lessonsButton);
    layout->addWidget(backButton);
    layout->addWidget(playGameButton);
    layout->addStretch();

    connect(lessonsButton, &QPushButton::clicked,
            this, &PlayerModePage::openLessonsRequested);
    connect(backButton, &QPushButton::clicked,
            this, &PlayerModePage::backRequested);
    connect(playGameButton, &QPushButton::clicked,
            this, &PlayerModePage::playGameClicked);
}
