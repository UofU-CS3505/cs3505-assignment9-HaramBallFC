#include "pages/HomePage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #2f2f2f; color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);

    QLabel *titleLabel = new QLabel("HaramBall FC", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: 700;");

    QLabel *subtitleLabel = new QLabel(
        "A World Cup inspired soccer learning app built with Qt Widgets.",
        this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setWordWrap(true);
    subtitleLabel->setStyleSheet("font-size: 16px; color: #d0d0d0;");

    QPushButton *fanButton = new QPushButton("Fan Mode", this);
    QPushButton *playerButton = new QPushButton("Player Mode", this);
    QPushButton *bracketButton = new QPushButton("Bracket", this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addSpacing(20);
    layout->addWidget(fanButton);
    layout->addWidget(playerButton);
    layout->addWidget(bracketButton);
    layout->addStretch();

    connect(fanButton, &QPushButton::clicked,
            this, &HomePage::fanModeClicked);
    connect(playerButton, &QPushButton::clicked,
            this, &HomePage::playerModeClicked);
    connect(bracketButton, &QPushButton::clicked,
            this, &HomePage::bracketClicked);
}
