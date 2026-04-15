#include "pages/PenaltyGame.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

PenaltyGamePage::PenaltyGamePage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #2f2f2f; color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);

    QLabel *titleLabel = new QLabel("Penalty Kick Game", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 700;");

    QLabel *subtitleLabel = new QLabel(
        "This is where the Box2D mini-game will go.",
        this
        );
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 16px; color: #d0d0d0;");

    QPushButton *backButton = new QPushButton("Back to Player Mode", this);
    backButton->setCursor(Qt::PointingHandCursor);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addSpacing(20);
    layout->addWidget(backButton, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(backButton, &QPushButton::clicked,
            this, &PenaltyGamePage::backClicked);
}
