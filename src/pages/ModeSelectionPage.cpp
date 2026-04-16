#include "pages/ModeSelectionPage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

ModeSelectionPage::ModeSelectionPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #2f2f2f; color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);

    QLabel *titleLabel = new QLabel("Choose Your Learning Path", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 700;");

    QLabel *descriptionLabel = new QLabel(
        "Fan Mode: Learn the rules, history, and key moments of the World Cup so you can follow every match with confidence.\n"
        "Player Mode: Go through lessons, test your knowledge with quizzes, and play interactive games to sharpen your skills.",
        this);
    descriptionLabel->setAlignment(Qt::AlignCenter);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("font-size: 16px; color: #d0d0d0;");

    QPushButton *fanButton = new QPushButton("Fan Mode", this);
    QPushButton *playerButton = new QPushButton("Player Mode", this);
    QPushButton *bracketButton = new QPushButton("Bracket", this);
    QPushButton *backButton = new QPushButton("Back", this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(descriptionLabel);
    layout->addSpacing(20);
    layout->addWidget(fanButton);
    layout->addWidget(playerButton);
    layout->addWidget(bracketButton);
    layout->addWidget(backButton);
    layout->addStretch();

    connect(fanButton, &QPushButton::clicked,
            this, &ModeSelectionPage::fanModeSelected);
    connect(playerButton, &QPushButton::clicked,
            this, &ModeSelectionPage::playerModeSelected);
    connect(bracketButton, &QPushButton::clicked,
            this, &ModeSelectionPage::bracketSelected);
    connect(backButton, &QPushButton::clicked,
            this, &ModeSelectionPage::backRequested);
}
