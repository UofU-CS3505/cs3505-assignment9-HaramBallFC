#include "pages/HomePage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("HaramBall FC", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *subtitleLabel = new QLabel(
        "A World Cup inspired soccer learning app built with Qt Widgets.",
        this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setWordWrap(true);

    QPushButton *fanButton = new QPushButton("Fan Mode", this);
    QPushButton *playerButton = new QPushButton("Player Mode", this);
    QPushButton *bracketButton = new QPushButton("Bracket", this);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addSpacing(20);
    layout->addWidget(fanButton, 0, Qt::AlignCenter);
    layout->addWidget(playerButton, 0, Qt::AlignCenter);
    layout->addWidget(bracketButton, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(fanButton, &QPushButton::clicked,
            this, &HomePage::fanModeClicked);
    connect(playerButton, &QPushButton::clicked,
            this, &HomePage::playerModeClicked);
    connect(bracketButton, &QPushButton::clicked,
            this, &HomePage::bracketClicked);
}
